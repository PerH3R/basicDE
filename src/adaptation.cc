#include "../include/adaptation.h"

AdaptationManager::AdaptationManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : argparser(argparser), 
	problem(problem), budget(budget), n(this->calc_population_size(argparser)), dim(problem->meta_data().n_variables){
	for (int i = 0; i < NUM_MUTATION_OPERATORS; ++i) {
	  this->available_mutops[i] = 1;
	}
	// this->budget = budget;
	this->F = std::stod(argparser->get_values()["-F"]);
	this->Cr = std::stod(argparser->get_values()["-Cr"]);
	this->archive_size = std::stoi(argparser->get_values()["-archive"]);
	this->lp = std::stoi(argparser->get_values()["-lp"]);
	
	// if (std::stoi(argparser->get_values()["-archive"]) < 0){
	// 	this->archive_size = this->n;
	// } //automatic archive size equal to population size when negative value is passed
	this->base_selection = std::stoi(argparser->get_values()["-s"]);
	this->base_crossover = std::stoi(argparser->get_values()["-c"]);
	this->base_boundary = std::stoi(argparser->get_values()["-b"]);
	this->base_mutation = std::stoi(argparser->get_values()["-m"]);
	if (std::stoi(argparser->get_values()["-archive"]) < 0){
		this->resample_limit = 10+std::pow((std::log(this->dim)),2);
	}else{
		this->resample_limit = std::stoi(argparser->get_values()["-archive"]);
	}
	this->credit_assigner = get_credit_operator(std::stoi(argparser->get_values()["-credit"]));
}

AdaptationManager::~AdaptationManager(){
	delete this->pop;
	this->pop = NULL;
}


std::shared_ptr<Credit> AdaptationManager::get_credit_operator(int crd_op){
	switch(crd_op){
			case 0:
				return std::make_shared<FitnessImprovement>(this->lp);
			case 1:
				return std::make_shared<TanhFitnessImprovement>(this->lp);
			case 2:
				return std::make_shared<BinaryImprovement>(this->lp);
			case 3:
				return std::make_shared<R2Improvement>(this->lp);
			default:
				return std::make_shared<R2Improvement>(this->lp);
		}
}


FixedManager::FixedManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	this->create_population();
}

void FixedManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
}

RandomManager::RandomManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF) : 
	AdaptationManager(argparser, problem, budget), RandomizeF(RandomizeF){
	this->create_population();
}

void RandomManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
	// initialize population with random operators
	for (size_t i = 0; i < this->pop->get_population_size(); ++i){
		int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS-1); //TODO: doublecheck values, -1 to exclude randomsearch
		this->pop->set_individual_mutation(this->pop->create_mutation_operator(new_m, this->F), i);
	}
}

void RandomManager::adapt(const double& previous_best_fitness){
	(void)previous_best_fitness; //silence warning  for unused parameter

	iteration_counter++;
	if (iteration_counter % this->lp == 0){ // apply operator randomization	once lp is reached
		for (size_t i = 0; i < this->pop->get_population_size(); ++i){
			int new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS-1); 	// exclude random search
			if (new_m == 6 && tools.rand_double_unif(0,1) > 1/(2*5)){ 		// BEA uses many evaluations so reroll based on Nclones and Nsegments (default values)
				new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS-1);	// in order to keep iterations/operator the same
			}
			double new_F = this->F;
			if (this->RandomizeF){
				pop->set_individual_mutation(this->pop->create_mutation_operator(new_m, tools.rand_double_unif(0.2,0.8)), i);
			}else{
				pop->set_individual_mutation(this->pop->create_mutation_operator(new_m, new_F), i);

			}
			auto mutation_ptr = pop->get_current_generation()[i]->get_mutation_ptr();
			CROSSOVER crossover_type = CROSSOVER(std::stoi(argparser->get_values()["-c"]));
			if (this->RandomizeF == false){
				// use predetermined best F if not random F
				mutation_ptr->auto_set_F(crossover_type);
			}
			
			// use best predetermined Cr crossover value for mutation+crossover operator combination
			pop->get_current_generation()[i]->set_crossover(
				pop->create_crossover_operator(crossover_type, mutation_ptr->get_predetermined_Cr(crossover_type))
			);

		}
	}
}

MABManager::MABManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	std::cout << "Using Multi-Armed Bandit approach." << std::endl;
	this->create_population();
	this->eps_a = std::stod(argparser->get_values()["-eps_a"]);
	this->alpha = std::stod(argparser->get_values()["-MABdecay"]);
	this->MABsel = std::stoi(argparser->get_values()["-MABsel"]);
	this->logging = !!std::stoi(argparser->get_values()["-logQ"]);
	std::cout << "logging=" << logging << std::endl;
	//seed config database with base configurations
	for (int i = 0; i <= NUM_MUTATION_OPERATORS-1; ++i){
		if (tools.extract_bit(std::stoi(argparser->get_values()["-ops"]), i)){
			// print if operator is added
			std::cout << MUTATION_NAMES[i] << ':' << '\t' << "True" << std::endl;

			auto mutation_ptr = this->pop->create_mutation_operator(i, -1);
			CROSSOVER crossover_type = CROSSOVER(std::stoi(argparser->get_values()["-c"]));
			mutation_ptr->auto_set_F(crossover_type);
			auto crossover_ptr = this->pop->create_crossover_operator(i, -1);
			crossover_ptr->set_Cr(mutation_ptr->get_predetermined_Cr(crossover_type));
			operator_configuration new_config = {
				mutation_ptr,
				crossover_ptr,
				{1.0}, //initial Q
				{0.0}, //credited improvement
				{0.0}, //raw improvement
				{0}	   //iteration counter
			};
			operator_configurations.push_back(new_config);
		} else {
			// print if operator is not added
			std::cout << MUTATION_NAMES[i] << ": "  << "False" << std::endl;
		}
	}
	//randomly set configurations from operator_configurations on agents
	for (size_t i = 0; i < this->n; ++i){
		int new_config_idx = tools.rand_int_unif(0, operator_configurations.size());
		set_config_on_agent(operator_configurations[new_config_idx], i);
	}

	// set up logging files and directories
	if (this->logging){
		Qlog_init();
	}
}

void MABManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
}

void MABManager::adapt(const double& previous_best_fitness){
	iteration_counter++;	
	if (iteration_counter % this->lp == 0){ // apply operator adaptation once lp is reached

		update_scores(previous_best_fitness);
		//calculate weighted scores
		total_Q = 0.0;

		for (size_t i = 0; i < this->n; ++i){ //for cur and next gen
			// get random config from tools.rand_double_unif(0,total_Q);
			operator_configuration new_config = get_new_config();
			this->set_config_on_agent(new_config, i);
		}

	}
	return;
}

void MABManager::set_config_on_agent(AdaptationManager::operator_configuration new_config, int a_idx){
	this->pop->set_individual_mutation(new_config.mutation_operator, a_idx);
	this->pop->set_individual_crossover(new_config.crossover_operator, a_idx);
	return;
}

AdaptationManager::operator_configuration MABManager::get_new_config(){
	//find best configuration TODO: how to choose
	int selected_config_idx;


	if (MABsel==0){ //greedy selection
		double best_Q = std::numeric_limits<double>::min();
		for (size_t i = 0; i < operator_configurations.size(); ++i){
			if (operator_configurations[i].Q.back() > best_Q){
				selected_config_idx = i;
				best_Q = operator_configurations[selected_config_idx].Q.back();
			}
		}
	} else if (MABsel == 1){ //proportionate selection
		double total_Q = 0.0;
		for (size_t i = 0; i < operator_configurations.size(); ++i){
			total_Q += operator_configurations[i].Q.back();
		}
		double prop_rand_val = tools.rand_double_unif(0.0, total_Q);
		double rand_tracker = 0.0;
		selected_config_idx = 0;

		for (size_t i = 0; i < operator_configurations.size(); ++i){
			rand_tracker += operator_configurations[i].Q.back();
			if(rand_tracker >= prop_rand_val){
				selected_config_idx = i;
				break;
			}
		}
	}
	

	//choose a random config instead with chance
	if (tools.rand_double_unif(0,1) < this->eps_a){
		//do we choose at random from existing configurations or attempt to create a new one
		bool createnew = false;
		if (tools.rand_double_unif(0,1) < this->new_config_chance){ //TODO how to update F and Cr
			createnew = true;
		}
		if(createnew){ //PARTIALLY SET UP -- if new configs can be created, do so (see createnew in adptation.h)
		//creating new config
			while(true){			
				//create new random config
				//TODO: make work for EXPONENTIAL crossover
				int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS-1); //exclude randomsearch
				auto m_ptr = this->pop->create_mutation_operator(new_m);
				m_ptr->auto_set_F(BINOMIAL);
				auto cr_ptr = this->pop->create_crossover_operator(BINOMIAL, m_ptr->get_predetermined_Cr(BINOMIAL));
				cr_ptr->set_Cr(m_ptr->get_predetermined_Cr(BINOMIAL));
				operator_configuration new_config = {
					m_ptr,
					cr_ptr,
					{1.0},
					{},
					{},
					{}
				};
				//if config does not exist yet
				if (config_in_configs(new_config) == -1){
					operator_configurations.push_back(new_config);
					return operator_configurations.back();
				}else{
					//if exists
					//make sure it is not by chance the same as best known config
					//TODO: change for proportionate selection
					if (config_in_configs(new_config) != selected_config_idx){ 
						return operator_configurations[config_in_configs(new_config)];
					}else{
						//try again with new random config
					}
				}
			}
		}else{ //choose random from existing configs
			int new_config_idx;
			do{
				new_config_idx = tools.rand_int_unif(0,operator_configurations.size()); 
			}while(selected_config_idx != new_config_idx && MABsel == 0); //in case of 'greedy' selection, dont randomize to the 'best' operator

			selected_config_idx = new_config_idx;
		}
	}

	return operator_configurations[selected_config_idx];
}

int MABManager::config_in_configs(operator_configuration new_config){
	int config_idx = -1;
	for (size_t i = 0; i < operator_configurations.size(); ++i){
		if (new_config.mutation_operator->get_F() == operator_configurations[i].mutation_operator->get_F() &&
			new_config.mutation_operator->get_type() == operator_configurations[i].mutation_operator->get_type() &&
			new_config.crossover_operator->get_Cr() == operator_configurations[i].crossover_operator->get_Cr() &&
			new_config.crossover_operator->get_type() == operator_configurations[i].crossover_operator->get_type()){
			return i;
		}
	}
	return config_idx;
}

void MABManager::update_scores(const double& previous_best_fitness){

	//calculate average position over all Agents, can be used for credit assignment
	std::vector<double> average_position(this->dim, 0.0);
	for (auto agent : this->pop->get_current_generation()){
		average_position = tools.vec_sum(average_position, agent->get_position());
	}
	average_position = tools.vec_scale(average_position, 1/this->n);

	for (auto agent : this->pop->get_current_generation()){
		int idx = -1;
		//find if config of agent already exists in operator_configurations
		for (size_t config = 0; config < operator_configurations.size(); config++){
			if (agent_has_config(agent, operator_configurations[config])){
				//config exists in database
				// std::cout << "f";
				idx = config;
				break;
			}
		}
		//add config to operator_configurations if not found
		if (idx == -1){ 
			//create new_config
			add_config_from_agent(agent);
			idx = operator_configurations.size()-1;
		}

		//calculate obtained credit trough fitness improvement over last learning period iterations of agent
		const std::vector< std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> >& hist = agent->get_history();
		double achieved_credit = credit_assigner->get_credit(hist, average_position, previous_best_fitness);
		operator_configurations[idx].lp_improvements.push_back(achieved_credit);
	}

	//calculate updated Q-values and update
	for(operator_configuration& config : this->operator_configurations){
		if (!config.lp_improvements.empty()){
			// average all obtained credit over all agents with a given configuration in the past learning period 
			double score_avg = tools.vec_avg(config.lp_improvements);
			config.scores.push_back(score_avg); //TODO: possible alternatives max(), sum()

			// Update Q-value by decaying past values and adding newest score
			double new_Q = config.scores.back() * (1-this->alpha) + config.Q.back() * (1-this->alpha);
			config.Q.push_back(new_Q);
			config.Qbudget.push_back(iteration_counter);
		}else{
			// configuration not used in last learning period
		}		
		config.lp_improvements.clear();
	}
	// std::cout << std::endl;
}

void MABManager::add_config_from_agent(Agent* a){
	for (auto c : operator_configurations){
		if(agent_has_config(a, c)){
			return;
		}
	}
	std::cout << "adding new config" << std::endl;
	operator_configuration new_config = {
			a->get_mutation_ptr(),
			a->get_crossover_ptr(),
			{1.0}, //initial Q
			{0.0}, //credited improvement
			{0.0}, //raw improvement
			{this->iteration_counter}	   //iteration counter
		};
	operator_configurations.push_back(new_config);
	return;
}

bool AdaptationManager::agent_has_config(Agent* a, const operator_configuration& o){
	if (a->get_mutation_ptr() == o.mutation_operator &&
		a->get_crossover_ptr() == o.crossover_operator){
		return true;
	}
	return false;
} 


void MABManager::Qlog_init(){
	std::ofstream Q_log;

	// construct path and name from current settings
	std::string Qlogger_adap_dir = fmt::format("Qresults/a{}m{}/", argparser->get_values()["-a"], argparser->get_values()["-m"]);
	
	std::string Qlogger_main_dir =  "-ops"+ std::bitset<NUM_MUTATION_OPERATORS>(std::stoi(argparser->get_values()["-ops"])).to_string() + "/";
									// "-d"+argparser->get_values()["-d"] +
									// "-pop_size"+ argparser->get_values()["-pop_size"] +
									// "lp" + argparser->get_values()["-lp"] + 
									// "eps" + argparser->get_values()["-eps_a"] + 
									// "sel" + argparser->get_values()["-MABsel"] +
									// "crd" + argparser->get_values()["-credit"];

	std::string Qlogger_func_dir = fmt::format("data_f{:d}_{}", this->problem->meta_data().problem_id, this->problem->meta_data().name);

	std::string Qlogger_name = fmt::format("Qlog_f{:d}_DIM{:d}.csv", this->problem->meta_data().problem_id, this->problem->meta_data().n_variables);

	this->Qlogger_location = Qlogger_adap_dir + 
								Qlogger_main_dir + 
								// Qlogger_func_dir + 
								Qlogger_name;

	tools.create_dir(Qlogger_adap_dir + Qlogger_main_dir);
	// create new file if first instance in suite
	if(this->problem->meta_data().instance == 1){
		tools.create_dir(Qlogger_adap_dir + Qlogger_main_dir);
		Q_log.open(Qlogger_location, std::ofstream::out);
	} else{ 
		Q_log.open(Qlogger_location, std::ofstream::app);
	}
	
	//create header data
	Q_log << "Budget";

	for (auto opconfig : operator_configurations){
		Q_log << ',' << MUTATION_NAMES[opconfig.mutation_operator->get_type()];
	}
	Q_log << '\n';
	Q_log.close();
	std::cout << "logging at: " << Qlogger_location << std::endl;

}

void MABManager::log_Qs(){
	std::cout << fmt::format("Logging instance {}: \n", this->problem->meta_data().instance);

	std::ofstream Q_log;
	Q_log.open(this->Qlogger_location, std::ofstream::app);
	// log values of Q over time per configuration
	for (unsigned int i = 0; i <= this->iteration_counter; i+=this->lp){
		Q_log <<  i;
		for (auto opconfig : operator_configurations){
			auto iteration_iterator = std::find(opconfig.Qbudget.begin(), opconfig.Qbudget.end(), i);
			if (iteration_iterator != opconfig.Qbudget.end()){
				Q_log << ',' << fmt::format("{}", opconfig.Q[iteration_iterator - opconfig.Qbudget.begin()]);
			} else {
				Q_log << ',' ;
			}
		}
		Q_log << '\n';
	}
	Q_log.close();
}

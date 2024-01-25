#include "../include/adaptation.h"

AdaptationManager::AdaptationManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : argparser(argparser), 
	problem(problem), budget(budget), n(this->calc_population_size(argparser)), dim(problem->meta_data().n_variables){
	for (int i = 0; i < NUM_MUTATION_OPERATORS; ++i) {
	  this->available_mutops[i] = 1;
	}
	// this->argparser = argparser;
	// this->problem = problem;
	this->budget = budget;
	this->F = std::stod(argparser->get_values()["-F"]);
	this->Cr = std::stod(argparser->get_values()["-Cr"]);
	// this->dim = problem->meta_data().n_variables;
	// n = std::stoi(argparser->get_values()["-pop_size"]);
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
	this->credit_assigner = new FitnessImprovement(this->lp);
}

AdaptationManager::~AdaptationManager(){
	delete this->pop;
	this->pop = NULL;
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
	for (int i = 0; i < this->pop->get_population_size(); ++i){
		int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS-1); //TODO: doublecheck values, -1 to exclude randomsearch
		this->pop->set_individual_mutation(this->pop->get_mutation_operator(new_m, this->F), i);
	}
}

void RandomManager::adapt(unsigned int iterations){
	iteration_counter++;
	if (iteration_counter % this->lp == 0){ // how often is adaptation applied	
		for (int i = 0; i < this->pop->get_population_size(); ++i){
			int new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS-1); 	//exclude random search
			if (new_m == 6 && tools.rand_double_unif(0,1) > 1/(2*5)){ 		// BEA uses many evaluations so reroll based on Nclones and Nsegments (default values)
				new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS-1);	// in order to keep iterations/operator the same
			}
			double new_F = this->F;
			if (this->RandomizeF){
				new_F = tools.rand_double_unif(0.2,0.8);
			}
			pop->set_individual_mutation( this->pop->get_mutation_operator(new_m, new_F), i);
			auto mutation_ptr = pop->get_current_generation()[i]->get_mutation_ptr();
			CROSSOVER crossover_type = BINOMIAL;
			mutation_ptr->auto_set_F(BINOMIAL);			
			pop->get_current_generation()[i]->set_crossover(
				pop->get_crossover_operator(crossover_type, mutation_ptr->get_predetermined_Cr(crossover_type))
			);

		}
	}
}

MABManager::MABManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	std::cout << "Using Multi-Armed Bandit approach." << std::endl;
	this->create_population();
	//seed config database with base configurations
	for (int i = 0; i < NUM_MUTATION_OPERATORS-1; ++i){
		auto tmp_mutation_ptr = this->pop->get_mutation_operator(i, -1);
		operator_configuration new_config = {
			tmp_mutation_ptr->get_type(),
			tmp_mutation_ptr->auto_set_F(BINOMIAL),
			BINOMIAL,
			tmp_mutation_ptr->get_predetermined_Cr(BINOMIAL),
			{0.0},
			{},
			{}
		};
		operator_configurations.push_back(new_config);
	}
	//set random configurations from operator_configurations on agents
	for (int i = 0; i < this->n; ++i){
		int new_config_idx = tools.rand_int_unif(0, operator_configurations.size()); //TODO: doublecheck values
		set_config_on_agent(operator_configurations[new_config_idx], i);
	}

}

void MABManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
	for(Agent* a : this->pop->get_current_generation()){
		int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS-1); //-1 excludes random search
		a->get_mutation_ptr()->auto_set_F(BINOMIAL);
	}

}

void MABManager::adapt(unsigned int iterations){
	iteration_counter++;	
	if (iteration_counter % this->lp == 0){
		std::cout << "calculating new_scores" << std::endl;
		update_scores();
		//calculate weighted scores
		total_Q = 0.0;
		std::cout << "Q_vals: ";
		for(operator_configuration& c : this->operator_configurations){
			// if (!c.scores.empty()){
				// double new_Q = c.scores.back() * (1-this->alpha) + c.Q.back() * (1-this->alpha);
				// c.Q.push_back(new_Q);
				std::cout << c.Q.back() << " - ";;
				total_Q += c.Q.back();
			// }
				 
		}
		std::cout << std::endl;
		for (int i = 0; i < this->n; ++i){ //for cur and next gen
			// get random config from tools.rand_double_unif(0,total_Q);
			operator_configuration new_config = get_new_config();
			this->set_config_on_agent(new_config, i);
		}
		
		//https://stackoverflow.com/questions/10531565/how-should-roulette-wheel-selection-be-organized-for-non-sorted-population-in-g
	}
	return;
}

void MABManager::set_config_on_agent(AdaptationManager::operator_configuration new_config, int a_idx){
	this->pop->set_individual_mutation(this->pop->get_mutation_operator(new_config.mutation_type, new_config.F), a_idx);
	this->pop->set_individual_crossover(this->pop->get_crossover_operator(new_config.crossover_type, new_config.Cr), a_idx);
	return;
}

AdaptationManager::operator_configuration MABManager::get_new_config(){
	//find best configuration 
	//TODO how to choose
	int best_config_idx;
	double best_Q = std::numeric_limits<double>::min();
	for (int i = 0; i < operator_configurations.size(); ++i){
		if (operator_configurations[i].Q.back() > best_Q){
			best_config_idx = i;
			best_Q = operator_configurations[best_config_idx].Q.back();
		}
	}

	if (tools.rand_double_unif(0,1) < this->random_config_epsilon){
		//do we choose at random from existing configurations or attempt to create a new one
		bool createnew = false;
		if (tools.rand_double_unif(0,1) < 0.0){ //TODO how to update F and Cr
			createnew = true;
		}
		if(createnew){
		//creating new config
			while(true){			
				//create new random config
				//TODO: make work for EXPONENTIAL crossover
				int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS-1); //exclude randomsearch
				auto temp_m_ptr = pop->get_mutation_operator(new_m);
				operator_configuration new_config = {
					temp_m_ptr->get_type(),
					temp_m_ptr->auto_set_F(BINOMIAL),
					BINOMIAL,
					temp_m_ptr->get_predetermined_Cr(BINOMIAL),
					{0.0},
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
					if (config_in_configs(new_config) != best_config_idx){
						return operator_configurations[config_in_configs(new_config)];
					}else{
						//try again with new random config
					}
				}
			}
		}else{ //choose random from existing configs
			best_config_idx = tools.rand_int_unif(0,operator_configurations.size()); //TODO dont choose same one as best
		}
	}
	return operator_configurations[best_config_idx];
}

int MABManager::config_in_configs(operator_configuration new_config){
	int config_idx = -1;
	for (int i = 0; i < operator_configurations.size(); ++i){
		if (new_config.mutation_type == operator_configurations[i].mutation_type &&
		new_config.F == operator_configurations[i].F &&
		new_config.Cr == operator_configurations[i].Cr &&
		new_config.crossover_type == operator_configurations[i].crossover_type){
			return i;
		}
	}
	return -1;
}

void MABManager::update_scores(){
	//calculate average position over all Agents
	std::vector<double> average_position(this->dim, 0.0);
	for (auto agent : this->pop->get_current_generation()){
		average_position = tools.vec_sum(average_position, agent->get_position());
	}
	average_position = tools.vec_scale(average_position, 1/this->n);
	for (auto agent : this->pop->get_current_generation()){
		int idx = -1;
		//find corresponding config
		for (int config = 0; config < operator_configurations.size(); config++){
			if (agent_has_config(agent, operator_configurations[config])){
				//config exists in database
				std::cout << "f";
				idx = config;
				break;
			}
		}
		if (idx == -1){ //matching config not found
			//create new_config
			add_config_from_agent(agent);
			idx = operator_configurations.size()-1;
		}
		//calc fitness improvement over learning period of agent
		const auto hist = agent->get_history();
		// const double last_fitness = std::get<1>(hist[hist.size()-1]);
		// const double first_fitness = std::get<1>(hist[hist.size()-this->lp]); //why does rbegin iterator straight up not work :'(
		// double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		double achieved_credit = credit_assigner->get_credit(hist, average_position);
		operator_configurations[idx].lp_improvements.push_back(achieved_credit);
	}

	//apply scoring method and update
	for(operator_configuration& config : this->operator_configurations){
		if (!config.lp_improvements.empty()){
			double score_avg = tools.vec_avg(config.lp_improvements);
			config.scores.push_back(score_avg); //TODO: apply more scoring methods
			double new_Q = config.scores.back() * (1-this->alpha) + config.Q.back() * (1-this->alpha);
			config.Q.push_back(new_Q);
		}else{
			std::cout <<  "e";
		}		
		config.lp_improvements.clear();
	}
	std::cout << std::endl;
}

void MABManager::add_config_from_agent(Agent* a){
	for (auto c : operator_configurations){
		if(agent_has_config(a, c)){
			return;
		}
	}
	std::cout << "adding new config" << std::endl;
	operator_configuration new_config = {
			a->get_mutation(),
			a->get_mutation_ptr()->get_F(),
			a->get_crossover(),
			a->get_mutation_ptr()->get_predetermined_Cr(BINOMIAL),
			{0.0},
			{},
			{}
		};
	operator_configurations.push_back(new_config);
	return;
}

bool AdaptationManager::agent_has_config(Agent* a, const operator_configuration& o){
	//std:: cout << a->get_mutation_ptr()->get_type() << " " << a->get_mutation_ptr()->get_F() << " " << a->get_crossover_ptr()->get_Cr() << " " << a->get_crossover_ptr()->get_type() << " " << std::endl;
	//std::cout<< o.mutation_type << " " << o.F << " " << o.Cr << " " << o.crossover_type << " " << std::endl;
	if (a->get_mutation_ptr()->get_type() == o.mutation_type &&
		a->get_mutation_ptr()->get_F() == o.F &&
		a->get_crossover_ptr()->get_Cr() == o.Cr &&
		a->get_crossover_ptr()->get_type() == o.crossover_type){
		return true;
	}
	return false;
} 
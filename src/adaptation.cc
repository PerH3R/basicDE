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
	this->credit_assigner = new FitnessImprovement(lp);
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
		int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS); //TODO: doublecheck values
		this->pop->set_individual_mutation(this->pop->get_mutation_operator(new_m, this->F), i);
	}
}

void RandomManager::adapt(unsigned int iterations){
	iteration_counter++;
	if (iteration_counter % this->lp == 0){ // how often is adaptation applied	
		for (int i = 0; i < this->pop->get_population_size(); ++i){
			int new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS); 	//exclude random search
			if (new_m == 6 && tools.rand_double_unif(0,1) > 1/(2*5)){ 		// BEA uses many evaluations so reroll based on Nclones and Nsegments (default values)
				new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS);	// in order to keep iterations/operator the same
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
	this->create_population();
	for (int i = 0; i < NUM_MUTATION_OPERATORS; ++i){
		auto tmp_mutation_ptr = this->pop->get_mutation_operator(i, -1);
		operator_configuration new_config = {
			tmp_mutation_ptr->get_type(),
			tmp_mutation_ptr->get_F(),
			BINOMIAL,
			tmp_mutation_ptr->get_predetermined_Cr(BINOMIAL),
			{0.0},
			{},
			{},
			0.0
		};
		operator_configurations.push_back(new_config);

	}

}

void MABManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
}

void MABManager::adapt(unsigned int iterations){
	iteration_counter++;	
	if (iteration_counter % this->lp == 0){
		update_scores();
		/* code */
		for(auto c : operator_configurations){
			c.Q = 0.0;
			for(auto s : c.scores){

			}
		}
	}
	return;
}

void MABManager::update_scores(){
	//add all obtained scores to respective config
	std::vector<double> average_position(this->dim,0.0);
	for (auto agent : this->pop->get_current_generation()){
		average_position = tools.vec_sum(average_position, agent->get_position());
	}
	average_position = tools.vec_scale(average_position, 1/this->n);
	for (auto agent : this->pop->get_current_generation()){
		int idx = -1;
		//find corresponding config
		for (int config = 0; config < operator_configurations.size(); config++){
			if (agent->get_mutation_ptr()->get_type() == operator_configurations[config].mutation_type &&
					agent->get_mutation_ptr()->get_F() == operator_configurations[config].F &&
					agent->get_crossover_ptr()->get_Cr() == operator_configurations[config].Cr &&
					agent->get_crossover_ptr()->get_type() == operator_configurations[config].crossover_type){
				idx = config;
				break;
			} else{
				//add config
				//idx = operator_configurations.size() - 1;
				break;
			}
		}
		//calc fitness improvement over learning period of agent
		const auto hist = agent->get_history();
		// const double last_fitness = std::get<1>(hist[hist.size()-1]);
		// const double first_fitness = std::get<1>(hist[hist.size()-this->lp]); //why does rbegin iterator straight up not work :'(
		// double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		operator_configurations[idx].lp_improvements.push_back(credit_assigner->get_credit(hist, average_position));
	}

	//apply scoring method and update
	for(auto config : operator_configurations){
		if (config.lp_improvements.empty() == false){
			config.scores.push_back(tools.vec_avg(config.lp_improvements)); //TODO: apply more scoring methods
		}
		config.lp_improvements.clear();
	}

}
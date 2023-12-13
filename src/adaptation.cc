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
	this->lp = std::stoi(argparser->get_values()["-lp"])
	
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
				int new_m = tools.rand_int_unif(0,NUM_MUTATION_OPERATORS);	// in order to keep iterations/operator the same
			}
			double new_F = this->F;
			if (this->RandomizeF){
				double new_F = tools.rand_double_unif(0.2,0.8);
			}
			pop->set_individual_mutation( this->pop->get_mutation_operator(new_m, new_F), i);
		}
	}
}

AdaptiveBoks::AdaptiveBoks(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	this->create_population();
}

void AdaptiveBoks::create_population(){
	this->pop = new Population(this->argparser, this->problem, n, this->budget, this->archive_size, this->resample_limit);
}
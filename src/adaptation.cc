#include "../include/adaptation.h"

AdaptationManager::AdaptationManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget){
	this->argparser = argparser;
	this->problem = problem;
	this->budget = budget;
	this->F = std::stod(argparser->get_values()["-F"]);
	this->Cr = std::stod(argparser->get_values()["-Cr"]);
	// this->F = std::stod(argparser->get_values()["-F"]);
	this->dim = problem->meta_data().n_variables;
	this->n = std::stoi(argparser->get_values()["-pop_size"]);
	this->archive_size = std::stoi(argparser->get_values()["-archive"]);
	if (this->n <4){
		this->n = 5*problem->meta_data().n_variables;
	}//automatic population size if too small or not specified
	if (std::stoi(argparser->get_values()["-archive"]) < 0){
		this->archive_size = this->n;
	} //automatic archive size equal to population size when negative value is passed
	this->base_selection = std::stoi(argparser->get_values()["-s"]);
	this->base_crossover = std::stoi(argparser->get_values()["-c"]);
	this->base_boundary = std::stoi(argparser->get_values()["-b"]);
	this->base_mutation = std::stoi(argparser->get_values()["-m"]);
}

AdaptationManager::~AdaptationManager(){
	delete this->pop;
	this->pop = NULL;
}



FixedManager::FixedManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	this->create_population();
}

void FixedManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, this->n, this->budget, this->archive_size);
}

RandomManager::RandomManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF) : 
	AdaptationManager(argparser, problem, budget), RandomizeF(RandomizeF){
	this->create_population();
}

void RandomManager::create_population(){
	this->pop =  new Population(this->argparser, this->problem, this->n, this->budget, this->archive_size);
	for (int i = 0; i < this->pop->get_population_size(); ++i){
		int new_m = tools.rand_int_unif(0, NUM_MUTATION_OPERATORS); //TODO: doublecheck values
		this->pop->set_individual_mutation(this->pop->get_mutation_operator(new_m, this->F), i);
	}
}

void RandomManager::adapt(unsigned int iterations){
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
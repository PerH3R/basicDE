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
		this->n = 4 + 5*problem->meta_data().n_variables;
	}//automatic population size if too small or not specified

	this->base_selection = std::stoi(argparser->get_values()["-s"]);
	this->base_crossover = std::stoi(argparser->get_values()["-c"]);
	this->base_boundary = std::stoi(argparser->get_values()["-b"]);
	this->base_mutation = std::stoi(argparser->get_values()["-m"]);
}

AdaptationManager::~AdaptationManager(){
	delete this->pop;
	this->pop = NULL;
}


// //default mut_op = -1, F = -1.0
// Mutation* AdaptationManager::get_mutation_operator(int mut_op, float F){

// 	size_t archive = std::stoi(argparser->get_values()["-archive"]);

// 	if (mut_op == -1){
// 		mut_op = base_mutation;
// 	}

// 	switch(mut_op){
// 		case 0:
// 			return new RandDiv1(this->dim, this->n, this->F);
// 		case 1:
// 			return new RandDiv2(this->dim, this->n, this->F);
// 		case 2:
// 			return new BestDiv1(this->dim, this->n, this->F);
// 		case 3:
// 			return new BestDiv2(this->dim, this->n, this->F);
// 		case 4:			
// 			return new TargetToPBestDiv1(this->dim, this->n, this->F, archive);
// 		case 5:
// 			return new RandToBestDiv1(this->dim, this->n, this->F);
// 		case 6:
// 			return new RandToBestDiv2(this->dim, this->n, this->F);
// 		case 7:
// 			return new TargetToBestDiv1(this->dim, this->n, this->F);
// 		case 8:
// 			return new TargetToBestDiv2(this->dim, this->n, this->F);
// 		case 9:
// 			return new TargetToRandDiv1(this->dim, this->n, this->F);
// 		case 10:
// 			return new TargetToRandDiv2(this->dim, this->n, this->F);
// 		case 11:
// 			return new TwoOptDiv1(this->dim, this->n, this->F);
// 		case 12:
// 			return new Desmu(this->dim, this->n, this->F);
// 		case 13:
// 			return new Bea(this->dim, this->n, this->get_boundary_operator(), this->problem, this->budget, this->F);
// 		case 14:
// 			return new DirMut(this->dim, this->n, this->F);
// 		case 15:
// 			return new RandomSearch(this->dim, this->n, problem, this->F);
// 		default:
// 			std::cerr << "Mutation operator " << mut_op << " out of range. Continuing using RandDiv1." << std::endl;
// 			return new RandDiv1(this->dim, this->n, this->F);		
// 	}	
// }

// //default sel_op = -1
// Selection* AdaptationManager::get_selection_operator(int sel_op){
// 	if (sel_op == -1){
// 		sel_op = this->base_selection;
// 	}
// 	switch(sel_op){
// 		case 0:
// 			return new Elitist(this->n);
// 		default:
// 			return new Elitist(this->n);
// 	}
// }

// //default c_op = -1, Cr = 1.0
// Crossover* AdaptationManager::get_crossover_operator(int c_op, float Cr){
// 	if (Cr == -1.0){
// 		Cr = this->Cr;
// 	}
// 	if (c_op == -1){
// 		c_op = base_crossover;
// 	}
// 	switch(c_op){
// 		case 0:
// 			return new Binomial(this->dim, Cr);
// 		case 1:
// 			return new Exponential(this->dim, Cr);
// 		default:
// 			return new Binomial(this->dim, Cr);
// 	}
// }

// //default bound_op = -1
// Boundary* AdaptationManager::get_boundary_operator(int bound_op){
// 	if (bound_op == -1){
// 		bound_op = base_boundary;
// 	}
// 	switch(bound_op){
// 		case 0:
// 			return new Clamp(this->problem);
// 		case 1:
// 			return new Reflect(this->problem);
// 		case 2:
// 			return new Reinit(this->problem);
// 		default:
// 			return new Clamp(this->problem);
// 	}
// }

FixedManager::FixedManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : AdaptationManager(argparser, problem, budget) {
	this->create_population();
}

Population* FixedManager::create_population(){
	this->pop = new Population(this->argparser, this->problem, this->n, this->budget, this->archive_size);
}

RandomManager::RandomManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF) : 
	AdaptationManager(argparser, problem, budget), RandomizeF(RandomizeF){
	this->create_population();
}

Population* RandomManager::create_population(){
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
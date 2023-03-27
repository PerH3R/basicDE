#include "../include/tools.h"
#include "../include/population.h"
#include "../include/argparse.h"
// #include "ioh.h"
//#include "../include/functions.h"

#include <iostream>


struct results {
	std::vector<double> best_location;
	double best_fitness;
	unsigned iterations;
};

Mutation* get_mutation_operator(Argparse* argparser, ioh::problem::RealSingleObjective* problem, Boundary* boundary_correction, 
									unsigned int* budget){
	int mut_op = std::stoi(argparser->get_values()["-m"]);
	size_t pop_size = std::stoi(argparser->get_values()["-pop_size"]);
	size_t dim = std::stoi(argparser->get_values()["-d"]);
	float F = std::stod(argparser->get_values()["-F"]);
	size_t archive = std::stoi(argparser->get_values()["-archive"]);
	switch(mut_op){
		case 1:
			return new RandDiv1(dim, pop_size, F);
		case 2:
			return new BestDiv1(dim, pop_size, F);
		case 3:			
			return new TargetToPBestDiv1(dim, pop_size, F, archive);
		case 4:
			return new TargetToBestDiv2(dim, pop_size, F);
		case 5:
			return new TargetToRandDiv2(dim, pop_size, F);
		case 6:
			return new TwoOptDiv1(dim, pop_size, F);
		case 7:
			return new Desmu(dim, pop_size, F);
		case 8:
			return new Bea(dim, pop_size, boundary_correction, problem, budget, F);
		default:
			std::cerr << "Mutation operator out of range. Continuing using RandDiv1." << std::endl;
			return new RandDiv1(dim, pop_size, F);		
	}	
}



results return_value(std::vector<double> location, double fitness, int i){
  results result;
  result.best_location = location;
  result.best_fitness = fitness;
  result.iterations = i;
  return result;
}



//main loop
results single_problem(Population* pop, unsigned int* budget, size_t dimension) {
	double best_fitness = std::numeric_limits<double>::max();
	//worst_fitness = std::numeric_limits<double>::max;
	//TODO: dimension getters in Function?
	std::vector<double> best_location(dimension, -1);
	unsigned int iterations=0;
	
	//TODO: budget
	while (*budget > pop->get_population_size()) {
		iterations++;
		std::cout << "budget left: " << *budget << " iteration: " << iterations <<std::endl;
		//mutate
		pop->apply_mutation();

		//crossover
		pop->apply_crossover();

		//selection
		pop->apply_selection();

		// pop->print_fitness();
		pop->sort();
		// pop->print_fitness();

		//fitness improvement
		if (pop->get_current_generation()[0]->get_fitness() < best_fitness)
		{
			pop->print_fitness();
			best_fitness = pop->get_current_generation()[0]->get_fitness();
			best_location = pop->get_current_generation()[0]->get_position();
		}
		//additional stopping criteria
		//...
		std::cout << "==================" << std::endl;
	}
	return return_value(best_location, best_fitness, iterations);
}





int main(int argc, char* argv[]) {
	//check arguments
	int num_args = 1; //TODO: remove

	auto argparser = new Argparse(argc, argv);

	if (argc != num_args) {
		std::cerr << "expected " << num_args << " arguments, got " << argc << " " << argv <<"." <<std::endl;
	};

	tools.set_seed();

	//TODO: convert argv to correct type



	//TODO: finish command line params and remove below
	int function_num = std::stoi(argparser->get_values()["-f"]);

	unsigned int number_of_runs = std::stoi(argparser->get_values()["-runs"]);

	size_t pop_size = std::stoi(argparser->get_values()["-pop_size"]);

	size_t dim = std::stoi(argparser->get_values()["-d"]);

	int m = std::stoi(argparser->get_values()["-m"]);

	size_t archive_size = std::stoi(argparser->get_values()["-archive"]);

	unsigned int budget_value = std::stoi(argparser->get_values()["-budget"]);

	unsigned int* budget = &budget_value;

	//TODO: run logs for IOH

	//main loop
	for (size_t i = 0; i < number_of_runs; i++) {	
		//TODO: problem selector
		auto problem = new ioh::problem::bbob::Sphere(i, dim);		
		Boundary* boundary_correction = new Clamp(problem);
		Mutation* mutation = get_mutation_operator(argparser, problem, boundary_correction, budget); //new TargetToPBestDiv1(dim, pop_size);
		Crossover* crossover = new Binomial(dim);
		Selection* selection = new Elitist(pop_size);

		
		std::cout << "metadata" << problem->meta_data() << std::endl;
		std::cout << "optimizing using " << mutation->get_type() << std::endl;

		Population* pop = new Population(crossover, selection, mutation, problem, boundary_correction, pop_size, budget, archive_size);

		std::cout << "Run " << i << " ";
		//set_seed();

		results result = single_problem(pop, budget, dim);
		std::cout << "final result" << std::endl;
		pop->print_fitness();
		std::cout << std::endl;
		delete pop;
		delete problem;
		pop=NULL;
		problem=NULL;

		// delete function;
		delete mutation;
		delete crossover;
		delete selection;
		delete boundary_correction;
		// function=NULL;
		mutation=NULL;
		crossover=NULL;
		selection=NULL;
		boundary_correction=NULL;

	};

	
	return 0;
}
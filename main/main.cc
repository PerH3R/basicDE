#include "../include/tools.h"
#include "../include/population.h"
// #include "ioh.h"
//#include "../include/functions.h"

#include <iostream>


struct results {
	std::vector<double> best_location;
	double best_fitness;
	unsigned iterations;
};



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
		std::cout << "budget left: " << *budget << std::endl;
		//mutate
		pop->apply_mutation();

		//crossover
		pop->apply_crossover();

		//boundary check
		pop->apply_boundary_correction();

		//selection
		pop->apply_selection();

		pop->print_fitness();
		pop->sort();
		pop->print_fitness();

		if (pop->get_current_generation()[0]->get_fitness() < best_fitness)
		{
			best_fitness = pop->get_current_generation()[0]->get_fitness();
			best_location = pop->get_current_generation()[0]->get_position();
		}
		//additional stopping criteria
		//...
	}
	return return_value(best_location, best_fitness, iterations);
}





int main(int argc, char* argv[]) {
	//check arguments
	int num_args = 0; //TODO: remove

	if (argc != num_args) {
		std::cerr << "expected " << num_args << " arguments, got " << argc << " " << argv <<"." <<std::endl;
	};

	//TODO: convert argv to correct type


	//TODO: finish command line params and remove below
	int function_num = 0;
	unsigned int number_of_runs = 1;
	size_t pop_size = 5;
	size_t dim = 5;
	unsigned int budget_value = 5*pop_size;
	unsigned int* budget = &budget_value;


	//implemented testfunctions
	// Function* function;
	// switch (static_cast<FUNCTION> (function_num)) {
	// 	case SPHERE: function = new Sphere(); break;
	// 	case SCHWEFEL: function = new Schwefel(); break;
	// 	case ZAKHAROV: function = new Zakharov(); break;
	// 	case MICHALEWICZ: function = new Michalewicz(); break;
	// default:
	// 	std::cerr << "selected unavailable function\n";
	// 	return 1;
	// }

	//TODO: run logs

	//main loop
	for (size_t i = 0; i < number_of_runs; i++) {	
		//TODO: problem selector
		auto problem = new ioh::problem::bbob::Sphere(i, dim);

		Mutation* mutation = new Randdiv1(dim, pop_size);
		Crossover* crossover = new Binomial(dim);
		Selection* selection = new Elitist(pop_size);
		Boundary* boundary_correction = new Clamp(problem);

		
		std::cout << "metadata" << problem->meta_data().n_variables << std::endl;

		Population* pop = new Population(crossover, selection, mutation, problem, boundary_correction, pop_size, budget);

		std::cout << "Run " << i << " ";
		//set_seed();

		results result = single_problem(pop, budget, dim);
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
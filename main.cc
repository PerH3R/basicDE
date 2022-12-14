#include "../include/tools.h"
#include "../include/population.h"
//#include "../include/functions.h"

#include <iostream>


struct results {
	std::vector<double> best_location;
	double best_fitness;
	unsigned iterations;
};







//main loop
results loop(Function* f, Population* pop, unsigned iterations) {
	double best_fitness = std::numeric_limits<double>::max;
	//worst_fitness = std::numeric_limits<double>::max;


	for (unsigned i = 0; i < iterations; i++) {
		//mutate
		pop->apply_mutation();

		//crossover
		pop->apply_crossover();

		//boundary check


		//selection
		pop->apply_selection();

		pop->sort();
		best_fitness = pop->get_current_generation()[0]->get_fitness();

		//additional stopping criteria
	}
}





int main(int argc, char* argv[]) {
	//check arguments
	int num_args = 0; //TODO: remove

	if (argc != num_args) {
		std::cerr << "expected " << num_args << " arguments, got " << argc << ".";
	};

	//convert argv to correct type


	//TODO: finish command line params and remove below
	int function_num = 0;
	int number_of_runs = 10;
	size_t pop_size = 100;
	int dim = 2;


	//implemented testfunctions
	Function* function;
	Mutation* mutation = new Randdiv1(pop_size);
	Crossover* crossover = new Binomial();
	Selection* selection = new Elitist(pop_size);
	switch (static_cast<FUNCTION> (function_num)) {
		case SPHERE: function = new Sphere(); break;
		case SCHWEFEL: function = new Schwefel(); break;
		case ZAKHAROV: function = new Zakharov(); break;
		case MICHALEWICZ: function = new Michalewicz(); break;
	default:
		std::cerr << "selected unavailable function\n";
		return 1;
	}

	//initialise population
	Population* pop = new Population(crossover, selection, mutation, pop_size, dim);

	//TODO: run logs

	//main loop
	for (size_t i = 0; i < number_of_runs; i++) {
		std::cout << "Run " << i << std::endl;
		//set_seed();

		results result = loop(function, pop, number_of_runs);
	};

}
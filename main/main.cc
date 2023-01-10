#include "../include/tools.h"
#include "../include/population.h"
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
results loop(Function* f, Population* pop, unsigned iterations, size_t dimension) {
	double best_fitness = std::numeric_limits<double>::max();
	//worst_fitness = std::numeric_limits<double>::max;
	//TODO: dimension getters in Function?
	std::vector<double> best_location;
	best_location.reserve(dimension);
	

	for (unsigned i = 0; i < iterations; i++) {
		//mutate
		pop->apply_mutation();

		//crossover
		pop->apply_crossover();

		//boundary check


		//selection
		pop->apply_selection();

		pop->sort();

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

	//convert argv to correct type


	//TODO: finish command line params and remove below
	int function_num = 0;
	int number_of_runs = 10;
	size_t pop_size = 5;
	size_t dim = 2;


	//implemented testfunctions
	Function* function;
	Mutation* mutation = new Randdiv1(dim, pop_size);
	Crossover* crossover = new Binomial(dim);
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
	Population* pop = new Population(crossover, selection, mutation, function, pop_size, dim);

	//TODO: run logs

	//main loop
	for (size_t i = 0; i < number_of_runs; i++) {
		std::cout << "Run " << i << " ";
		//set_seed();

		results result = loop(function, pop, number_of_runs, dim);
		std::cout << result.best_fitness;
		for (float i: result.best_location){
			std::cout << i;
		}
		std::cout << std::endl;
	};
	return 0;
}
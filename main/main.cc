#include "../include/tools.h"
#include "../include/adaptation.h"
// #include "ioh.h"
//#include "../include/functions.h"

#include <iostream>


struct results {
	std::vector<double> best_location;
	double best_fitness;
	unsigned iterations;
};

inline std::shared_ptr<ioh::suite::Suite<ioh::problem::RealSingleObjective>> create_suite(int problem, int inst, int dim, const bool using_factory = true)
{
	const std::vector<int> problems{1,2,5};
    const std::vector<int> instances{1};
    // const std::vector<int> problems{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    // const std::vector<int> instances{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    std::vector<int> dimensions;
    if (dim == 0){ dimensions = {5,20}; }
    else{ dimensions = {dim}; }

    if (using_factory)
        return ioh::suite::SuiteRegistry<ioh::problem::RealSingleObjective>::instance()
            .create("BBOB", problems, instances, dimensions);
    return std::make_shared<ioh::suite::BBOB>(problems, instances, dimensions);
}

inline ioh::logger::Analyzer get_logger(Argparse* argparser, const std::string &folder_name = "results", const bool store_positions = true) //false
{
	std::string algname;
	if (std::stoi(argparser->get_values()["-m"]) != 99)
	{
		algname = MUTATION_NAMES[std::stoi(argparser->get_values()["-m"])];
		algname += 'F' + MUTATION_NAMES[std::stoi(argparser->get_values()["-F"])];
		algname += "Cr" + MUTATION_NAMES[std::stoi(argparser->get_values()["-Cr"])];
	} else {
		algname = "random operators";
	}
	std::string info = argparser->getArgsAsString();

    /// Instantiate a logger.
    using namespace ioh;
    return logger::Analyzer(
        {trigger::on_improvement}, // trigger when the objective value improves
        {},                        // no additional properties 
        fs::current_path(),        // path to store data
        folder_name,               // name of the folder in path, which will be newly created
        algname,                     // name of the algoritm 
        info,                   // additional info about the algorithm              
        store_positions            // where to store x positions in the data files 
    );
}

AdaptationManager* get_adaptation_manager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, int selection){
	switch(selection){
		case 0:
			return new FixedManager(argparser, problem, budget);
		case 1:
			return new RandomManager(argparser, problem, budget);
		default:
			std::cerr << "Operator manager " << selection << " out of range. Continuing using FixedManager." << std::endl;
			return new FixedManager(argparser, problem, budget);		
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
results single_problem(AdaptationManager* manager, unsigned int* budget, ioh::problem::RealSingleObjective* problem, Argparse* argparser) {
	double best_fitness = std::numeric_limits<double>::max();
	std::vector<double> best_location(problem->meta_data().n_variables, -1);
	unsigned int iterations = 0;
	unsigned int no_movement = 0;
	unsigned int previous_iteration_budget = *budget;
	Population* pop = manager->get_population();

	//TODO: do something neat for this
	bool log_pos  = false;
	bool first_it = true;
	// bool  = true;
	
	//TODO: budget
	while (*budget > 0) {
		if (log_pos && first_it){
			pop->write_population();
			first_it = false;
		}

		manager->adapt(iterations);		

		iterations++;
		// std::cout << "budget left: " << *budget << " iteration: " << iterations <<std::endl;
		//mutate
		pop->apply_mutation();
		std::cout << "|===";

		//crossover
		pop->apply_crossover();
		std::cout << "===";

		//selection
		pop->apply_selection();
		std::cout << "===";

		//sort population
		pop->sort();
		std::cout << "=| ";
		if (log_pos){
			pop->write_population();
		}
		

		//on fitness improvement
		if (pop->get_current_generation()[0]->get_fitness() < best_fitness)
		{
			std::cout << "new best is: " << pop->get_current_generation()[0]->get_fitness() << " at iteration " << iterations << std::endl;

			// // update vector pool using previous bestfitness
			// if (pop->get_mutation() == DIRMUT){
				pop->update_vector_pool(best_fitness);  //TODO: IF DIRMUT IS A POSSIBLE OPERATOR
			// }

			best_fitness = pop->get_current_generation()[0]->get_fitness(); //update best fitness
			best_location = pop->get_current_generation()[0]->get_position();
			
		} else {
			std::cout << std::endl;
		}
		//additional stopping criteria
		//...
		if (*budget != previous_iteration_budget){
			no_movement = 0;
			previous_iteration_budget = *budget;
		}else{
			no_movement++;
		}
		if (no_movement > 5){
			std::cerr << "Searching stuck. No movement for >5  iterations. shuffling population. " << std::endl;
			pop->randomise_population();
		}


		//optimum discovered, stop searching
		// if (best_fitness == problem->meta_data().optimum){
		// 	break;
		// }

		// std::cout << "==================" << std::endl;
	}
	std::cout << "==================" << std::endl;
	// std::cout << "best possible fitness: " << problem->meta_data.optimum << std::endl;
	std::cout << "best fitness found   : " << best_fitness << std::endl;
	std::cout << "==================" << std::endl;
	return return_value(best_location, best_fitness, iterations);

}





int main(int argc, char* argv[]) {
	auto argparser = new Argparse(argc, argv);
	
    std::cout << "Warning: dont use Desmu with reflection" << std::endl;

	tools.set_seed();	



	//TODO: finish command line params and remove below
	int function_num = std::stoi(argparser->get_values()["-f"]);
	unsigned int number_of_runs = std::stoi(argparser->get_values()["-runs"]);

	

	// int dim = std::stoi(argparser->get_values()["-d"]);

	int m = std::stoi(argparser->get_values()["-m"]);
	size_t archive_size = std::stoi(argparser->get_values()["-archive"]);
	unsigned int budget_value = std::stoi(argparser->get_values()["-budget"]);
	unsigned int* budget = &budget_value;

	auto logger = get_logger(argparser, "results/results-m"+argparser->get_values()["-m"]+"-d"+argparser->get_values()["-d"]
		+"-pop_size"+argparser->get_values()["-pop_size"]+"-F"+argparser->get_values()["-F"]+"-Cr"+argparser->get_values()["-Cr"]);
    /// Instatiate a bbob suite of problem {1,2}, instance {1, 2} and dimension {5,10}.
    // const auto &suite_factory = ioh::suite::SuiteRegistry<ioh::problem::RealSingleObjective>::instance();
    // const auto suite = suite_factory.create("BBOB", {1, 20}, {1, 2}, {dim});
    const auto suite = create_suite(1, 1, std::stoi(argparser->get_values()["-d"]) );

    /// Show output folder of the logger 
    std::cout << "Storing data at: " << logger.output_directory() << std::endl; 
    
    /// Attach logger to a suite. We can either attach a logger to a suite or for
    /// each new problem in a suite manually (e.g. call problem->attach_logger(logger)
    /// for each new problem), the functionality is the same. 
    suite->attach_logger(logger);

    /// To access problems of the suite.
    for (const auto &problem_shr : *suite){
    	auto problem = problem_shr.get();
    	tools.set_seed(problem->meta_data().instance);

    	//reset budget
    	unsigned int budget_value = std::stoi(argparser->get_values()["-budget"]);
    	unsigned int* budget = &budget_value;
    	std::cout << "setting budget to " << *budget << std::endl;

		// set up operators and problem variables
		int problem_dim = problem->meta_data().n_variables;
    	int pop_size = std::stoi(argparser->get_values()["-pop_size"]);//read population size		

		
		std::cout << "metadata" << problem->meta_data() << std::endl;

		AdaptationManager* manager = get_adaptation_manager(argparser, problem, budget, std::stoi(argparser->get_values()["-a"]));
		// Population* pop = new Population(crossover, selection, mutation, problem, boundary_correction, pop_size, budget, archive_size);


		// std::cout << "Run " << i << " ";
		

		results result = single_problem(manager, budget, problem, argparser);
		std::cout << "final result" << std::endl;
		

		//print some results
		manager->get_population()->print_fitness();
		std::cout << "--------------- history of best individual" << std::endl;
		manager->get_population()->get_current_generation()[0]->print_history();
		
		std::cout << "--------------- 2nd best result history" << std::endl;
		manager->get_population()->get_current_generation()[1]->print_history();
		std::cout << "=================" << std::endl;
		

		delete manager;
		manager=NULL;
	};

	delete argparser;
	argparser = NULL;
	return 0;
}
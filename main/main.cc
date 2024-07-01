#include "../include/tools.h"
#include "../include/adaptation.h"
// #include "ioh.h"
//#include "../include/functions.h"

#include <iostream> //general output

struct results {
	std::vector<double> best_location;
	double best_fitness;
	unsigned iterations;
};

inline std::shared_ptr<ioh::suite::Suite<ioh::problem::RealSingleObjective>> create_suite(int problem, int inst, int dim, const bool using_factory = true)
{
	// const std::vector<int> problems{21,22,23,24};
    // const std::vector<int> instances{1};
    const std::vector<int> problems{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    const std::vector<int> instances{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,
    								 11,12,13,14,15,16,17,18,19,20,
    								 21,22,23,24,25,26,27,28,29,30,
    								 31,32,33,34,35,36,37,38,39,40,
    								 41,42,43,44,45,46,47,48,49,50,
    								}; //always start from 1 when logging MAB Q_vals
    std::vector<int> dimensions;
    if (dim == 0){ dimensions = {5,20}; }
    else{ dimensions = {dim}; }

    if (using_factory)
        return ioh::suite::SuiteRegistry<ioh::problem::RealSingleObjective>::instance()
            .create("BBOB", problems, instances, dimensions);
    return std::make_shared<ioh::suite::BBOB>(problems, instances, dimensions);
}

inline ioh::logger::Analyzer get_logger(Argparse* argparser, const std::string &folder_name = "results", const bool store_positions = false) //false
{
	// Construct the name  and info field for the IOHprofiler JSONs
	std::string algname;
	if (std::stoi(argparser->get_values()["-a"]) == 0)
	{
		algname = MUTATION_NAMES[std::stoi(argparser->get_values()["-m"])] + "_";
		algname += 'F' + argparser->get_values()["-F"] + "_";
		algname += CROSSOVER_NAMES[std::stoi(argparser->get_values()["-c"])] + "_";
		algname += "Cr" + argparser->get_values()["-Cr"];
	} else if (std::stoi(argparser->get_values()["-a"]) == 1){
		algname = "random operators";
	} else if (std::stoi(argparser->get_values()["-a"]) == 2){
		algname = "ops" + std::bitset<NUM_MUTATION_OPERATORS>(std::stoi(argparser->get_values()["-ops"])).to_string();
		// algname = "MAB_lp" + argparser->get_values()["-lp"] + 
		// 			"eps" + argparser->get_values()["-eps_a"] + 
		// 			"sel" + argparser->get_values()["-MABsel"] +
		// 			"crd" + argparser->get_values()["-credit"];
	} else {
		algname = "-aERROR";
		algname += MUTATION_NAMES[std::stoi(argparser->get_values()["-m"])] + "_";
		algname += 'F' + argparser->get_values()["-F"] + "_";
		algname += CROSSOVER_NAMES[std::stoi(argparser->get_values()["-c"])] + "_";
		algname += "Cr" + argparser->get_values()["-Cr"];
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


// Instantiates a population which uses the specified adaptation strategy.
AdaptationManager* get_adaptation_manager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, int selection){
	switch(selection){
		case 0:
			return new FixedManager(argparser, problem, budget);
		case 1:
			return new RandomManager(argparser, problem, budget);
		case 2:
			return new MABManager(argparser, problem, budget);
		default:
			std::cerr << "Operator manager " << selection << " out of range. Continuing using FixedManager." << std::endl;
			return new FixedManager(argparser, problem, budget);		
	}	
}


// Consolidate the most important information of a run
results return_value(std::vector<double> location, double fitness, int i){
  results result;
  result.best_location = location;
  result.best_fitness = fitness;
  result.iterations = i;
  return result;
}


//main loop
results single_problem(AdaptationManager* manager, unsigned int* budget, ioh::problem::RealSingleObjective* problem, Argparse* argparser) {
	// Set up variables
	double best_fitness = std::numeric_limits<double>::max();
	double previous_best_fitness = std::numeric_limits<double>::max();
	std::vector<double> best_location(problem->meta_data().n_variables, -1);
	unsigned int iterations = 0;
	unsigned int no_movement = 0;
	unsigned int previous_iteration_budget = *budget;

	Population* pop = manager->get_population();
	

	// Generally not used because writes large amounts of data
	bool first_it = true;
	bool log_pos  = false;

	
	
	//TODO: budget
	while (*budget > 0) {
		if (log_pos && first_it){
			pop->write_population();
			first_it = false;
		}

		
		// std::cout << "budget left: " << *budget << " iteration: " << iterations <<std::endl;
		//mutate
		pop->apply_mutation();

		//crossover
		pop->apply_crossover();

		//selection
		pop->apply_selection();

		//sort population
		pop->sort();
		if (log_pos){
			pop->write_population();
		}
		
		
		// On fitness improvement
		if (pop->get_current_generation()[0]->get_fitness() < best_fitness)
		{
			// std::cout << "new best is: " << fmt::format("{}", pop->get_current_generation()[0]->get_fitness()) << " at iteration " << iterations << std::endl;

			// // update vector pool using previous bestfitness
			if (tools.extract_bit(std::stoi(argparser->get_values()["-ops"]), DIRMUT)){
				pop->update_vector_pool(best_fitness);  
			}
			previous_best_fitness = best_fitness;
			best_fitness = pop->get_current_generation()[0]->get_fitness(); //update best fitness
			best_location = pop->get_current_generation()[0]->get_position();
			
		}
		
		manager->adapt(previous_best_fitness);	
		iterations++;

		//additional stopping criteria and 
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
		if (best_fitness <= problem->optimum().y){
			std::cout << fmt::format("Optimum {} found at iteration {} with {} budget left", best_fitness, iterations, *budget) << std::endl;
			*budget = 0;
		}
	}
	if (!!std::stoi(argparser->get_values()["-logQ"])){ manager->log_Qs(); }
	std::cout << "==================" << std::endl;
	std::cout << "best possible fitness: " << problem->optimum().y << std::endl;
	std::cout << "best fitness found   : " << best_fitness << std::endl;
	std::cout << "==================" << std::endl;
	return return_value(best_location, best_fitness, iterations);

}





int main(int argc, char* argv[]) {
	auto argparser = new Argparse(argc, argv);	

	// Set up the path where logs are stored
	std::string adap_logger_folder = fmt::format("results/a{}m{}/", argparser->get_values()["-a"], argparser->get_values()["-m"]);
	std::string sub_logger_folder = "-ops" + std::bitset<NUM_MUTATION_OPERATORS>(std::stoi(argparser->get_values()["-ops"])).to_string();
	std::string logger_folder = adap_logger_folder + sub_logger_folder;

	auto logger = get_logger(argparser, logger_folder);

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

    	// Set the seed to current instance number for repeatable experiments
    	tools.set_seed(problem->meta_data().instance);

    	// set up operators and problem variables
		int problem_dim = problem->meta_data().n_variables;	

    	int budget_pop_multiplier;
    	if (std::stoi(argparser->get_values()["-pop_size"]) <= 4){
			budget_pop_multiplier = 5 * problem->meta_data().n_variables;
		}else{
			budget_pop_multiplier = std::stoi(argparser->get_values()["-pop_size"]);
		}

    	//reset budget
    	unsigned int budget_value = std::stoi(argparser->get_values()["-budget"]) * (std::stod(argparser->get_values()["-budget_multiplier"])) * problem_dim; 
    	
    	if (budget_value < std::stoi(argparser->get_values()["-budget"])){
    		budget_value = std::stoi(argparser->get_values()["-budget"]);
    	}
    	unsigned int* budget = &budget_value;


    	std::cout << "setting budget to " << *budget << std::endl;		
		std::cout << "metadata" << problem->meta_data() << std::endl;
		std::cout << "optimal y: " << problem->optimum().y << std::endl;

		// Construct population through AdaptationManager
		AdaptationManager* manager = get_adaptation_manager(argparser, problem, budget, std::stoi(argparser->get_values()["-a"]));
		
		// Run the DE on current problem
		results result = single_problem(manager, budget, problem, argparser);
		
		

		//print some results
		std::cout << "final result" << std::endl;
		manager->get_population()->print_fitness();
		// std::cout << "--------------- history of best individual" << std::endl;
		// manager->get_population()->get_current_generation()[0]->print_history();
		
		// std::cout << "--------------- 2nd best result history" << std::endl;
		// manager->get_population()->get_current_generation()[1]->print_history();
		std::cout << "=================" << std::endl;
		

		delete manager;
		manager=NULL;
	};

	delete argparser;
	argparser = NULL;
	return 0;
}
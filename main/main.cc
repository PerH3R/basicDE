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

Mutation* get_mutation_operator(Argparse* argparser, ioh::problem::RealSingleObjective* problem, Boundary* boundary_correction, 
									unsigned int* budget, int pop_size, int mut_op = -1, float F = -1.0){
	// int mut_op = std::stoi(argparser->get_values()["-m"]);
	size_t dim = problem->meta_data().n_variables;
	if (F < 0){
		F = std::stod(argparser->get_values()["-F"]);
	}
	size_t archive = std::stoi(argparser->get_values()["-archive"]);
	// if (mutator != -1){
	// 	mut_op = mutator;
	// }
	switch(mut_op){
		case 0:
			return new RandDiv1(dim, pop_size, F);
		case 1:
			return new BestDiv1(dim, pop_size, F);
		case 2:			
			return new TargetToPBestDiv1(dim, pop_size, F, archive);
		case 3:
			return new TargetToBestDiv2(dim, pop_size, F);
		case 4:
			return new TargetToRandDiv2(dim, pop_size, F);
		case 5:
			return new TwoOptDiv1(dim, pop_size, F);
		case 6:
			return new Desmu(dim, pop_size, F);
		case 7:
			return new Bea(dim, pop_size, boundary_correction, problem, budget, F);
		case 8:
			return new DirMut(dim, pop_size, F);
		case 9:
			return new RandomSearch(dim, pop_size, problem, F);
		default:
			std::cerr << "Mutation operator " << mut_op << " out of range. Continuing using RandDiv1." << std::endl;
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
results single_problem(Population* pop, unsigned int* budget, size_t dimension, 
		ioh::problem::RealSingleObjective* problem, Argparse* argparser, Boundary* boundary_correction) { //this row for random
	double best_fitness = std::numeric_limits<double>::max();
	//worst_fitness = std::numeric_limits<double>::max;
	std::vector<double> best_location(dimension, -1);
	unsigned int iterations = 0;
	unsigned int no_movement = 0;
	unsigned int previous_iteration_budget = *budget;

	//TODO: do something neat for this
	bool log_pos = false;
	bool first_it = true;
	
	//TODO: budget
	while (*budget > 0) {
		if (log_pos && first_it){
			pop->write_population();
			first_it = false;
		}

		//full random for baselines
		if (std::stoi(argparser->get_values()["-m"]) == 99){
			for (int i = 0; i < pop->get_population_size(); ++i){
				int new_m = tools.rand_int_unif(0,9); //exclude random search
				if (new_m == 6 && tools.rand_double_unif(0,1) > 1/(2*5)){ //BEA uses many evaluations so reroll based on Nclones and Nsegments (default values)
					int new_m = tools.rand_int_unif(0,10);
				}
				double new_F = tools.rand_double_unif(0.2,0.8);
				pop->set_individual_mutation( get_mutation_operator(argparser, problem, boundary_correction, budget, pop->get_population_size(), new_m, new_F), i);
			}
		}

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
			// std::cout << "==================" << std::endl;
			// std::cout << "budget left: " << *budget << " iteration: " << iterations <<std::endl;
			// pop->print_fitness();
			// std::cout << "==================" << std::endl;

			// // update vector pool using previous bestfitness
			// if (pop->get_mutation() == DIRMUT){
				pop->update_vector_pool(best_fitness);  /////TODO: IF DIRMUT IS A POSSIBLE OPERATOR
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
			// return return_value(best_location, best_fitness, iterations);
		}

		//TODO: optimum reached (value can't be retrieved from ioh?)
		// if (best_fitness == ){
		// 	/* code */
		// }

		// std::cout << "==================" << std::endl;
	}
	std::cout << "==================" << std::endl;
	std::cout << "best fitness found: " << best_fitness << std::endl;
	std::cout << "==================" << std::endl;
	return return_value(best_location, best_fitness, iterations);

}





int main(int argc, char* argv[]) {
	//check arguments
	int num_args = 1; //TODO: remove

	auto argparser = new Argparse(argc, argv);

	if (argc != num_args) {
		std::cerr << "expected " << num_args << " arguments, got " << argc << " " << argv <<"." <<std::endl;
	};

    std::cout << "Warning: dont use Desmu with reflection" << std::endl;

	tools.set_seed();

	//TODO: convert argv to correct type



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

		//TODO: problem selector

		// set up operators and problem variables
		int problem_dim = problem->meta_data().n_variables;
    	int pop_size = std::stoi(argparser->get_values()["-pop_size"]);//set population size
    	if (pop_size < 4){
    		pop_size = 4 + 5*problem_dim;
    		std::cout << "automatic population size of: " << pop_size << std::endl;
    	}//automatic pop_size if not specified or too small
    	
		// auto problem = get_problem(2, i, dim);		
		Boundary* boundary_correction = new Reinit(problem);
		Mutation* mutation = get_mutation_operator(argparser, problem, boundary_correction, budget, pop_size, m); //new TargetToPBestDiv1(dim, pop_size);
		Crossover* crossover = new Binomial(problem_dim, std::stod(argparser->get_values()["-Cr"]));
		Selection* selection = new Elitist(pop_size);
		

		
		std::cout << "metadata" << problem->meta_data() << std::endl;
		std::cout << "optimizing using operator " << mutation->get_type()+1 << std::endl;

		Population* pop = new Population(crossover, selection, mutation, problem, boundary_correction, pop_size, budget, archive_size);

		// std::cout << "Run " << i << " ";
		

		results result = single_problem(pop, budget, problem_dim, problem, argparser, boundary_correction);
		std::cout << "final result" << std::endl;
		pop->print_fitness();
		std::cout << "--------------- best result history" << std::endl;
		auto best_history = pop->get_current_generation()[0]->get_history();
		for (auto snapshot : best_history){
			std::string hist_string= "";
			const auto [top_pos, top_fitness, top_CrOpPtr, top_MutOpPtr, top_BoundOpPtr] = snapshot;
			
			//print position
			// for(double i : top_pos){
			// 	std::cout << i <<',';
			// }

			//print outher information
			std::cout << " " << top_fitness << " " << top_MutOpPtr->get_F() << " " << top_CrOpPtr->get_Cr() << " " 
						<< CROSSOVER_NAMES[top_CrOpPtr->get_type()] << " " << MUTATION_NAMES[top_MutOpPtr->get_type()] << " " << BOUNDARY_NAMES[top_BoundOpPtr->get_type()] << std::endl;
			
			std::cout << hist_string;
		}
		std::cout << "--------------- 2nd best result history" << std::endl;
		best_history = pop->get_current_generation()[1]->get_history();
		for (auto snapshot : best_history){
			std::string hist_string= "";
			const auto [top_pos, top_fitness, top_CrOpPtr, top_MutOpPtr, top_BoundOpPtr] = snapshot;
			
			//print position
			// for(double i : top_pos){
			// 	std::cout << i <<',';
			// }

			//print outher information
			std::cout << " " << top_fitness << " " << top_MutOpPtr->get_F() << " " << top_CrOpPtr->get_Cr() << " " 
						<< CROSSOVER_NAMES[top_CrOpPtr->get_type()] << " " << MUTATION_NAMES[top_MutOpPtr->get_type()] << " " << BOUNDARY_NAMES[top_BoundOpPtr->get_type()] << std::endl;
			
			std::cout << hist_string;
		}
		std::cout << "=================" << std::endl;
		

		delete pop;
		pop=NULL;
		// delete problem;
		// problem=NULL;

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
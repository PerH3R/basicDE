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

inline std::shared_ptr<ioh::suite::Suite<ioh::problem::RealSingleObjective>> create_suite(int problem, int inst, int dim, const bool using_factory = true)
{
    const std::vector<int> problems{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    const std::vector<int> instances{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    const std::vector<int> dimensions{5,20};

    if (using_factory)
        return ioh::suite::SuiteRegistry<ioh::problem::RealSingleObjective>::instance()
            .create("BBOB", problems, instances, dimensions);
    return std::make_shared<ioh::suite::BBOB>(problems, instances, dimensions);
}

inline ioh::logger::Analyzer get_logger(const std::string &folder_name = "results", const bool store_positions = false)
{
    /// Instantiate a logger.
    using namespace ioh;
    return logger::Analyzer(
        {trigger::on_improvement}, // trigger when the objective value improves
        {},                        // no additional properties 
        fs::current_path(),        // path to store data
        folder_name,               // name of the folder in path, which will be newly created
        "PSO",                     // name of the algoritm 
        "Type1",                   // additional info about the algorithm              
        store_positions            // where to store x positions in the data files 
    );
}

Mutation* get_mutation_operator(Argparse* argparser, ioh::problem::RealSingleObjective* problem, Boundary* boundary_correction, 
									unsigned int* budget){
	int mut_op = std::stoi(argparser->get_values()["-m"]);
	size_t pop_size = std::stoi(argparser->get_values()["-pop_size"]);
	size_t dim = problem->meta_data().n_variables;
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
		case 9:
			return new DirMut(dim, pop_size, F);
		default:
			std::cerr << "Mutation operator out of range. Continuing using RandDiv1." << std::endl;
			return new RandDiv1(dim, pop_size, F);		
	}	
}

ioh::problem::RealSingleObjective* get_problem(int problem, int i, size_t dim){
	switch(problem){
		case 1:
			return new ioh::problem::bbob::Sphere(i, dim);
		case 2:
			return new ioh::problem::bbob::Schwefel(i, dim);
		default:
			return new ioh::problem::bbob::Sphere(i, dim);
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
	unsigned int iterations = 0;
	unsigned int no_movement = 0;
	unsigned int previous_iteration_budget = *budget;
	
	//TODO: budget
	while (*budget > 0) {
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
			if (pop->get_mutation() == DIRMUT)			{
				pop->update_vector_pool(best_fitness);
			}
			best_fitness = pop->get_current_generation()[0]->get_fitness();
			best_location = pop->get_current_generation()[0]->get_position();
			
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
			std::cerr << "No movement for >5  iterations. Aborting run. " << std::endl;
			return return_value(best_location, best_fitness, iterations);
		}

		//TODO: optimum reached (value can't be retrieved from ioh?)
		// if (best_fitness == ){
		// 	/* code */
		// }

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

	// int dim = std::stoi(argparser->get_values()["-d"]);

	int m = std::stoi(argparser->get_values()["-m"]);

	size_t archive_size = std::stoi(argparser->get_values()["-archive"]);

	unsigned int budget_value = std::stoi(argparser->get_values()["-budget"]);

	unsigned int* budget = &budget_value;

	//TODO: run logs for IOH

	//main loop
	// for (size_t i = 0; i < number_of_runs; i++) {	

	auto logger = get_logger();
    /// Instatiate a bbob suite of problem {1,2}, instance {1, 2} and dimension {5,10}.
    // const auto &suite_factory = ioh::suite::SuiteRegistry<ioh::problem::RealSingleObjective>::instance();
    // const auto suite = suite_factory.create("BBOB", {1, 20}, {1, 2}, {dim});
    const auto suite = create_suite(1, 1, 1);


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
		// auto problem = get_problem(2, i, dim);		
		Boundary* boundary_correction = new Clamp(problem);
		Mutation* mutation = get_mutation_operator(argparser, problem, boundary_correction, budget); //new TargetToPBestDiv1(dim, pop_size);
		Crossover* crossover = new Binomial(problem_dim);
		Selection* selection = new Elitist(pop_size);
		

		
		std::cout << "metadata" << problem->meta_data() << std::endl;
		std::cout << "optimizing using " << mutation->get_type() << std::endl;

		Population* pop = new Population(crossover, selection, mutation, problem, boundary_correction, pop_size, budget, archive_size);

		// std::cout << "Run " << i << " ";
		

		results result = single_problem(pop, budget, problem_dim);
		std::cout << "final result" << std::endl;
		pop->print_fitness();
		std::cout << std::endl;
		

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
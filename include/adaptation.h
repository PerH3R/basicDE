#pragma once

#include "population.h"
#include "credit.h"
#include <fstream>
/*
0 - no adaptation, runs according to all passed parameters
1 - random operator switching, uses all operators besides random search and their best found F & Cr. IGNORES: -m -F -Cr
2 - Multi-Armed Bandit ...
*/

class AdaptationManager{
public:
	AdaptationManager() = default;
	AdaptationManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
	virtual ~AdaptationManager();
	virtual void adapt(const double& previous_best_fitness) = 0;
	Population* get_population(){return this->pop;};
	virtual void log_Qs() {}; 
	virtual void Qlog_init() {}; 
	std::shared_ptr<Credit> get_credit_operator(int crd_op = 0);


protected:
	// Holds values related to operator adaptation for a given operator combination
	struct operator_configuration{
		std::shared_ptr<Mutation> mutation_operator;
		std::shared_ptr<Crossover> crossover_operator;
		std::vector<double> Q; //weighted Q scores over time
		std::vector<double> scores; //average 'raw' score based on credit per learning period
		std::vector<double> lp_improvements; //'raw' score based on credit this learning period from all agent with this config
		std::vector<unsigned int> Qbudget; //budget at moment of logging Q
	};

	// Contains the above structure for all operator combinations currently considered
	std::vector<operator_configuration> operator_configurations;
	
	// Returns an automatic population size if no (valid) population size was specified
	unsigned int calc_population_size(const Argparse* argparser) const{
		if (std::stoi(argparser->get_values()["-pop_size"]) <= 4){
			return 5 * problem->meta_data().n_variables;
		}//automatic population size if too small or not specified
		return std::stoi(argparser->get_values()["-pop_size"]);
	};
	virtual void create_population() = 0;

	// Checks if an Agent in the population uses the specified configuration
	bool agent_has_config(Agent* a, const operator_configuration& o);

	// Access to command line arguments
	const Argparse* argparser;

	// Manages how improvement is measured for MAB
	std::shared_ptr<Credit> credit_assigner;

	// pointer to population class
	Population* pop;

	// Fallback operators
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;

	// How often an agent is allowed to retry generating a donor vector if it generates oob.
	// default value = 10+ln(dim)^2;
	int resample_limit;

	ioh::problem::RealSingleObjective* problem;
	unsigned int* budget;
	
	int lp; //learning period, how many iterations between the application of new operators

	// population size
	const size_t n;

	//dimensionality of current problem
	const size_t dim;


	float Cr;
	float F;
	size_t archive_size;
	bool available_mutops[NUM_MUTATION_OPERATORS];
	unsigned int iteration_counter = 0;
};


// No adaptation
class FixedManager : public AdaptationManager{
public:
	FixedManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
    ~FixedManager(){};
    void adapt(const double& previous_best_fitness){
    	(void)previous_best_fitness; //silence warning
    };	//no adaptation
protected:	
	void create_population();
};

// Randomizes the mutation operator for each individual for each generation
class RandomManager : public AdaptationManager{
public:
	RandomManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF=false);
    ~RandomManager(){};
    void adapt(const double& previous_best_fitness);
protected:
	void create_population();

	bool RandomizeF;
};

// Dynamically switches operators using the Multi-Armed-Bandit technique a given Credit fuction
class MABManager : public AdaptationManager{
public:
	MABManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
	~MABManager(){};

	// Adapt by updating q-values
    void adapt(const double& previous_best_fitness);

    // fuctions for logging Q-values (these are NOT the IOH logs)	
	void log_Qs() override;
	void Qlog_init() override; 
    
protected:
	// Update Q-values for adaptation step
	void update_scores(const double& previous_best_fitness);

	// initialize population
	void create_population();

	// Check if a configuration with certain values already exists in the operator_configurations
	int config_in_configs(operator_configuration new_config);

	// Generates a config from current settings of an agent add adds it to operator_configurations
	void add_config_from_agent(Agent* a);

	// Selects a configuration according to eps_a and MABsel
	operator_configuration get_new_config();
	// Set operators of a configuration on an Agent
	void set_config_on_agent(operator_configuration new_config, int a_idx);


	// Amount of decay applied to results previous iteration
	double alpha = 0.5;
	// Chance to create new operator_configuration entry. Use to, for example, implement SaDE like-behaviour (logging wont work correctly)
	// Not fully implemented and likely causes errors if set to >0
	double new_config_chance = 0.0; 
	// Should the MAB select greedily or proportionately from operator_configurations, given current Q-values 
	int MABsel; 
	// Chance to choose an operator at random instead of greedy or proportionate strategy					
	double eps_a; 
	// Sum of all current Q-values for proportionate selections			
	double total_Q = 0.0;
	// Log Q-values over time			
	bool logging = false;	
	// Where to save logs (these are NOT the IOH logs)		
	std::string Qlogger_location;	
};
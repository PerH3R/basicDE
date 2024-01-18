#pragma once

#include "population.h"
#include "credit.h"
/*

*/

class AdaptationManager{
public:
	AdaptationManager() = default;
	AdaptationManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
	virtual ~AdaptationManager();
	virtual void adapt(unsigned int iterations) = 0;
	Population* get_population(){return this->pop;};


protected:
	struct operator_configuration{
		MUTATION mutation_type;
		float F;
		CROSSOVER crossover_type;
		float Cr;
		std::vector<double> Q; //weighted Q scores over time
		std::vector<double> scores; //average 'raw' score based on credit per learning period
		std::vector<double> lp_improvements; //'raw' score based on credit this learning period from all agent with this config
	};

	std::vector<operator_configuration> operator_configurations;
	
	unsigned int calc_population_size(const Argparse* argparser) const{
		if (std::stoi(argparser->get_values()["-pop_size"]) <= 4){
			return 5 * problem->meta_data().n_variables;
		}//automatic population size if too small or not specified
		return std::stoi(argparser->get_values()["-pop_size"]);
	};
	virtual void create_population() = 0;

	bool agent_has_config(Agent* a, const operator_configuration& o);


	std::vector<float> F_history;
	std::vector<float> Cr_history;

	const Argparse* argparser;
	Credit* credit_assigner;
	Population* pop;
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;
	int resample_limit;
	unsigned int* budget;
	ioh::problem::RealSingleObjective* problem;
	int lp; //learning period, how many iterations between the application of new operators

	const size_t n;
	const size_t dim;
	float Cr;
	float F;
	size_t archive_size;
	bool available_mutops[NUM_MUTATION_OPERATORS];
	int iteration_counter = 0;
	int mab_alpha = 0.5;
};


//no adaptation
class FixedManager : public AdaptationManager{
public:
	FixedManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
    ~FixedManager(){};
    void adapt(unsigned int iterations){};	//no adaptation
protected:	
	void create_population();
};

//Randomizes the mutation operator for each individual for each generation
class RandomManager : public AdaptationManager{
public:
	RandomManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF=false);
    ~RandomManager(){};
    void adapt(unsigned int iterations);
protected:
	void create_population();

	bool RandomizeF;
};

class MABManager : public AdaptationManager{
public:
	MABManager(const Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
	~MABManager(){};
    void adapt(unsigned int iterations);	//no adaptation
    void update_scores();
protected:	
	void create_population();
	int config_in_configs(operator_configuration new_config);
	void add_config_from_agent(Agent* a);
	operator_configuration get_new_config();
	void set_config_on_agent(operator_configuration new_config, int a_idx);
	double alpha = 0.5;
	double random_config_epsilon = 0.1;
	double total_Q = 0.0;
};
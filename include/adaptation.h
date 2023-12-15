#pragma once

#include "../include/population.h"
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
	struct operator_combination{
		MUTATION mutation_operator;
		float F;
		CROSSOVER crossover_operator;
		float Cr;
		float score;
	};

	std::vector<operator_combination> scores;
	
	unsigned int calc_population_size(const Argparse* argparser) const{
		if (std::stoi(argparser->get_values()["-pop_size"]) <= 4){
			return 5 * problem->meta_data().n_variables;
		}//automatic population size if too small or not specified
		return std::stoi(argparser->get_values()["-pop_size"]);
	};
	virtual void create_population() = 0;


	std::vector<float> F_history;
	std::vector<float> Cr_history;

	const Argparse* argparser;
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
protected:	
	void create_population();
};
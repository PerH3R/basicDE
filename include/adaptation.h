#pragma once

#include "../include/population.h"
/*

*/

class AdaptationManager{
public:
	AdaptationManager() = default;
	AdaptationManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
	~AdaptationManager();
	virtual void adapt(unsigned int iterations) = 0;
	Population* get_population(){return this->pop;};
	// Mutation* get_mutation_operator(int mut_op = -1, float F = -1.0);
	// Selection* get_selection_operator(int sel_op = -1);
	// Crossover* get_crossover_operator(int c_op = -1, float Cr = -1.0);
	// Boundary* get_boundary_operator(int bound_op = -1);
protected:
	

	virtual Population* create_population() = 0;


	std::vector<float> F_history;
	std::vector<float> Cr_history;

	Argparse* argparser;
	Population* pop;
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;
	unsigned int* budget;
	ioh::problem::RealSingleObjective* problem;

	size_t n;
	size_t dim;
	float Cr;
	float F;
	size_t archive_size;
};


//no adaptation
class FixedManager : public AdaptationManager{
public:
	FixedManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget);
    // ~FixedManager(){};
    void adapt(unsigned int iterations){};	
protected:	
	Population* create_population();
};

//Randomizes the mutation operator for each individual for each generation
class RandomManager : public AdaptationManager{
public:
	RandomManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF = false);
    // ~RandomManager(){};
    void adapt(unsigned int iterations);
protected:
	Population* create_population();

	bool RandomizeF;
};
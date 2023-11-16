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

protected:
	

	virtual void create_population() = 0;


	std::vector<float> F_history;
	std::vector<float> Cr_history;

	Argparse* argparser;
	Population* pop;
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;
	int resample_limit;
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
    void adapt(unsigned int iterations){};	//no adaptation
protected:	
	void create_population();
};

//Randomizes the mutation operator for each individual for each generation
class RandomManager : public AdaptationManager{
public:
	RandomManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget, bool RandomizeF = false);
    // ~RandomManager(){};
    void adapt(unsigned int iterations);
protected:
	void create_population();

	bool RandomizeF;
};
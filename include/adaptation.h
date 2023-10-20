#pragma once

#include "../include/argparse.h"
#include "../include/population.h"
/*

*/

class AdaptationManager{
public:
	AdaptationManager() = default;
	AdaptationManager(Argparse* argparser, ioh::problem::RealSingleObjective* problem, unsigned int* budget) : 
						argparser(argparser), problem(problem), budget(budget){
		this->F = std::stod(argparser->get_values()["-F"]);
		this->Cr = std::stod(argparser->get_values()["-Cr"]);
		this->F = std::stod(argparser->get_values()["-F"]);
		this->dim = problem->meta_data().n_variables;
		this->selection = get_selection_operator(std::stoi(argparser->get_values()["-s"]));
		this->crossover = get_crossover_operator(std::stoi(argparser->get_values()["-c"]));
		this->boundary = get_boundary_operator(std::stoi(argparser->get_values()["-b"]));
		this->mutation = get_mutation_operator();

		create_population(std::stoi(argparser->get_values()["-pop_size"]););
		this->n = pop->get_population_size();
	};
	virtual ~AdaptationManager() { };
	virtual void apply();
	Population* get_population(return pop;);
protected:
	Mutation* get_mutation_operator(int mut_op = -1, float F = -1.0);
	Selection* get_selection_operator(int sel_op = -1);
	Crossover* get_crossover_operator(int c_op = -1, float Cr = -1.0);
	Boundary* get_boundary_operator(int bound_op = -1);


	std::vector<float> F_history;
	std::vector<float> Cr_history;

	Argparse* argparser;
	Population* pop;
	Boundary* boundary;
	Crossover* crossover;
	Selection* selection;
	unsigned int* budget;
	ioh::problem::RealSingleObjective* problem;

	size_t n;
	size_t dim;
	float Cr;
	float F;
	size_t archive_size;
}


//no adaptation
class FixedManager : public AdaptationManager{
public:
	FixedManager(size_t dim, size_t n) : AdaptationManager(dim, n, F) {};
    ~FixedManager() {};
    void apply(){};	
protected:	
	Population* create_population(this->pop = new Population((this->crossover, this->selection, this->mutation, this->problem, this->boundary, 
		this->n, this->budget, this->archive_size)));
}

//Randomizes the mutation operator for each individual for each generation
class RandomManager : public AdaptationManager{
public:
	RandomManager(size_t dim, size_t n) : AdaptationManager(dim, n, F) {};
    ~RandomManager() {};
    void apply();
protected:
	Population* create_population(){
		this->pop =  new Population(this->crossover, this->selection, this->mutation, this->problem, this->boundary, 
			this->n, this->budget, this->archive_size)
		for (int i = 0; i < this->pop->get_population_size(); ++i){
			int new_m = tools.rand_int_unif(0, 15);
			this->pop->get_current_generation[i]->set_mutation(get_mutation_operator(new_m, this->F));
			this->pop->get_next_generation[i]->set_mutation(get_mutation_operator(new_m, this->F));
		}};
	bool const RandomizeF;
}
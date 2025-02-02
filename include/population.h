#pragma once

#include "../include/argparse.h"
#include "tools.h"
// #include "agent.h"
// #include "boundary.h" //via agent
#include "selection.h"
// #include "crossover.h"
// #include "mutation.h"
// #include "functions.h"
#include <string>
#include <iostream>

class Population {
public:
	Population(const Argparse* argparser, ioh::problem::RealSingleObjective* target_function, const size_t n, unsigned int* budget, int archive_size = 0,
		int resample_limit = -1);
	~Population();

	

	// applies the set operators to the current generation (selection also modifies next_gen of course)
	void apply_mutation();
	void apply_crossover();
	void apply_boundary_correction();
	void apply_selection();

	// prints all fitness values of Agents in cur_gen to terminal
	void print_fitness();

	// writes positions of population to file
	void write_population(std::string filename = "");
	
	// sorts cur_gen and next_gen based on fitness of cur_gen Agents, lower fitness = lower index
	void sort();

	// randomises the position vectors of all Agents
	void randomise_population();

	// self explanatory
	size_t get_population_size();	
	std::vector<Agent*> get_current_generation();
	std::vector<Agent*> get_next_generation();	

	// functions that create operator pointers to operators
	std::shared_ptr<Mutation> create_mutation_operator(int mut_op = -1, float F = 0.0);
	std::shared_ptr<Selection> create_selection_operator(int sel_op = -1);
	std::shared_ptr<Crossover> create_crossover_operator(int c_op = -1, float Cr = 0.0);
	std::shared_ptr<Boundary> create_boundary_operator(int bound_op = -1);

	// get current selection operator used for entire population
	SELECTION get_selection(){return this->selection_operator->get_type();};

	// get operator of an individual agent (pointer to this operator can be obtained through the Agent itself)
	CROSSOVER get_individual_crossover(int idx){return this->cur_gen[idx]->get_crossover();};
	MUTATION get_individual_mutation(int idx){return this->cur_gen[idx]->get_mutation();};
	BOUNDARY get_individual_boundary(int idx){return this->cur_gen[idx]->get_boundary();};

	//change selection operator
	void set_selection(std::shared_ptr<Selection> new_selection){this->selection_operator = new_selection;};

	//changes operator of a one or all Agents from the current population (idx=-1 to change all, idx between 0 to n to change specific individual)
	void set_individual_crossover(std::shared_ptr<Crossover> new_crossover, int idx = -1);
	void set_individual_mutation(std::shared_ptr<Mutation> new_mutation, int idx = -1);
	void set_individual_boundary(std::shared_ptr<Boundary> new_boundary, int idx = -1);

	// populates the vector pool with diference vectors
	void update_vector_pool(double previous_best_fitness); //DirMut specific

	

private:
	void add_to_archive(std::vector<bool> kept_indexes);
	void repopulate_next_gen();
	Agent* create_agent(int id);
	bool agent_in_use(Agent* target);
	bool improved = false;
	
	const Argparse* argparser;
	ioh::problem::RealSingleObjective* target_function;
	const size_t n; //population size	
	unsigned int* budget;
	size_t dim; //dimension size
	float F;
	float Cr;
	size_t archive_size;
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;
	int resample_limit;

	std::vector<Agent*> cur_gen;
	std::vector<Agent*> next_gen;

	std::vector<
		std::vector<double>
	> archive;

	int base_crossover_operator;
	int base_mutation_operator;
	int base_boundary_correction;
	std::shared_ptr<Selection> selection_operator;

	//for DirMut
	std::vector< std::vector<double> > vector_pool; //pool of difference vectors of improved agents	

	void next_to_current();
};
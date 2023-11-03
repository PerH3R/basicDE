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
	Population(Argparse* argparser, ioh::problem::RealSingleObjective* target_function, size_t pop_size, unsigned int* budget, int archive_size = 0);
	~Population();

	std::vector<Agent*> get_current_generation();
	std::vector<Agent*> get_next_generation();

	//void current_to_next(std::vector<Agent*>);

	void apply_mutation();
	void apply_crossover();
	void apply_boundary_correction();
	void apply_selection();

	void print_fitness();

	std::shared_ptr<Mutation> get_mutation_operator(int mut_op = -1, float F = -1.0);
	std::shared_ptr<Selection> get_selection_operator(int sel_op = -1);
	std::shared_ptr<Crossover> get_crossover_operator(int c_op = -1, float Cr = -1.0);
	Boundary* get_boundary_operator(int bound_op = -1);

	//writes positions of population to file
	void write_population(std::string filename = "");

	size_t get_population_size();	
	
	void sort();

	//if the population gets stuck
	void randomise_population();


	SELECTION get_selection(){return this->selection_operator->get_type();};

	// CROSSOVER get_base_crossover(){return this->base_crossover_operator->get_type();};
	// MUTATION get_base_mutation(){return this->base_mutation_operator->get_type();};
	// BOUNDARY get_base_boundary(){return this->base_boundary_correction->get_type();};

	CROSSOVER get_individual_crossover(int idx){return this->cur_gen[idx]->get_crossover();};
	MUTATION get_individual_mutation(int idx){return this->cur_gen[idx]->get_mutation();};
	BOUNDARY get_individual_boundary(int idx){return this->cur_gen[idx]->get_boundary();};


	//change selection operator
	void set_selection(std::shared_ptr<Selection> new_selection){this->selection_operator = new_selection;};

	//change the operator that's passed when creating a new Agent
	// void set_base_crossover(Crossover* new_crossover){this->base_crossover_operator = new_crossover;};
	// void set_base_mutation(Mutation* new_mutation){this->base_mutation_operator = new_mutation;};
	// void set_base_boundary(Boundary* new_boundary){this->base_boundary_correction = new_boundary;};



	//changes operator of a one or all Agents from the current population (idx=-1 to change all, idx between 0 to n to change specific individual)
	void set_individual_crossover(std::shared_ptr<Crossover> new_crossover, int idx = -1);
	void set_individual_mutation(std::shared_ptr<Mutation> new_mutation, int idx = -1);
	void set_individual_boundary(Boundary* new_boundary, int idx = -1);

	void update_vector_pool(double previous_best_fitness); //DirMut specific

private:
	void add_to_archive();
	void repopulate_next_gen();
	Agent* create_agent();
	bool agent_in_use(Agent* target);
	bool improved = false;
	

	size_t n; //population size
	size_t dim; //dimension size
	float F;
	float Cr;
	size_t archive_size;
	int base_boundary;
	int base_crossover;
	int base_selection;
	int base_mutation;
	unsigned int* budget;

	std::vector<Agent*> cur_gen;
	std::vector<Agent*> next_gen;

	std::vector<std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, Boundary*>> archive;

	int base_crossover_operator;
	int base_mutation_operator;
	int base_boundary_correction;
	std::shared_ptr<Selection> selection_operator;
	Argparse* argparser;
	ioh::problem::RealSingleObjective* target_function;
	// unsigned int* budget; 

	//for DirMut
	std::vector< std::vector<double> > vector_pool; //pool of difference vectors of improved agents	

	void next_to_current();
};
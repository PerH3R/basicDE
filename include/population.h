#pragma once

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
	Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::RealSingleObjective* target_function, 
				Boundary* boundary_correction, size_t pop_size, unsigned int* budget, int archive_size);
	~Population();

	std::vector<Agent*> get_current_generation();
	std::vector<Agent*> get_next_generation();

	//void current_to_next(std::vector<Agent*>);

	void apply_mutation();
	void apply_crossover();
	void apply_boundary_correction();
	void apply_selection();

	void print_fitness();

	//writes positions of population to file
	void write_population(std::string filename = "");

	size_t get_population_size();	
	
	void sort();

	//if the population gets stuck
	void randomise_population();


	SELECTION get_selection(){return this->selection_operator->get_type();};

	CROSSOVER get_base_crossover(){return this->crossover_operator->get_type();};
	MUTATION get_base_mutation(){return this->mutation_operator->get_type();};
	BOUNDARY get_base_boundary(){return this->boundary_correction->get_type();};

	CROSSOVER get_individual_crossover(int idx){return this->cur_gen[idx]->get_crossover();};
	MUTATION get_individual_mutation(int idx){return this->cur_gen[idx]->get_mutation();};
	BOUNDARY get_individual_boundary(int idx){return this->cur_gen[idx]->get_boundary();};


	//change selection operator
	void set_selection(Selection* new_selection){this->selection_operator = new_selection;};

	//change the operator that's passed when creating a new Agent
	void set_base_crossover(Crossover* new_crossover){this->crossover_operator = new_crossover;};
	void set_base_mutation(Mutation* new_mutation){this->mutation_operator = new_mutation;};
	void set_base_boundary(Boundary* new_boundary){this->boundary_correction = new_boundary;};



	//changes operator of a one or all Agents from the current population (idx=-1 to change all, idx between 0 to n to change specific individual)
	void set_individual_crossover(Crossover* new_crossover, int idx = -1);
	void set_individual_mutation(Mutation* new_mutation, int idx = -1);
	void set_individual_boundary(Boundary* new_boundary, int idx = -1);

	void update_vector_pool(double best_fitness){mutation_operator->update_vector_pool(best_fitness, cur_gen, next_gen);}; //DirMut specific

private:
	void add_to_archive();
	void repopulate_next_gen();
	Agent* create_agent();
	bool agent_in_use(Agent* target);
	

	size_t n; //population size
	size_t dim; //dimension size
	size_t archive_size;
	std::vector<Agent*> cur_gen;
	std::vector<Agent*> next_gen;

	std::vector<Agent*> archive;

	Crossover* crossover_operator;
	Selection* selection_operator;
	Mutation* mutation_operator;
	Boundary* boundary_correction;
	ioh::problem::RealSingleObjective* target_function;
	unsigned int* budget; 

	void next_to_current();
};
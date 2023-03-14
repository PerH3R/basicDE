#pragma once

#include "tools.h"
// #include "agent.h"
// #include "boundary.h" //via agent
#include "selection.h"
// #include "crossover.h"
// #include "mutation.h"
// #include "functions.h"

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

	size_t get_population_size();	
	
	void sort();
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
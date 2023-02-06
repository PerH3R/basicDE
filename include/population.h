#pragma once

#include "tools.h"
// #include "agent.h"
#include "boundary.h"
#include "selection.h"
// #include "crossover.h"
// #include "mutation.h"
// #include "functions.h"

class Population {
public:
	Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::bbob::Sphere* target_function, 
				Boundary* boundary_correction, size_t size, unsigned int* budget);
	~Population();

	std::vector<Agent*> get_current_generation();
	std::vector<Agent*> get_next_generation();

	void current_to_next(std::vector<Agent*>);

	void apply_mutation();
	void apply_crossover();
	void apply_boundary_correction();
	void apply_selection();

	void print_fitness();

	size_t get_population_size();



	
	
	void sort();
private:
	size_t n; //population size
	size_t dim; //dimension size
	std::vector<Agent*> cur_gen;
	std::vector<Agent*> next_gen;

	Crossover* crossover_operator;
	Selection* selection_operator;
	Mutation* mutation_operator;
	Boundary* boundary_correction;
	ioh::problem::bbob::Sphere* target_function;
	unsigned int* budget; 

	void next_to_current();
};
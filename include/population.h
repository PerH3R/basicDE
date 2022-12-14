#pragma once

#include "tools.h"
#include "agent.h"
#include "selection.h"
#include "crossover.h"

class Population {
public:
	Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, size_t size, size_t dim);
	~Population();

	std::vector<Agent*> get_current_generation();
	std::vector<Agent*> get_next_generation();

	void current_to_next(std::vector<Agent*>);

	void apply_mutation();
	void apply_crossover();
	void apply_selection();



	
	
	void sort();
private:
	size_t n; //population size
	size_t d; //dimension size
	std::vector<Agent*> cur_gen;
	std::vector<Agent*> next_gen;

	Crossover* crossover_operator;
	Selection* selection_operator;
	Mutation* mutation_operator;

	void next_to_current();
};
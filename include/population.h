#pragma once

#include "tools.h"
//#include "agent.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "functions.h"

class Population {
public:
	Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, Function* target_function, size_t size, size_t dim);
	~Population();

	std::vector<std::shared_ptr<Agent>> get_current_generation();
	std::vector<std::shared_ptr<Agent>> get_next_generation();

	void current_to_next(std::vector<std::shared_ptr<Agent> >);

	void apply_mutation();
	void apply_crossover();
	void apply_selection();



	
	
	void sort();
private:
	size_t n; //population size
	size_t d; //dimension size
	std::vector<std::shared_ptr<Agent> > cur_gen;
	std::vector<std::shared_ptr<Agent> > next_gen;

	Crossover* crossover_operator;
	Selection* selection_operator;
	Mutation* mutation_operator;
	Function* target_function;

	void next_to_current();
};
#pragma once

#include "tools.h"
#include "mutation.h"
#include "functions.h"

class Agent {
public:
	Agent(size_t dimension, Mutation* mutation_operator, Function* fitness_function);
	~Agent();

	void mutate();

	void calculate_fitness();
	double get_fitness();

	std::vector<double> get_position();
	void set_position(std::vector<double> new_position);



private:
	size_t d;
	std::vector<double> position;
	size_t fitness;
	Mutation* mutation_operator;
	Function* fitness_function;
	bool fitness_uptodate;

};
#pragma once

#include "tools.h"
#include "functions.h"
#include "mutation.h"
#include "crossover.h"

//TODO: double/float consistency

//TODO: look at this
// class Crossover;
class Mutation;
#include <chrono> //sleep
#include <thread> //sleep

#include <cstdlib>
#include <iostream>

class Agent {
public:
	Agent(size_t dimension, Mutation* mutation_operator, Crossover* crossover_operator, Function* fitness_function);
	~Agent();

	void mutate(std::vector<Agent*> cur_gen, size_t idx);
	void crossover(std::vector<Agent*> next_gen, size_t idx);

	void calculate_fitness();
	double get_fitness();

	std::vector<double> get_position();
	void set_position(std::vector<double> new_position);



private:
	void print_position(std::vector<double> to_print);

	size_t dim;
	std::vector<double> position;
	std::vector<double> donor;
	size_t fitness;
	Mutation* mutation_operator;
	Crossover* crossover_operator;
	Function* fitness_function;
	bool fitness_uptodate;



};
#pragma once

#include "tools.h"
#include "ioh.hpp"
#include "mutation.h"
#include "crossover.h"
#include "boundary.h"
#include "operators.h"

//TODO: look at this
// #include "functions.h"
// class Crossover;
class Mutation;
#include <chrono> //sleep
#include <thread> //sleep

#include <cstdlib>
#include <iostream>

class Agent {
public:
	Agent(size_t dimension, std::shared_ptr<Mutation> mutation_operator, std::shared_ptr<Crossover> crossover_operator, 
			std::shared_ptr<Boundary> boundary_correction, ioh::problem::RealSingleObjective* target_function, unsigned int* budget, int resample_limit);
	~Agent();

	void mutate(const std::vector<Agent*>& cur_gen, size_t idx);
	void crossover(std::vector<Agent*>& next_gen, size_t idx);

	void calculate_fitness();
	double get_fitness();

	bool check_position_oob(const std::vector<double>& given_position);

	const std::vector<double>& get_position();
	void set_position(std::vector<double> new_position);


	CROSSOVER get_crossover();
	MUTATION get_mutation();
	BOUNDARY get_boundary();

	void set_crossover(std::shared_ptr<Crossover> new_crossover){this->crossover_operator = new_crossover;};
	void set_mutation(std::shared_ptr<Mutation> new_mutation){this->mutation_operator = new_mutation;};
	void set_boundary(std::shared_ptr<Boundary> new_boundary){this->boundary_correction = new_boundary;};

	std::shared_ptr<Crossover> get_crossover_ptr(){return this->crossover_operator;};
	std::shared_ptr<Mutation> get_mutation_ptr(){return this->mutation_operator;};
	std::shared_ptr<Boundary> get_boundary_ptr(){return this->boundary_correction;};

	std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> update_history();
	void add_history(std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> snapshot);
	std::vector< std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> > get_history() {return this->history;};
	void print_history(bool print_positions = false);


private:
	void print_position(std::vector<double> to_print);
	
	size_t dim;
	std::vector<double> position;
	std::vector<double> donor;
	double fitness;
	unsigned int* budget;

	std::shared_ptr<Mutation> mutation_operator;
	std::shared_ptr<Crossover> crossover_operator;
	std::shared_ptr<Boundary> boundary_correction;

	ioh::problem::RealSingleObjective* target_function;
	bool fitness_uptodate;

	int resample_limit; //10+ln(dim)^2

	// position, fitness, crossover operator, mutation operator, boundary operator
	std::vector<
		std::tuple<
			std::vector<double>, 			//position
			double, 						//fitness
			std::shared_ptr<Crossover>, 	
			std::shared_ptr<Mutation>, 
			std::shared_ptr<Boundary>
		> 
	> history;



};
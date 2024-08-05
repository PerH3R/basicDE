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
	Agent(int id, size_t dimension, std::shared_ptr<Mutation> mutation_operator, std::shared_ptr<Crossover> crossover_operator, 
			std::shared_ptr<Boundary> boundary_correction, ioh::problem::RealSingleObjective* target_function, unsigned int* budget, int resample_limit);
	~Agent();

	// Applies mutation operator to current Agent
	void mutate(const std::vector<Agent*>& cur_gen, size_t idx);

	// Applies mutation operator to current Agent
	void crossover(std::vector<Agent*>& next_gen, size_t idx);

	// Updates and returns fitness of current Agent
	double get_fitness();

	// return current position
	const std::vector<double>& get_position();

	// Move current Agent to a new position
	void set_position(std::vector<double> new_position);

	// Checks if supplied position is within valid problem bounds. Called in mutate() (see resample_limit)
	bool check_position_oob(const std::vector<double>& given_position);

	// Returns currently set operators
	CROSSOVER get_crossover();
	MUTATION get_mutation();
	BOUNDARY get_boundary();

	// Returns identifier nr. of current Agent
	const int get_id() const {return id;};

	// Change set operator to a new operator using a pointer to to said operator.
	void set_crossover(std::shared_ptr<Crossover> new_crossover){this->crossover_operator = new_crossover;};
	void set_mutation(std::shared_ptr<Mutation> new_mutation){this->mutation_operator = new_mutation;};
	void set_boundary(std::shared_ptr<Boundary> new_boundary){this->boundary_correction = new_boundary;};

	// get a pointer to currently set operator
	std::shared_ptr<Crossover> get_crossover_ptr(){return this->crossover_operator;};
	std::shared_ptr<Mutation> get_mutation_ptr(){return this->mutation_operator;};
	std::shared_ptr<Boundary> get_boundary_ptr(){return this->boundary_correction;};

	// Add current state of Agent to history
	std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> update_history();

	// Add a (constructed) state to history
	void add_history(std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> snapshot);

	// Retrieve history of current Agent
	const std::vector< std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> >& get_history() {return this->history;};
	
	// Prints out entire history to terminal (printing of positions optional to increase readability)
	void print_history(bool print_positions = false);


private:
	// Prints a position to terminal. Currently unused, but used for debug purposes
	void print_position(std::vector<double> to_print);

	// Calculates and updates fitness using the currently stored position
	void calculate_fitness();
	double fitness;

	const int id;	
	size_t dim;
	

	// Current position of the Agent. Base vector
	std::vector<double> position;

	// Donor vector calculated by mutation operator
	std::vector<double> donor;
	
	// Currently used operators
	std::shared_ptr<Mutation> mutation_operator;
	std::shared_ptr<Crossover> crossover_operator;
	std::shared_ptr<Boundary> boundary_correction;

	// Pointer to problem function
	ioh::problem::RealSingleObjective* target_function;
	
	// Total shared budget for entire population
	unsigned int* budget;

	// How often the mutation operator can be retried if the resulting donor vector is out of bounds
	int resample_limit; //10+ln(dim)^2

	// Keeps track of movement in order to not recalculate fitness if no movement has occured
	bool fitness_uptodate;

	// History of the agent
	// position, fitness, crossover operator, mutation operator, boundary operator
	std::vector<
		std::tuple<
			std::vector<double>,
			double,
			std::shared_ptr<Crossover>, 	
			std::shared_ptr<Mutation>, 
			std::shared_ptr<Boundary>
		> 
	> history;



};
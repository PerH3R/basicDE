#include "../include/agent.h"

Agent::Agent(size_t dimension, Mutation* mutation_operator, Function* fitness_function) {
	this->d = dimension;
	this->mutation_operator = mutation_operator;
	this->fitness_function = fitness_function;
	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;
}

Agent::~Agent() {

}

void Agent::calculate_fitness() {
	this->fitness = this->fitness_function->evaluate(this->position);
	fitness_uptodate = true;
}

double Agent::get_fitness() {
	if (fitness_uptodate == false) {
		this->calculate_fitness();
	}
	return this->fitness;
}

std::vector<double> Agent::get_position() {
	return this->position;
}

void Agent::set_position(std::vector<double> new_position) {
	if (new_position.size() == this->d) {
		this->position = new_position;
		fitness_uptodate = false;
	}
}
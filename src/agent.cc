#include "../include/agent.h"

Agent::Agent(size_t dimension, Mutation* mutation_operator, Crossover* crossover_operator, Function* fitness_function) {
	this->d = dimension;
	this->mutation_operator = mutation_operator;
	this->crossover_operator = crossover_operator;
	this->fitness_function = fitness_function;
	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;

	//TODO:initialize pos and donor
	std::pair<double,double> range = fitness_function->get_range();
	position.reserve(dimension);
	donor.reserve(dimension);
	for (int i = 0; i < d; ++i)	{

		float value = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(range.first-range.second));
		this->position[i] = value;
		this->donor[i] = value;
	}
	// std::cout << "new agent at:" << position << std::endl;

}

Agent::~Agent() {

}

void Agent::mutate(std::vector<std::shared_ptr<Agent>> cur_gen, size_t idx){
	this->donor = this->mutation_operator->apply(cur_gen, idx);
}

void Agent::crossover(std::vector<std::shared_ptr<Agent>> next_gen, size_t idx){
	next_gen[idx]->set_position(this->crossover_operator->apply(this->position, donor));
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
	//if dimension fits and new position is different from current position
	if (new_position.size() == this->d && new_position != this->position) {
		this->position = new_position;
		fitness_uptodate = false;
	}
}
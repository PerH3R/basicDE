#include "../include/agent.h"

Agent::Agent(size_t dimension, Mutation* mutation_operator, Crossover* crossover_operator, Function* fitness_function) : 
			dim(dimension), mutation_operator(mutation_operator), crossover_operator(crossover_operator), fitness_function(fitness_function) {
	// this->dim = dimension;
	// this->mutation_operator = mutation_operator;
	// this->crossover_operator = crossover_operator;
	// this->fitness_function = fitness_function;
	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;

	//TODO:rand seed
	std::pair<double,double> range = fitness_function->get_range();
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(range.first, range.second);
	this->position.reserve(dim);
	this->donor.reserve(dim);
	for (size_t i = 0; i < dim; ++i)	{
		float value = distribution(generator);
		this->position[i] = value;
		this->donor[i] = value;
	}
	this->calculate_fitness();
	std::cout << "new agent with fitness:" << this->fitness << " at " << this->position[0] << " " << this->position[1] << std::endl;

}

Agent::~Agent() {

}

void Agent::mutate(std::vector<Agent*> cur_gen, size_t idx){
	this->donor = this->mutation_operator->apply(cur_gen, idx);
}

void Agent::crossover(std::vector<Agent*> next_gen, size_t idx){
	next_gen[idx]->set_position( this->crossover_operator->apply(position, donor) );
}

void Agent::calculate_fitness() {
	this->fitness = this->fitness_function->evaluate(position);
	fitness_uptodate = true;
}

double Agent::get_fitness() {
	if (fitness_uptodate == false) {
		std::cout << "calculating fitness... ";
		this->calculate_fitness();
	}
	std::cout << fitness << std::endl;
	return fitness;
}

std::vector<double>& Agent::get_position() {
	return position;
}

void Agent::set_position(std::vector<double> new_position) {
	//if dimension fits and new position is different from current position
	if (new_position.size() == this->dim && new_position != this->position) {
		this->position = new_position;
		fitness_uptodate = false;
	}
}
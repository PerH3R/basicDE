#include "../include/agent.h"

Agent::Agent(size_t dimension, Mutation* mutation_operator, Crossover* crossover_operator, Function* fitness_function) : 
			dim(dimension), mutation_operator(mutation_operator), crossover_operator(crossover_operator), fitness_function(fitness_function) {
	// this->dim = dimension;
	// this->mutation_operator = mutation_operator;
	// this->crossover_operator = crossover_operator;
	// this->fitness_function = fitness_function;
	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;

	//TODO:remove sleep
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	//TODO:rand seed
	set_seed();
	std::pair<double,double> range = fitness_function->get_range();
	// std::default_random_engine generator;
	// std::uniform_real_distribution<float> distribution(range.first, range.second);
	this->position.reserve(dim);
	this->donor.reserve(dim);
	for (size_t i = 0; i < dim; ++i)	{
		// float value = distribution(generator);
		double random = (double)rand()/RAND_MAX;
		double value = range.first + random * (range.second-range.first);
		this->position.push_back(value);
		this->donor.push_back(value);
	}
	this->calculate_fitness();
	std::cout << "new agent with fitness:" << this->fitness << " at ";
	print_position(this->position);

}

Agent::~Agent() {

}

void Agent::print_position(std::vector<double> to_print){
	for (size_t i = 0; i < this->dim; ++i){
		std::cout << to_print[i] << " ";
	}
	std::cout << to_print.size() << std::endl;
}

void Agent::mutate(std::vector<Agent*> cur_gen, size_t idx){
	// std::cout << "==========" << std::endl << "mutating" << std::endl;
	// print_position(this->position);
	std::vector<double> donor_vec = this->mutation_operator->apply(cur_gen, idx);
	for (size_t i = 0; i < this->dim; ++i){
		this->donor[i] = donor_vec[i];
	}
	// this->donor = donor_vec;
	// print_position(this->donor);
	// std::cout << "==========" << std::endl;
}

void Agent::crossover(std::vector<Agent*> next_gen, size_t idx){
	// std::cout << "==========" << std::endl << "crossover" << std::endl;
	// print_position(this->position);
	// print_position(this->donor);
	std::vector<double> new_position = this->crossover_operator->apply(this->position, this->donor);
	// print_position(new_position);
	// std::cout << "agent co idx: " << idx << std::endl;
	next_gen[idx]->set_position(new_position);
	// std::cout << "=========" << std::endl;
}

void Agent::calculate_fitness() {
	// std::cout << "fitness " << std::endl;
	// print_position(this->position);
	this->fitness = this->fitness_function->evaluate(position);
	fitness_uptodate = true;
	// print_position(this->position);
}

double Agent::get_fitness() {
	if (fitness_uptodate == false) {
		// std::cout << "calculating fitness... ";
		this->calculate_fitness();
	}
	// std::cout << fitness << std::endl;
	return fitness;
}

std::vector<double> Agent::get_position() {
	return position;
}

void Agent::set_position(std::vector<double> new_position) {
	//if dimension fits and new position is different from current position
	if (new_position.size() == this->dim) {
		if (this->position.size() == this->dim){
			// for (double i : this->position){
				
			// }
			for (size_t i = 0; i < this->dim; ++i){
				this->position[i] = new_position[i];
			}

			// this->position = new_position;
			fitness_uptodate = false;
		}else{
			std::cerr << "incorrect dimensions current position\n";
		}
	}else{
		std::cerr << "incorrect dimensions new position\n";
	}
}
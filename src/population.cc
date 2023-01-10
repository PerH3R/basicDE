#include "../include/population.h"

Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, Function* target_function,
					   size_t size, size_t dim) {
	this->crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->mutation_operator = mutation_operator;
	this->target_function = target_function;
	this->n = size;
	this->d = dim;

	this->cur_gen.reserve(size);
	this->next_gen.reserve(size);
	for (size_t i = 0; i < size; i++) {
		this->cur_gen.push_back(std::make_shared<Agent>(dim, mutation_operator, crossover_operator, target_function));
		this->next_gen.push_back(std::make_shared<Agent>(dim, mutation_operator, crossover_operator, target_function));
	}

	

}

Population::~Population() {

}

void Population::apply_mutation() {
	for (size_t i = 0; i < this->n; i++) {
		//this->next_gen[i] = 
		this->cur_gen[i]->mutate(this->cur_gen, i);
	}
	// mutation_operator->apply(this->cur_gen);
}

void Population::apply_crossover() {
	for (size_t i = 0; i < this->n; i++) {
		//this->next_gen[i] = 
		this->cur_gen[i]->crossover(this->next_gen, i);
	}
	
}

void Population::apply_selection() {
	selection_operator->apply(this->cur_gen, this->next_gen);
}

void Population::sort(){
	bool sorted;
	do{
		sorted = true;
		for (size_t i = 0; i < n-1; ++i){
			double cur = cur_gen[i]->get_fitness();
			double next = cur_gen[i+1]->get_fitness();
			if (cur > next){
				sorted = false;
				std::swap(cur_gen[i], cur_gen[i+1]);
			}
		}
	}while(sorted == false);
}

std::vector<std::shared_ptr<Agent>> Population::get_current_generation(){
	return cur_gen;
}


std::vector<std::shared_ptr<Agent>> Population::get_next_generation(){
	return next_gen;
}
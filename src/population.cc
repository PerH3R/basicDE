#include "..\include\population.h"

Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, size_t size, size_t dim) {
	this->crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->n = size;
	this->d = dim;

	this->cur_gen.reserve(size);
	this->next_gen.reserve(size);
	for (size_t i = 0; i < size; i++) {
		this->cur_gen.push_back(std::make_unique<Agent>(dim, mutation_operator));
		this->next_gen.push_back(std::make_unique<Agent>(dim, mutation_operator));
	}

	

}

Population::~Population() {

}

void Population::apply_mutation() {
	mutation_operator->apply(this->cur_gen);
}

void Population::apply_crossover() {
	for (size_t i = 0; i < this->n; i++) {
		this->next_gen[i] = this->crossover_operator->apply(this->cur_gen, i);
	}
	
}

void Population::apply_selection() {
	selection_operator->apply(this->cur_gen, this->next_gen);
}
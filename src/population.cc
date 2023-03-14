#include "../include/population.h"


//TODO: change variable assignment
Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::RealSingleObjective* target_function,
					   Boundary* boundary_correction, size_t size, unsigned int* budget) {
	std::cout << "creating Population...";
	this->crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->mutation_operator = mutation_operator;
	this->boundary_correction = boundary_correction;
	this->target_function = target_function;
	this->n = target_function->meta_data().n_variables;
	this->dim = target_function->bounds().lb.size();
	this->budget=budget;

	this->cur_gen.reserve(size);
	this->next_gen.reserve(size);
	for (size_t i = 0; i < size; i++) {
		this->cur_gen.push_back(new Agent(dim, mutation_operator, crossover_operator, boundary_correction, target_function, budget));
		this->next_gen.push_back(new Agent(dim, mutation_operator, crossover_operator, boundary_correction, target_function, budget));
	}
	std::cout << " done!" << std::endl;
	

}

Population::~Population() {
	for(Agent* i : cur_gen){
		delete i;
		i = NULL;
	}
	for(Agent* i : next_gen){
		delete i;
		i = NULL;
	}
}

size_t Population::get_population_size(){
	return this->n;
}

void Population::print_fitness(){
	for (size_t i = 0; i < n; ++i){
		std::cout << cur_gen[i]->get_fitness() << " ";
	}
	std::cout << std::endl;
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

void Population::add_to_archive(std::vector< std::pair<std::vector<double>, double > > rejected_values){
	int rv_idx = 0;
	while (archive.size() < n*archive_multiplier){
		archive.push_back(rejected_values[rv_idx]);
		rv_idx++;
	}
	while (rv_idx < rejected_values.size()){
		//TODO: randomly generate number
		std::default_random_engine int_generator;
		std::uniform_int_distribution<size_t> distribution(0, n*archive_multiplier);
		archive[distribution(int_generator)] = rejected_values[rv_idx];
		rv_idx++;
	}

}

void Population::apply_selection() {
	if (mutation_operator->use_archive()){
		add_to_archive(selection_operator->apply(this->cur_gen, this->next_gen));
	}else{
	selection_operator->apply(this->cur_gen, this->next_gen);
	}
	// for (size_t i = 0; i < n; ++i)
	// {
	// 	// delete cur_gen[i];
	// 	// delete next_gen[i];
	// }
	// cur_gen = selected_agents;
	// for (size_t i = 0; i < n; i++) {
	// 	this->next_gen[i] = new Agent(dim, mutation_operator, crossover_operator, target_function);
	// }
}

void Population::sort(){
	bool sorted;
	//TODO: efficient sorting
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

std::vector<Agent*> Population::get_current_generation(){
	return cur_gen;
}


std::vector<Agent*> Population::get_next_generation(){
	return next_gen;
}
#include "../include/population.h"


//TODO: change variable assignment
Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::RealSingleObjective* target_function,
					   Boundary* boundary_correction, size_t pop_size, unsigned int* budget) {
	std::cout << "creating Population...";
	this->crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->mutation_operator = mutation_operator;
	this->boundary_correction = boundary_correction;
	this->target_function = target_function;
	this->n = pop_size;
	this->dim = target_function->meta_data().n_variables;
	this->budget=budget;

	this->cur_gen.reserve(this->n);
	this->next_gen.reserve(this->n);
	for (size_t i = 0; i < this->n; i++) {
		this->cur_gen.push_back(create_agent());
		this->next_gen.push_back(create_agent());
	}
	std::cout << "population created!" << std::endl;
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
	for(Agent* i : archive){
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

bool Population::agent_in_use(Agent* target){
	for (auto x : cur_gen){
		if (x == target){
			return true;
		}
	}
	for (auto x : next_gen){
		if (x == target){
			return true;
		}
	}
	return false;
} 

void Population::add_next_to_archive(){
	int rv_idx = 0;
	std::default_random_engine int_generator;
	std::uniform_int_distribution<size_t> distribution(0, archive_size);

	for (int i = 0; i < this->n; ++i){
		if (archive.size() < archive_size){
			next_gen[i];
		}else{ //archive limit reached
			n = distribution(int_generator	);
			delete archive[i];
			archive[i] = next_gen[i];
			next_gen[i] = create_agent();
		}
	} 

	// while (archive.size() < archive_size){
	// 	archive.push_back(rejected_values[rv_idx]);
	// 	rv_idx++;
	// }
	// while (rv_idx < rejected_values.size()){
	// 	//TODO: randomly generate number
		
	// 	archive[distribution(int_generator)] = rejected_values[rv_idx];
	// 	rv_idx++;
	// }

}

void Population::apply_selection() {
	auto kept_indexes = selection_operator->apply(this->cur_gen, this->next_gen);
	if (mutation_operator->use_archive()){
		add_next_to_archive();
	}
	
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

Agent* Population::create_agent(){
	return new Agent(dim, mutation_operator, crossover_operator, boundary_correction, target_function, budget);
}

std::vector<Agent*> Population::get_current_generation(){
	return cur_gen;
}


std::vector<Agent*> Population::get_next_generation(){
	return next_gen;
}
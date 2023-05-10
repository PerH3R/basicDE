#include "../include/population.h"


//TODO: change variable assignment
Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::RealSingleObjective* target_function,
					   Boundary* boundary_correction, size_t pop_size, unsigned int* budget, int archive_size) {
	std::cout << "creating Population...";
	this->crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->mutation_operator = mutation_operator;
	this->boundary_correction = boundary_correction;
	this->target_function = target_function;
	this->n = pop_size;
	this->dim = target_function->meta_data().n_variables;
	this->budget=budget;
	this->archive_size = archive_size;

	this->cur_gen.reserve(pop_size);
	this->next_gen.reserve(pop_size);
	for (size_t i = 0; i < pop_size; i++) {
		this->cur_gen.push_back(create_agent());
		this->next_gen.push_back(create_agent());
	}
	std::cout << " Population created!" << std::endl;
	

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

Agent* Population::create_agent(){
	return new Agent(dim, mutation_operator, crossover_operator, boundary_correction, target_function, budget);
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

void Population::randomise_population(){
	for (size_t i = 0; i < this->n; i++) {
		std::vector<double> new_position;
		new_position.reserve(this->dim);
		for (int j = 0; j < this->dim; ++j){
			double value = tools.rand_double_unif(target_function->bounds().lb[j], target_function->bounds().ub[j]);
			new_position.push_back(value);
		}
		this->cur_gen[i]->set_position(new_position);
		this->next_gen[i]->set_position(new_position);
	}
}

void Population::repopulate_next_gen() {
	for (size_t i = 0; i < this->n; i++) {
		if (mutation_operator->use_archive() == false)		{
			delete next_gen[i];
			next_gen[i] = NULL;
		}
		this->next_gen[i] = create_agent();
	}
	
}

void Population::add_to_archive(){
	int ra_idx = 0; //keeps track of rejected agent to add from next_gen
	while (archive.size() < archive_size){
		archive.push_back(next_gen[ra_idx]);
		ra_idx++;
	}
	while (ra_idx < next_gen.size()){
		//TODO: randomly generate number
		std::default_random_engine int_generator;
		std::uniform_int_distribution<size_t> distribution(0, archive_size);
		delete archive[distribution(int_generator)];
		archive[distribution(int_generator)] = next_gen[ra_idx];
		ra_idx++;
	}

}

void Population::apply_selection() {
	selection_operator->apply(this->cur_gen, this->next_gen);

	if (mutation_operator->use_archive()){ add_to_archive(); }

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
	//TODO: more efficient sorting?
	// reverse bubble sort
	do{
		sorted = true;
		for (size_t i = this->n-1; i > 0; --i){ //looping backwards might be faster
			double cur = cur_gen[i]->get_fitness();
			double next = cur_gen[i-1]->get_fitness();
			if (cur < next){
				sorted = false;
				std::swap(cur_gen[i], cur_gen[i-1]);
				std::swap(next_gen[i], next_gen[i-1]); //keeping indexes matched is important for some operators
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

void Population::set_individual_crossover(Crossover* new_crossover, int idx){
	if (idx == -1){
		//change all
		for (int i = 0; i < this->n; ++i){
			cur_gen[i]->set_crossover(new_crossover);
			next_gen[i]->set_crossover(new_crossover);
		}
		/* code */
	} else if(idx > -1 && idx < this->n){
		//change single individual
		cur_gen[idx]->set_crossover(new_crossover);
		next_gen[idx]->set_crossover(new_crossover);
	}else{
		std::cerr << "invalid index: " << idx << std::endl;
	}
}

void Population::set_individual_mutation(Mutation* new_mutation, int idx){
	if (idx == -1){
		//change all
		for (int i = 0; i < this->n; ++i){
			cur_gen[i]->set_mutation(new_mutation);
			next_gen[i]->set_mutation(new_mutation);
		}
		/* code */
	} else if(idx > -1 && idx < this->n){
		//change single individual
		cur_gen[idx]->set_mutation(new_mutation);
		next_gen[idx]->set_mutation(new_mutation);
	}else{
		std::cerr << "invalid index: " << idx << std::endl;
	}
}

void Population::set_individual_boundary(Boundary* new_boundary, int idx){
	if (idx == -1){
		//change all
		for (int i = 0; i < this->n; ++i){
			cur_gen[i]->set_boundary(new_boundary);
			next_gen[i]->set_boundary(new_boundary);
		}
		/* code */
	} else if(idx > -1 && idx < this->n){
		//change single individual
		cur_gen[idx]->set_boundary(new_boundary);
		next_gen[idx]->set_boundary(new_boundary);
	}else{
		std::cerr << "invalid index: " << idx << std::endl;
	}
}
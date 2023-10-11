#include "../include/population.h"


//TODO: change variable assignment
Population::Population(Crossover* crossover_operator, Selection* selection_operator, Mutation* mutation_operator, ioh::problem::RealSingleObjective* target_function,
					   Boundary* boundary_correction, size_t pop_size, unsigned int* budget, int archive_size) {
	std::cout << "creating Population...";
	this->base_crossover_operator = crossover_operator;
	this->selection_operator = selection_operator;
	this->base_mutation_operator = mutation_operator;
	this->base_boundary_correction = boundary_correction;
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
	//TODO remove?
	// for(Agent* i : archive){
		// delete i;
		// i = NULL;
	// }
}

Agent* Population::create_agent(){
	return new Agent(dim, base_mutation_operator, base_crossover_operator, base_boundary_correction, target_function, budget);
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
		if (this->cur_gen[i]->get_mutation() == DIRMUT){
			this->cur_gen[i]->get_mutation_ptr()->pass_vector_pool(this->vector_pool);
			if (this->improved){
				this->cur_gen[i]->get_mutation_ptr()->improved_to_true();
			}			
			this->cur_gen[i]->mutate(this->cur_gen, i);
		}
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
		if (next_gen[i]->get_mutation_ptr()->use_archive() == false){
			delete next_gen[i];
			next_gen[i] = NULL;
		}
		this->next_gen[i] = create_agent();
	}
	
}

void Population::add_to_archive(){
	int reject_idx = 0; //keeps track of rejected agent to add from next_gen
	while (this->archive.size() < this->archive_size){
		archive.push_back(next_gen[reject_idx]->get_history().back());
		reject_idx++;
	}
	while (reject_idx < next_gen.size()){
		int rand_idx = tools.rand_int_unif(0, archive_size);
		// delete archive[rand_idx];
		this->archive[rand_idx] = next_gen[reject_idx]->get_history().back();
		reject_idx++;
	}

}

void Population::apply_selection() {
	//swap outperforming trial vectors to cur_gen
	auto kept_indexes = selection_operator->apply(this->cur_gen, this->next_gen);
	for (int i = 0; i < this->n; ++i){
		//record winning strategy
		auto t = this->cur_gen[i]->update_history();
		//record winning strategy in counterpart
		this->next_gen[i]->add_history(t);
	}

	//TODO set to true 
	if (false){ 
		add_to_archive(); 
	}
}

void Population::sort(){
	bool sorted;
	//TODO: is there a more efficient sorting?
	// reverse bubble sort
	do{
		sorted = true;
		for (size_t i = this->n-1; i > 0; --i){ //looping backwards sounds faster
			double cur = cur_gen[i]->get_fitness();
			double next = cur_gen[i-1]->get_fitness();
			if (cur < next){
				sorted = false;
				std::swap(cur_gen[i], cur_gen[i-1]);
				std::swap(next_gen[i], next_gen[i-1]); //keeping indexes matched is important for keeping track of history
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
	} else if(idx > -1 && idx < this->n){
		//change single individual
		cur_gen[idx]->set_boundary(new_boundary);
		next_gen[idx]->set_boundary(new_boundary);
	}else{
		std::cerr << "invalid index: " << idx << std::endl;
	}
}

//default: filenames=""
void Population::write_population(std::string filename){
	//if filename not provided, automatic filename generation based on problem attributes if 
	// std::string file_location = "progress/";
	if (filename == ""){
		filename = std::to_string(target_function->meta_data().problem_id) + "_";
		filename += target_function->meta_data().name + "_";
		filename += std::to_string(target_function->meta_data().n_variables) + "_";
		filename += std::to_string(this->base_mutation_operator->get_type());
		filename += std::to_string(target_function->meta_data().instance);
	}
	// filename = file_location + filename;
	std::ofstream positionfile;
	positionfile.open(filename, std::ios::app);
	for (int i = 0; i < n; ++i){
		std::vector<double> i_pos = this->cur_gen[i]->get_position();
		std::ostringstream pos_oss;
    	std::copy(i_pos.begin(), i_pos.end(), std::ostream_iterator<double>(pos_oss, ","));
    	std::string pos_str( pos_oss.str() );
    	positionfile << pos_str;
    	positionfile << ";";
	}
	positionfile << '\n';
	positionfile.close();
	
}

void Population::update_vector_pool(double previous_best_fitness){
	for (int i = 0; i < this->n; ++i){
		if (cur_gen[i]->get_fitness() < previous_best_fitness){
			//next_gen contains previous generation at this point in the loop
			auto diff = tools.vec_sub(cur_gen[i]->get_position(), next_gen[i]->get_position());
			this->vector_pool.push_back(diff); 
		}
	}
	// std::cout << "vector pool size: " << vector_pool.size() << std::endl;
	this->improved = true;
}
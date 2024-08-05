#include "../include/population.h"



Population::Population(const Argparse* argparser, ioh::problem::RealSingleObjective* target_function, const size_t n, unsigned int* budget, int archive_size, int resample_limit) :
	argparser(argparser), target_function(target_function), n(n), budget(budget), archive_size(archive_size) {
	std::cout << "creating Population...";

	this->F = std::stod(argparser->get_values()["-F"]);
	this->Cr = std::stod(argparser->get_values()["-Cr"]);
	this->F = std::stod(argparser->get_values()["-F"]);
	this->dim = target_function->meta_data().n_variables;

	this->base_selection = std::stoi(argparser->get_values()["-s"]);
	this->base_crossover = std::stoi(argparser->get_values()["-c"]);
	this->base_boundary = std::stoi(argparser->get_values()["-b"]);
	this->base_mutation = std::stoi(argparser->get_values()["-m"]);

	this->selection_operator = create_selection_operator(this->base_selection);

	// if (n <4){
	// 	this->n = 5*ta-rget_function->meta_data().n_variables;;
	// } else {
	// 	this->n = n;
	// }//automatic population size if too small or not specified

	this->resample_limit = resample_limit;
	
	this->dim = target_function->meta_data().n_variables;
	// this->archive.resize(archive_size);
	this->cur_gen.reserve(n);
	this->next_gen.reserve(n);
	for (size_t i = 0; i < n; i++) {
		this->cur_gen.push_back(create_agent(i));
		this->next_gen.push_back(create_agent(i));
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
}

Agent* Population::create_agent(int id){
	return new Agent(id, dim, create_mutation_operator(-1, this->F), 
						  create_crossover_operator(-1, this->Cr), create_boundary_operator(), target_function, budget, this->resample_limit); 
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
		this->next_gen[i] = create_agent(i);
	}	
}

void Population::add_to_archive(std::vector<bool> kept_indexes){
	for (size_t i = 0; i < kept_indexes.size(); ++i){
		if (kept_indexes[i] == 0){ //if child usurped parent
			if (this->archive.size() < this->archive_size){ //if archive full
				archive.push_back(std::get<0>(next_gen[i]->get_history().back())); //put parent position in archive
			}else{
				int rand_idx = tools.rand_int_unif(0, this->archive_size);
				this->archive[rand_idx] = std::get<0>(next_gen[i]->get_history().back()); //replace random spot in archive
			}
		}
	}
}

void Population::apply_selection() {
	//swap outperforming trial vectors to cur_gen
	auto kept_indexes = this->selection_operator->apply(this->cur_gen, this->next_gen);
	for (size_t i = 0; i < this->n; ++i){
		//record winning strategy
		auto t = this->cur_gen[i]->update_history();
		//record winning strategy in counterpart
		this->next_gen[i]->add_history(t);
	}

	//TODO set to true 
	if (this->archive_size > 0){
		add_to_archive(kept_indexes); 
	}
}

void Population::sort(){
	bool sorted;
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

void Population::set_individual_crossover(std::shared_ptr<Crossover> new_crossover, int idx){
	if (idx == -1){
		//change all
		for (size_t i = 0; i < this->n; ++i){
			cur_gen[i]->set_crossover(new_crossover);
			next_gen[i]->set_crossover(new_crossover);
		}
	} else if(idx > -1 && idx < this->n){
		//change single individual
		//TODO: get Cr value from mutation operator
		cur_gen[idx]->set_crossover(new_crossover);
		next_gen[idx]->set_crossover(new_crossover);
	}else{
		std::cerr << "invalid index: " << idx << std::endl;
	}
}

void Population::set_individual_mutation(std::shared_ptr<Mutation> new_mutation, int idx){
	if (idx == -1){
		//change all
		for (size_t i = 0; i < this->n; ++i){
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

void Population::set_individual_boundary(std::shared_ptr<Boundary> new_boundary, int idx){
	if (idx == -1){
		//change all
		for (size_t i = 0; i < this->n; ++i){
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
		filename += MUTATION_NAMES[base_mutation] + "_";
		filename += argparser->get_values()["-a"] + "_";
		filename += std::to_string(target_function->meta_data().instance);
	}
	// filename = file_location + filename;
	std::ofstream positionfile;
	positionfile.open(filename, std::ios::app);
	for (size_t i = 0; i < n; ++i){
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
	for (size_t i = 0; i < this->n; ++i){
		if (cur_gen[i]->get_fitness() < previous_best_fitness){
			//next_gen contains previous generation at this point in the loop
			auto diff = tools.vec_sub(cur_gen[i]->get_position(), next_gen[i]->get_position());
			this->vector_pool.push_back(diff); 
		}
	}
	this->improved = true;
}

//default mut_op = -1, F = 0.0
std::shared_ptr<Mutation> Population::create_mutation_operator(int mut_op, float F){

	if (mut_op == -1){
		mut_op = base_mutation;
	}
	if(F != 0.0){
		switch(mut_op){
			case 0:
				return std::make_shared<RandDiv1>(this->dim, this->n, F);
			case 1:
				return std::make_shared<RandDiv2>(this->dim, this->n, F);
			case 2:
				return std::make_shared<BestDiv1>(this->dim, this->n, F);
			case 3:
				return std::make_shared<BestDiv2>(this->dim, this->n, F);
			case 4:	
				return std::make_shared<TargetToPBestDiv1>(this->dim, this->n, 
					&(this->archive), F, this->archive_size);
			case 5:
				return std::make_shared<RandToBestDiv1>(this->dim, this->n, F);
			case 6:
				return std::make_shared<RandToBestDiv2>(this->dim, this->n, F);
			case 7:
				return std::make_shared<TargetToBestDiv1>(this->dim, this->n, F);
			case 8:
				return std::make_shared<TargetToBestDiv2>(this->dim, this->n, F);
			case 9:
				return std::make_shared<TargetToRandDiv1>(this->dim, this->n, F);
			case 10:
				return std::make_shared<TargetToRandDiv2>(this->dim, this->n, F);
			case 11:
				return std::make_shared<TwoOptDiv1>(this->dim, this->n, F);
			case 12:
				return std::make_shared<TwoOptDiv2>(this->dim, this->n, F);
			case 13:
				return std::make_shared<Desmu>(this->dim, this->n, F);
			case 14:
				return std::make_shared<Bea>(this->dim, this->n, this->create_boundary_operator(), this->target_function, this->budget, F);
			case 15:
				return std::make_shared<DirMut>(this->dim, this->n, F);
			case 16:
				return std::make_shared<RandomSearch>(this->dim, this->n, this->target_function, F);
			default:
				std::cerr << "Mutation operator " << mut_op << " out of range. Continuing using RandDiv1." << std::endl;
				return std::make_shared<RandDiv1>(this->dim, this->n, F);		
		}	
	} else {
		switch(mut_op){
			case 0:
				return std::make_shared<RandDiv1>(this->dim, this->n);
			case 1:
				return std::make_shared<RandDiv2>(this->dim, this->n);
			case 2:
				return std::make_shared<BestDiv1>(this->dim, this->n);
			case 3:
				return std::make_shared<BestDiv2>(this->dim, this->n);
			case 4:	
				return std::make_shared<TargetToPBestDiv1>(this->dim, this->n, 
					&(this->archive), this->archive_size);
			case 5:
				return std::make_shared<RandToBestDiv1>(this->dim, this->n);
			case 6:
				return std::make_shared<RandToBestDiv2>(this->dim, this->n);
			case 7:
				return std::make_shared<TargetToBestDiv1>(this->dim, this->n);
			case 8:
				return std::make_shared<TargetToBestDiv2>(this->dim, this->n);
			case 9:
				return std::make_shared<TargetToRandDiv1>(this->dim, this->n);
			case 10:
				return std::make_shared<TargetToRandDiv2>(this->dim, this->n);
			case 11:
				return std::make_shared<TwoOptDiv1>(this->dim, this->n);
			case 12:
				return std::make_shared<TwoOptDiv2>(this->dim, this->n);
			case 13:
				return std::make_shared<Desmu>(this->dim, this->n);
			case 14:
				return std::make_shared<Bea>(this->dim, this->n, this->create_boundary_operator(), this->target_function, this->budget);
			case 15:
				return std::make_shared<DirMut>(this->dim, this->n);
			case 16:
				return std::make_shared<RandomSearch>(this->dim, this->n, this->target_function);
			default:
				std::cerr << "Mutation operator " << mut_op << " out of range. Continuing using RandDiv1." << std::endl;
				return std::make_shared<RandDiv1>(this->dim, this->n);		
		}	
	}
}

//default sel_op = -1
std::shared_ptr<Selection> Population::create_selection_operator(int sel_op){
	if (sel_op == -1){
		sel_op = this->base_selection;
	}
	switch(sel_op){
		case 0:
			return std::make_shared<Elitist>(this->n);
		default:
			return std::make_shared<Elitist>(this->n);
	}
}

//default c_op = -1, Cr = 0.0
std::shared_ptr<Crossover> Population::create_crossover_operator(int c_op, float Cr){
	if (Cr == 0.0){
		Cr = this->Cr;
	}
	if (c_op == -1){
		c_op = base_crossover;
	}
	if(Cr == 0.0){ 
		switch(c_op){
			case 0:
				return std::make_shared<Binomial>(this->dim);
			case 1:
				return std::make_shared<Exponential>(this->dim);
			default:
				return std::make_shared<Binomial>(this->dim);
		}
	} else {
		switch(c_op){
			case 0:
				return std::make_shared<Binomial>(this->dim, Cr);
			case 1:
				return std::make_shared<Exponential>(this->dim, Cr);
			default:
				return std::make_shared<Binomial>(this->dim, Cr);
		}
	}
}

//default bound_op = -1
std::shared_ptr<Boundary> Population::create_boundary_operator(int bound_op){
	if (bound_op == -1){
		bound_op = base_boundary;
	}
	switch(bound_op){
		case 0:
			return std::make_shared<Clamp>(this->target_function);
		case 1:
			return std::make_shared<Reflect>(this->target_function);
		case 2:
			return std::make_shared<Reinit>(this->target_function);
		default:
			return std::make_shared<Clamp>(this->target_function);
	}
}
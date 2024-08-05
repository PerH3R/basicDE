#include "../include/agent.h"

Agent::Agent(int id, size_t dimension, std::shared_ptr<Mutation> mutation_operator, std::shared_ptr<Crossover> crossover_operator, std::shared_ptr<Boundary> boundary_correction, 
				ioh::problem::RealSingleObjective* target_function, unsigned int* budget, int resample_limit) : id(id),
			dim(dimension), mutation_operator(mutation_operator), crossover_operator(crossover_operator), boundary_correction(boundary_correction),
				target_function(target_function), budget(budget), resample_limit(resample_limit){

	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;

	this->position.reserve(dim);
	this->donor.reserve(dim);

	//random initialization
	for (size_t i = 0; i < dim; ++i){
		double value = tools.rand_double_unif(target_function->bounds().lb[i], target_function->bounds().ub[i]);
		this->position.push_back(value);
		this->donor.push_back(value);
	}
	this->calculate_fitness();



}

Agent::~Agent() {
}

void Agent::print_position(std::vector<double> to_print){
	for (size_t i = 0; i < this->dim; ++i){
		std::cout << to_print[i] << " ";
	}
	std::cout << to_print.size() << std::endl;
}

void Agent::mutate(const std::vector<Agent*>& cur_gen, size_t idx){	

	// Attempt generation of donor vector within bounds
	int tries = 0;
	do{
		tries++;
		this->donor = this->mutation_operator->apply(cur_gen, idx);
	}while((check_position_oob(donor) == true) && (tries <= this->resample_limit));
	
	// Apply correction if no donor vector within bounds could be generated
	if (check_position_oob(donor) == true){
		this->donor = boundary_correction->apply(this->donor);
	}
}

void Agent::crossover(std::vector<Agent*>& next_gen, size_t idx){
	// Obtain a new position given the current position and donor vector
	std::vector<double> new_position = this->crossover_operator->apply(this->position, this->donor);
	next_gen[idx]->set_position(new_position);
}


void Agent::calculate_fitness() {
	this->fitness = (*target_function)(position);
	fitness_uptodate = true;
}



double Agent::get_fitness() {
	// If movement has occured, ask problem funtion fitness of new position at cost of budget
	if (fitness_uptodate == false && *budget > 0) {
		calculate_fitness();
		*budget -= 1;
	}
	return fitness;
}

const std::vector<double>& Agent::get_position() {
	return position;
}

CROSSOVER Agent::get_crossover(){return this->crossover_operator->get_type();}
MUTATION Agent::get_mutation(){return this->mutation_operator->get_type();}
BOUNDARY Agent::get_boundary(){return this->boundary_correction->get_type();}

void Agent::set_position(std::vector<double> new_position) {
	// Make sure dimension fits
	if (new_position.size() != this->position.size()) {std::cerr << "incorrect dimensions new position\n";}

	// New position should be in bounds, check if this is the case
	std::vector<double> corrected_position = boundary_correction->apply(new_position);
	if (corrected_position != new_position){
		new_position = corrected_position;
		std::cerr << "correction applied - please investigate mutation or crossover operator";
	}
	

	// Update position
	for (size_t i = 0; i < this->dim; ++i){
		if (this->position[i] != new_position[i]){
			this->position[i] = new_position[i];
			this->fitness_uptodate = false;			//recalculate fitness when asked		
		}
	}
}

bool Agent::check_position_oob(const std::vector<double>& given_position){
	for (size_t i = 0; i < this->dim; ++i){
		if (given_position[i] < this->target_function->bounds().lb[i] || given_position[i] > this->target_function->bounds().ub[i]){
			return true;
		}
	}
	return false;
}

std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> Agent::update_history(){
	std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> t;
	if (!fitness_uptodate){		
		this->calculate_fitness();
		std::cerr << "Fitness wasn't up to date. Why?" << std::endl;
	}	
	t = std::make_tuple(this->position, this->fitness, this->crossover_operator, this->mutation_operator, this->boundary_correction);
		this->history.push_back(t);
	return t;
}

void Agent::add_history(std::tuple<std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>> snapshot){
	this->history.push_back(snapshot);
}

void Agent::print_history(bool print_positions){
	for (auto snapshot : this->history){
		std::string hist_string= "";
		const auto [pos, fitness, CrOpPtr, MutOpPtr, BoundOpPtr] = snapshot;
		
		// print position if asked to
		if (print_positions){
			for(double i : pos){
			std::cout << i <<',';
			}
		}
				
		//print other information
		std::cout << " " << fmt::format("{}", fitness) << " " << MutOpPtr->get_F() << " " << CrOpPtr->get_Cr() << " " 
					<< CROSSOVER_NAMES[CrOpPtr->get_type()] << " " << MUTATION_NAMES[MutOpPtr->get_type()] << " " << BOUNDARY_NAMES[BoundOpPtr->get_type()] << std::endl;
		
		std::cout << hist_string;
	}
}
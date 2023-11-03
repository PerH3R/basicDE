#include "../include/agent.h"

Agent::Agent(size_t dimension, std::shared_ptr<Mutation> mutation_operator, Crossover* crossover_operator, Boundary* boundary_correction, 
				ioh::problem::RealSingleObjective* target_function, unsigned int* budget) : 
			dim(dimension), mutation_operator(mutation_operator), crossover_operator(crossover_operator), boundary_correction(boundary_correction),
				target_function(target_function), budget(budget){
	// this->dim = dimension;
	// this->mutation_operator = mutation_operator;
	// this->crossover_operator = crossover_operator;
	// this->target_function = target_function;
	this->fitness = std::numeric_limits<double>::max();
	this->fitness_uptodate = false;


	// std::cout << target_function->meta_data() << std::endl;
    // std::cout << "bounds of variables :  " << target_function->bounds().lb << std::endl;

	//TODO: read funcion metadata
	
	// std::default_random_engine generator;
	// std::uniform_real_distribution<float> distribution(range.first, range.second);
	this->position.reserve(dim);
	this->donor.reserve(dim);
	for (size_t i = 0; i < dim; ++i)	{
		double value = tools.rand_double_unif(target_function->bounds().lb[i], target_function->bounds().ub[i]);
		this->position.push_back(value);
		this->donor.push_back(value);
	}
	this->calculate_fitness();
	// std::cout << "new agent with fitness:" << this->fitness << " at ";
	// print_position(this->position);
	this->resample_limit = 10+std::pow((std::log(this->dim)),2);

}

Agent::~Agent() {
	// std::vector<Crossover*> CrSeen;
	// std::vector<std::shared_ptr<Mutation>> MutSeen;
	// std::vector<Boundary*> BoundSeen;
	// for (auto snapshot : this->history){
	// 	auto [pos, fitness, CrOpPtr, MutOpPtr, BoundOpPtr] = snapshot;
	// 	std::cout << CrOpPtr << std::endl;
	// 	if (CrOpPtr){
	// 		CrSeen.push_back(CrOpPtr);
	// 		CrOpPtr = NULL;
	// 	}
	// 	if (MutOpPtr){
	// 		MutSeen.push_back(MutOpPtr);
	// 		MutOpPtr = NULL;
	// 	}
	// 	if (BoundOpPtr){
	// 		BoundSeen.push_back(BoundOpPtr);
	// 		BoundOpPtr = NULL;
	// 	}
	// }
	// for (auto ptr : CrSeen){
	// 	delete ptr; ptr=NULL;
	// }
	// for (auto ptr : MutSeen){
	// 	delete ptr; ptr=NULL;
	// }
	// for (auto ptr : BoundSeen){
	// 	delete ptr; ptr=NULL;
	// }
	// delete this->crossover_operator;
	// this->crossover_operator = NULL;
	// delete this->mutation_operator;
	// this->mutation_operator = NULL;
	// delete this->boundary_correction;
	// this->boundary_correction = NULL;
}

void Agent::print_position(std::vector<double> to_print){
	for (size_t i = 0; i < this->dim; ++i){
		std::cout << to_print[i] << " ";
	}
	std::cout << to_print.size() << std::endl;
}

void Agent::mutate(std::vector<Agent*> cur_gen, size_t idx){
	std::vector<double> donor_vec = this->mutation_operator->apply(cur_gen, idx);
	int retries = 0;
	// std::cout << "dv" << std::endl;
	// print_position(donor_vec);
	//attempt generation of donor vector within bounds
	while(check_position_oob(donor_vec) == true && retries < this->resample_limit){
		retries++;
		std::vector<double> donor_vec = this->mutation_operator->apply(cur_gen, idx);
		// print_position(donor_vec);
	}
	
	//apply correction if no donor vector within bounds could be generated
	if (check_position_oob(donor_vec) == true){
		donor_vec = boundary_correction->apply(donor_vec);
	}

	for (size_t i = 0; i < this->dim; ++i){
		this->donor[i] = donor_vec[i];
	}
}

void Agent::crossover(std::vector<Agent*> next_gen, size_t idx){
	std::vector<double> new_position = this->crossover_operator->apply(this->position, this->donor);

	next_gen[idx]->set_position(new_position);
}



//TODO: budget
void Agent::calculate_fitness() {
	// this->fitness = this->target_function(position);
	this->fitness = (*target_function)(position);
	fitness_uptodate = true;
}



double Agent::get_fitness() {
	if (fitness_uptodate == false && *budget > 0) {
		// std::cout << "calculating fitness... ";
		this->calculate_fitness();
		*budget -= 1;
	}
	return fitness;
}

std::vector<double> Agent::get_position() {
	return position;
}

CROSSOVER Agent::get_crossover(){return this->crossover_operator->get_type();}
MUTATION Agent::get_mutation(){return this->mutation_operator->get_type();}
BOUNDARY Agent::get_boundary(){return this->boundary_correction->get_type();}

void Agent::set_position(std::vector<double> new_position) {
	//if dimension fits
	if (new_position.size() == this->dim) {
		if (this->position.size() == this->dim){
			
			//redundant check for out-of-bounds coordinates TODO:remove for performance if everything works fine
			std::vector<double> corrected_position = boundary_correction->apply(new_position);
			if (corrected_position != new_position){
				new_position = corrected_position;
				std::cerr << "emergency correction applied - please investigate";
			}
			

			//track if new position is different from current position
			bool change = false;
			
			//update position
			for (size_t i = 0; i < this->dim; ++i){
				if (this->position[i] != new_position[i]){
					this->position[i] = new_position[i];
					change = true;					
				}
			}

			//tell agent to update fitness if position has changed
			if (change){
				fitness_uptodate = false;
			}
		}else{
			//would be very improbable
			std::cerr << "incorrect dimensions current position\n";
		}
	}else{
		std::cerr << "incorrect dimensions new position\n";
	}
}

bool Agent::check_position_oob(const std::vector<double>& given_position){
	//std::cout << "gp" << given_position.size() << " " << this->dim << std::endl;
	for (size_t i = 0; i < this->dim; ++i){
		if (given_position[i] < this->target_function->bounds().lb[i] || given_position[i] > this->target_function->bounds().ub[i]){
			return true;
		}
	}
	return false;
}

std::tuple<std::vector<double>, double, Crossover*, std::shared_ptr<Mutation>, Boundary*> Agent::update_history(){
	std::tuple<std::vector<double>, double, Crossover*, std::shared_ptr<Mutation>, Boundary*> t;
	if (fitness_uptodate){
		t = std::make_tuple(this->position, this->fitness, this->crossover_operator, this->mutation_operator, this->boundary_correction);
		this->history.push_back(t);
		
	}else{
		std::cerr << "Hey, the fitness isn't up to date. Why?" << std::endl;
	}	
	return t;
}

void Agent::add_history(std::tuple<std::vector<double>, double, Crossover*, std::shared_ptr<Mutation>, Boundary*> snapshot){
	history.push_back(snapshot);
}

void Agent::print_history(bool print_positions){
	for (auto snapshot : this->history){
		std::string hist_string= "";
		const auto [pos, fitness, CrOpPtr, MutOpPtr, BoundOpPtr] = snapshot;
		
		// print position
		if (print_positions){
			for(double i : pos){
			std::cout << i <<',';
			}
		}
				
		//print outher information
		std::cout << " " << fitness << " " << MutOpPtr->get_F() << " " << CrOpPtr->get_Cr() << " " 
					<< CROSSOVER_NAMES[CrOpPtr->get_type()] << " " << MUTATION_NAMES[MutOpPtr->get_type()] << " " << BOUNDARY_NAMES[BoundOpPtr->get_type()] << std::endl;
		
		std::cout << hist_string;
	}
}
#include "../include/crossover.h"


CROSSOVER Binomial::get_type() {
  return BINOMIAL;
}

// Apply crossover on all agents, returns selection
std::vector<double> Binomial::apply(const std::vector<double>& cur_pos, const std::vector<double>& donor_vec){
	// std::cout << "co apply dim: " << this->dim << std::endl;

	std::vector<double> new_pos;
	new_pos.reserve(this->dim);
	
	// pick randomly from base or donor vector based on Cr
	unsigned int crossover_point = tools.rand_uint_unif(0,this->dim);
	for (size_t i = 0; i < this->dim; ++i){
		float chance = tools.rand_double_unif(0.0, 1.0);
		if ((chance <= Cr) || (i == crossover_point))		{
			new_pos.push_back(donor_vec[i]);
		}else{
			new_pos.push_back(cur_pos[i]);
		}
	}

	return new_pos;
}

// Exponential
CROSSOVER Exponential::get_type() {
	return EXPONENTIAL;
}

// Apply crossover on all agents, returns selection
std::vector<double> Exponential::apply(const std::vector<double>& cur_pos, const std::vector<double>& donor_vec){
	// initialize donor vector from base vector
	std::vector<double> new_pos = cur_pos;
	new_pos.reserve(this->dim);

	// set crossover point
	unsigned int crossover_point = tools.rand_uint_unif(0,this->dim);
	// crossover point is always from donor
	new_pos[crossover_point] = donor_vec[crossover_point];
	unsigned int current_index = crossover_point+1;
	
	// Replace indexes with donor vector until looped trough entire vector
	// or until a chance based on Cr is reched
	while (current_index % this->dim < crossover_point){
		if (tools.rand_double_unif(0.0,1.0) <= this->Cr){
			new_pos[current_index % this->dim] = donor_vec[current_index % this->dim];
		}else{
			break;
		}
		current_index++;
	}


	return new_pos;
}


#include "../include/crossover.h"

// Crossover 
//TODO:remove?
Crossover::Crossover(size_t dim, float F) {
	this->F = F; 
	this->dim = dim;
}

// Crossover::~Crossover(){

// }

// Randdiv1
Binomial::Binomial(size_t dim, float F) {
	this->dim = dim;
	this->F = F;
}


CROSSOVER Binomial::get_type() {
  return BINOMIAL;
}

// Apply crossover on all agents, returns selection
std::vector<double> Binomial::apply(std::vector<double> cur_pos, std::vector<double> donor_vec){
	//TODO:sanity check, are cur_pos and donor vec same dim as 'n'
	for (size_t i = 0; i < dim; ++i){
		//TODO: look at rand generation
		float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (chance < F)		{
			cur_pos[i] = donor_vec[i];
		}
	}
	return cur_pos;
}




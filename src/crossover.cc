#include "../include/crossover.h"



// Crossover 
//TODO:remove?
Crossover::Crossover(size_t dim, float F) : dim(dim), F(F){
	// this->F = F; 
	// this->dim = dim;
}

// Crossover::~Crossover(){

// }

// Randdiv1
Binomial::Binomial(size_t dim, float F) : Crossover(dim, F){
	// this->dim = dim;
	// this->F = F;
}


CROSSOVER Binomial::get_type() {
  return BINOMIAL;
}

// Apply crossover on all agents, returns selection
std::vector<double> Binomial::apply(std::vector<double> cur_pos, std::vector<double> donor_vec){
	// std::cout << "co apply dim: " << this->dim << std::endl;

	std::vector<double> new_pos;
	new_pos.reserve(this->dim);

	//TODO:sanity check, are cur_pos and donor vec same dim as 'n'
	if (cur_pos.size() != this->dim){
		std::cerr << "co apply invalid posiiton size\n";
	}
	if (donor_vec.size() != this->dim){
		std::cerr << "co apply invalid donor size\n";
	}

	for (size_t i = 0; i < this->dim; ++i){
		//TODO: look at rand generation
		float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (chance < F)		{
			// std::cout << "donated position " << i << std::endl;
			new_pos.push_back(donor_vec[i]);
		}else{
			new_pos.push_back(cur_pos[i]);
		}
	}
	if (cur_pos.size() != this->dim){
		std::cerr << "co apply invalid posiiton size\n";
	}
	if (donor_vec.size() != this->dim){
		std::cerr << "co apply invalid donor size\n";
	}if (new_pos.size() != this->dim){
		std::cerr << "co apply invalid posiiton size\n";
	}
	// std::cout << "co apply pos: " << new_pos.size() << " dim: " << this->dim << std::endl;
	return new_pos;
}




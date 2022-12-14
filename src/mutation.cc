#include "../include/mutation.h"

// Mutation 
//TODO:remove?
Mutation::Mutation(size_t n, float F) {
	this->F = F; 
	this->n = n;
}

// Randdiv1
Randdiv1::Randdiv1(size_t n, float F = 0.2) {
	Randdiv1::n = n;
	Randdiv1::F = F;
}

MUTATION Randdiv1::get_type() {
  return RANDDIV1;
}

// Apply mutation on all agents, returns selection
void Randdiv1::apply(std::vector<std::shared_ptr<Agent>> next_gen){
	std::default_random_engine generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

	for (size_t i = 0; i < this->n; i++) {
		//select 3 uniform random vectors
		do {
			x1 = distribution(generator);
		} while (x1 == i);		
		do {
			x2 = distribution(generator);
		} while (x2 == x1 || x2 == i);
		do {
			x3 = distribution(generator);
		} while (x3 == x2 || x3 == x1 || x3 == i);

		std::vector<double> donor_vec;
		donor_vec.reserve(this->n);
		for (size_t j = 0; j < this->n; j++) {
			donor_vec.push_back(next_gen[j]->get_position()[j] + this->F * (next_gen[x2]->get_position()[j] - next_gen[x3]->get_position()[j]));
		}
		next_gen[i]->set_position(donor_vec);
	}
}




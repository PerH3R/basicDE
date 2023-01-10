#include "../include/mutation.h"

// Mutation 
//TODO:remove?
Mutation::Mutation(size_t n, float F) {
	this->F = F; 
	this->n = n;
}

// Randdiv1
Randdiv1::Randdiv1(size_t n, float F) {
	Randdiv1::n = n;
	Randdiv1::F = F;
}


MUTATION Randdiv1::get_type() {
  return RANDDIV1;
}

// Apply mutation on an agent, returns new position
std::vector<double> Randdiv1::apply(std::vector<std::shared_ptr<Agent>> cur_gen, size_t idx){
	std::default_random_engine generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

	// for (size_t i = 0; i < this->n; i++) {

 	//select 3 uniform random vectors
	do {
		x1 = distribution(generator);
	} while (x1 == idx);		
	do {
		x2 = distribution(generator);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = distribution(generator);
	} while (x3 == x2 || x3 == x1 || x3 == idx);

	std::vector<double> donor_vec;
	donor_vec.reserve(this->n);
	for (size_t j = 0; j < this->n; j++) {
		donor_vec.push_back(cur_gen[j]->get_position()[j] + this->F * (cur_gen[x2]->get_position()[j] - cur_gen[x3]->get_position()[j]));
	}
	return donor_vec;

	//}
}




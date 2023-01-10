#include "../include/mutation.h"

// Mutation 
//TODO:remove?
Mutation::Mutation(size_t dim, size_t n, float F) {
	this->F = F; 
	this->n = n;
	this->dim = dim;
}

// Randdiv1
Randdiv1::Randdiv1(size_t dim, size_t n, float F) {
	this->n = n;
	this->F = F;
	this->dim = dim;
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

	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;

	//}
}




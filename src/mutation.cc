#include "../include/mutation.h"

//TODO:remove
#include <iostream>

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
std::vector<double> Randdiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
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
	// for (double p : donor_vec){
	// 	std::cout << p << " ";
	// }
	// std::cout << std::endl;

	// std::cout << idx << " " << x1 << " " << x2 << " " << x3 << std::endl;

	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;

	//}
}



// Bestdiv1
Bestdiv1::Bestdiv1(size_t dim, size_t n, float F) {
	this->n = n;
	this->F = F;
	this->dim = dim;
}


MUTATION Bestdiv1::get_type() {
  return BESTDIV1;
}

// Apply mutation on an agent, returns new position
std::vector<double> Bestdiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = 0;
	} while (x1 == idx);		
	do {
		x2 = distribution(generator);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = distribution(generator);
	} while (x3 == x2 || x3 == x1 || x3 == idx);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;
	
}


// TargetToPBestDiv1
TargetToPBestDiv1::TargetToPBestDiv1(size_t dim, size_t n, float F) {
	this->n = n;
	this->F = F;
	this->dim = dim;
}


MUTATION TargetToPBestDiv1::get_type() {
  return TARGETTOPBESTDIV1;
}

// Apply mutation on an agent, returns new position
std::vector<double> TargetToPBestDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine float_generator;
	std::uniform_real_distribution<float> float_distribution(0, 1);
	float p = float_distribution(float_generator);

	std::default_random_engine int_generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = idx;
	} while (x1 == idx);		
	do {
		x2 = distribution(int_generator);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = distribution(int_generator);
	} while (x3 == x2 || x3 == x1 || x3 == idx);


	//TODO
	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;
	
}
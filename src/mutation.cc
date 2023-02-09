#include "../include/mutation.h"

//TODO:remove
#include <iostream>

// Mutation 
//TODO:remove?
Mutation::Mutation(size_t dim, size_t n, float F, bool archive) : dim(dim), n(n), F(F), archive(archive){
	
}

bool Mutation::use_archive(){
	return archive;
}

// Randdiv1
Randdiv1::Randdiv1(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION Randdiv1::get_type() {
  return RANDDIV1;
}

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
Bestdiv1::Bestdiv1(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION Bestdiv1::get_type() {
  return BESTDIV1;
}

std::vector<double> Bestdiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = 0;
	} while (false);		
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


// Target To PBest Div 1
TargetToPBestDiv1::TargetToPBestDiv1(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION TargetToPBestDiv1::get_type() {
  return TTPBESTDIV1;
}


//TODO: add archive, fix p sampling
std::vector<double> TargetToPBestDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine float_generator;
	std::uniform_real_distribution<float> float_distribution(0.0, 1.0);
	float p;
	do{
		p = float_distribution(float_generator);
	}while(p<(2.0/this->n));
	

	std::default_random_engine int_generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, lower index should be better
		x1 = distribution(int_generator);
	} while (x1 == idx || x1 > (p*n));		
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
		double self = cur_gen[idx]->get_position()[j];
		double p = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = self + (this->F * (p - self)) + (this->F * (b - c));
	}
	return donor_vec;
	
}

// Target To Best Div 2
TargetToBestDiv2::TargetToBestDiv2(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION TargetToBestDiv2::get_type() {
  return TTBESTDIV2;
}


std::vector<double> TargetToBestDiv2::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	//from 1 to n because we take always include index 0
	std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

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
	do {
		x4 = distribution(generator);
	} while (x4 == x3|| x4 == x2 || x4 == x1 || x4 == idx);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double best = cur_gen[0]->get_position()[j];
		double self = cur_gen[idx]->get_position()[j];
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		double d = cur_gen[x4]->get_position()[j];
		donor_vec[j] = self + (this->F * (best - self)) + (this->F * (a - b)) + (this->F * (c - d));
	}
	return donor_vec;
	
}


//Target To Rand Div 1
TargetToRandDiv2::TargetToRandDiv2(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}

MUTATION TargetToRandDiv2::get_type() {
  return TTRANDDIV2;
}

std::vector<double> TargetToRandDiv2::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	//from 1 to n because we take always include index 0
	std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

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
	do {
		x4 = distribution(generator);
	} while (x4 == x3|| x4 == x2 || x4 == x1 || x4 == idx);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double self = cur_gen[idx]->get_position()[j];
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		double d = cur_gen[x4]->get_position()[j];
		donor_vec[j] = self + (this->F * (a - b)) + (this->F * (c - d));
	}
	return donor_vec;
	
}

//2-OptDiv1
TwoOptDiv1::TwoOptDiv1(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION TwoOptDiv1::get_type() {
  return TWOOPTDIV1;
}


std::vector<double> TwoOptDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	//from 1 to n because we take always include index 0
	std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = distribution(generator);
	} while (x1 == idx);		
	do {
		x2 = distribution(generator);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = distribution(generator);
	} while (x3 == x2 || x3 == x1 || x3 == idx);

	//use the fittest vector as base for donor
	if (cur_gen[x2]->get_fitness() < cur_gen[x1]->get_fitness()){
		std::swap(x1, x2);
	}

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double c = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + (this->F * (b - c));
	}
	return donor_vec;
	
}

// Desmu (stochastic levy-flight)
Desmu::Desmu(size_t dim, size_t n, float F, bool archive) : Mutation(dim, n, F, archive){
	
}


MUTATION Desmu::get_type() {
  return DESMU;
}

std::vector<double> Desmu::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = distribution(generator);
	} while (false);		
	do {
		x2 = distribution(generator);
	} while (x2 == x1);
	
	x3 = 0;

	std::default_random_engine float_generator;
	std::uniform_real_distribution<float> float_distribution(0.0, 1.0);
	float scale_factor;

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		scale_factor = float_distribution(float_generator);
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double best = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + (this->F * scale_factor) * (best - b);
	}
	return donor_vec;
	
}
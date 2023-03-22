#include "../include/mutation.h"

// RandDiv1

std::vector<double> RandDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	// std::uniform_int_distribution<size_t> distribution(0, this->n-1);
	// generator.seed(time(NULL));
	int x1, x2, x3;

	// for (size_t i = 0; i < this->n; i++) {

 	//select 3 uniform random vectors
	do {
		x1 = tools.rand_int_unif(0, this->n);
		// x1 = distribution(generator);
	} while (x1 == idx);		
	do {
		x2 = tools.rand_int_unif(0, this->n);
		// x2 = distribution(generator);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(0, this->n);
		// x3 = distribution(generator);
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



// BestDiv1
std::vector<double> BestDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	// std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = 0;
	} while (false);		
	do {
		x2 = tools.rand_int_unif(0, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(0, this->n);
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
//TODO: add archive, fix p sampling
std::vector<double> TargetToPBestDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	std::default_random_engine float_generator;
	std::uniform_real_distribution<float> float_distribution(0.0, 1.0);
	float p;
	do{
		p = tools.rand_double_unif(0.0,1.0);
	}while(p<(2.0/this->n));
	

	std::default_random_engine int_generator;
	std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, lower index should be better
		x1 = tools.rand_int_unif(0, this->n);
	} while (x1 == idx || x1 > (p*n));		
	do {
		x2 = tools.rand_int_unif(0, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(0, this->n);
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

bool TargetToPBestDiv1::use_archive(){
	return archive;
}

// Target To Best Div 2
std::vector<double> TargetToBestDiv2::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	//from 1 to n because we take always include index 0
	// std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = 0;
	} while (x1 == idx);		
	do {
		x2 = tools.rand_int_unif(1, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(1, this->n);
	} while (x3 == x2 || x3 == x1 || x3 == idx);
	do {
		x4 = tools.rand_int_unif(1, this->n);
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


//Target To Rand Div 2
std::vector<double> TargetToRandDiv2::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	//from 1 to n because we take always include index 0
	// std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = 0;
	} while (x1 == idx);		
	do {
		x2 = tools.rand_int_unif(1, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(1, this->n);
	} while (x3 == x2 || x3 == x1 || x3 == idx);
	do {
		x4 = tools.rand_int_unif(1, this->n);
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
std::vector<double> TwoOptDiv1::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	// //from 1 to n because we take always include index 0
	// std::uniform_int_distribution<size_t> distribution(1, this->n);
	size_t x1, x2, x3, x4;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = tools.rand_int_unif(1, this->n);
	} while (x1 == idx);		
	do {
		x2 = tools.rand_int_unif(1, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(1, this->n);
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
//TODO
std::vector<double> Desmu::apply(std::vector<Agent*> cur_gen, size_t idx){
	// std::default_random_engine generator;
	// std::uniform_int_distribution<size_t> distribution(0, this->n);
	size_t x1, x2, x3;

 	//select 3 uniform random vectors
	do {
		//provided population is sorted, x1 should be optimal
		x1 = tools.rand_int_unif(1, this->n);
	} while (false);		
	do {
		x2 = tools.rand_int_unif(1, this->n);
	} while (x2 == x1);
	
	x3 = 0;



	// std::default_random_engine float_generator;
	// std::uniform_real_distribution<float> float_distribution(0.0, 1.0);
	float scale_factor;

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		scale_factor = tools.rand_double_unif(0.0,1.0);
		double a = cur_gen[x1]->get_position()[j];
		double b = cur_gen[x2]->get_position()[j];
		double best = cur_gen[x3]->get_position()[j];
		donor_vec[j] = a + (this->F * scale_factor) * (best - b);
	}
	return donor_vec;
	
}

//Bea
void Bea::mutate_segment(std::vector<Agent*> cur_gen, size_t idx, int x1, int x2, int x3,
		std::vector< std::vector<double> >& clones, std::vector<double>& fitness, int start_gene, int end_gene){

	//change segment using Bea or TTR/1
	for (int i = 0; i < clones.size(); ++i){
		if(*budget > 0){
			if (i == 0 && tools.rand_double_unif(0.0,1.0) < this->Pbea){
				//Bea
				for (size_t j = start_gene; j < end_gene; j++) {
					double self = clones[i][j];
					clones[i][j] = self + tools.rand_double_unif(target_function->bounds().lb[j], target_function->bounds().ub[j]);
				}
			} else {
				//calculate segment using TTR/1
				for (size_t j = start_gene; j < end_gene; j++) {
					double self = clones[i][j];
					double a = cur_gen[x1]->get_position()[j];
					double b = cur_gen[x2]->get_position()[j];
					double c = cur_gen[x3]->get_position()[j];
					clones[i][j] = self + (this->F * (c - self)) + (this->F * (a - b));
				}
			}

			//evaluate		
			fitness[i] = (*target_function)(clones[i]);
			*budget-=1;
		}
	}


	int best_clone = std::distance(fitness.begin(), std::max_element(std::begin(fitness), std::end(fitness)));

	//copy into others
	for (int i = 0; i < clones.size(); ++i){
		if(i != best_clone){
			for (size_t j = start_gene; j < end_gene; j++) {
				clones[i][j] = clones[best_clone][j];
			}
		}
	}
}

std::vector<double> Bea::apply(std::vector<Agent*> cur_gen, size_t idx){
	int remainder = this->dim % this->Nsegments;
	int remainder_settled = 0;

	std::vector< std::vector<double> > clones;
	for (int i = 0; i < Nclones; ++i){
		clones.push_back(cur_gen[idx]->get_position());
	}
	std::vector<double> fitness;
	fitness.reserve(clones.size());


	//select 3 uniform random vectors in advance for consistency TODO:ambiguous if each segment should have new random idx's
	size_t x1, x2, x3;
	do {
		x1 = tools.rand_int_unif(0, this->n);
	} while (x1 == idx);		
	do {
		x2 = tools.rand_int_unif(0, this->n);
	} while (x2 == x1 || x2 == idx);
	do {
		x3 = tools.rand_int_unif(0, this->n);
	} while (x3 == x2 || x3 == x1 || x3 == idx);

	for (int i = 0; i < Nsegments; ++i){
		if (remainder == 0){
			mutate_segment(cur_gen, idx, x1, x2, x3, clones, fitness,
							(i*segment_size) + remainder_settled, (i+1) * segment_size + remainder_settled);
		} else {
			mutate_segment(cur_gen, idx, x1, x2, x3, clones, fitness,
							(i*segment_size) + remainder_settled, (i+1) * segment_size + remainder_settled + 1);
			remainder--;
			remainder_settled++;
		}
	}

	//all data copied into every clone
	return clones[0];
	
}
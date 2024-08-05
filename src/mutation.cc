#include "../include/mutation.h"

// RandDiv1
std::vector<double> RandDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	//select 3 vectors uniformly at random 
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen, 3, false);

	std::vector<double> donor_vec(this->dim, 0.0);

	for (size_t j = 0; j < this->dim; j++) {
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;
}

//RandDiv2
std::vector<double> RandDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	//select 5 vectors uniformly at random 
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen, 5, false);

	std::vector<double> donor_vec(this->dim, 0.0);

	for (size_t j = 0; j < this->dim; j++) {
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		double d = chosen_vectors[3]->get_position()[j];
		double e = chosen_vectors[4]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c) + this->F * (d - e);
	}
	return donor_vec;
}


// BestDiv1
std::vector<double> BestDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	//exclude optimal (first because sorted) solution from random options
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 2, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[0]->get_position()[j];
		double b = chosen_vectors[0]->get_position()[j];
		double c = chosen_vectors[1]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c);
	}
	return donor_vec;	
}


// BestDiv2
std::vector<double> BestDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	//exclude optimal (first if sorted) solution from random options
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 4, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = cur_gen[0]->get_position()[j];
		double b = chosen_vectors[0]->get_position()[j];
		double c = chosen_vectors[1]->get_position()[j];		
		double d = chosen_vectors[2]->get_position()[j];
		double e = chosen_vectors[3]->get_position()[j];
		donor_vec[j] = a + this->F * (b - c) + this->F * (d - e);
	}
	return donor_vec;
	
}

// Target To PBest Div 1
std::vector<double> TargetToPBestDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	float p_val;

	//p between idx=2 and 20%	
	p_val = tools.rand_double_unif((2/this->n), this->p_ceil);


	unsigned int p_idx = tools.rand_uint_unif(0, std::round(p_val*this->n) + 1); //+1 to avoid rounding to 0 since rand_int_unif has range [low,high) 

	std::vector<std::vector<double>> temp_gen;
	temp_gen.reserve(cur_gen.size()-2 + (*archive).size());
	for (size_t i = 0; i < cur_gen.size(); ++i){
	 	if (i != p_idx && i != idx){ //exclude p and target
	 		temp_gen.push_back(cur_gen[i]->get_position());
	 	}	 	
	} 
	if (this->use_archive()){
		for (size_t i = 0; i < (*archive).size(); ++i){
			if (!(*archive)[i].empty()){
				temp_gen.push_back((*archive)[i]);
			}
		 	
		}
	}
	// std::cout << temp_gen.size() << " " << (*archive).size() << " " << cur_gen.size() << std::endl;
	//only last index (c) can be chosen from archive
	std::vector<std::vector<double>> chosen_vectors;
	do{
		chosen_vectors = tools.pick_random(temp_gen, 2, false);
	}while((this->use_archive() && std::find(archive->begin(), archive->end(), chosen_vectors[0]) != archive->end()) /*if c is from archive*/);


	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double self = cur_gen[idx]->get_position()[j];
		double p = cur_gen[p_idx]->get_position()[j];
		double b = chosen_vectors[0][j];
		double c = chosen_vectors[1][j];
		donor_vec[j] = self + (this->F * (p - self)) + (this->F * (b - c));
	}
	return donor_vec;
	
}

bool TargetToPBestDiv1::use_archive(){
	return archive_size != 0;
}

// Target To Best Div 1
std::vector<double> TargetToBestDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
 	
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	//if best and self are not the same
	if(idx != 0){
		cur_gen_ex_first.erase(cur_gen_ex_first.begin()+(idx-1));
	}
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 2, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double best = cur_gen[0]->get_position()[j];
		double self = cur_gen[idx]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		donor_vec[j] = self + (this->F * (best - self)) + (this->F * (a - b));
	}
	return donor_vec;
	
}


// Target To Best Div 2
std::vector<double> TargetToBestDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){
 	
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	//if best and self are not the same
	if(idx != 0){
		cur_gen_ex_first.erase(cur_gen_ex_first.begin()+(idx-1));
	}
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 4, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double best = cur_gen[0]->get_position()[j];
		double self = cur_gen[idx]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		double d = chosen_vectors[3]->get_position()[j];
		donor_vec[j] = self + (this->F * (best - self)) + (this->F * (a - b)) + (this->F * (c - d));
	}
	return donor_vec;
	
}

// Rand To Best Div 1
std::vector<double> RandToBestDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
 	
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	//if best and self are not the same
	if(idx != 0){
		cur_gen_ex_first.erase(cur_gen_ex_first.begin()+(idx-1));
	}
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 3, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double best = cur_gen[0]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[1]->get_position()[j];
		donor_vec[j] = a + (this->F * (best - a)) + (this->F * (b - c));
	}
	return donor_vec;
	
}


// Rand To Best Div 2
std::vector<double> RandToBestDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){
 	
	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	//if best and self are not the same
	if(idx != 0){
		cur_gen_ex_first.erase(cur_gen_ex_first.begin()+(idx-1));
	}
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 5, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double best = cur_gen[0]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		double d = chosen_vectors[3]->get_position()[j];
		double e = chosen_vectors[4]->get_position()[j];
		donor_vec[j] = a + (this->F * (best - a)) + (this->F * (b - c)) + (this->F * (d - e));
	}
	return donor_vec;	
}


//Target To Rand Div 1
std::vector<double> TargetToRandDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){

	std::vector<Agent*> cur_gen_ex_self = std::vector<Agent*>(cur_gen.begin(), cur_gen.end());
	cur_gen_ex_self.erase(cur_gen_ex_self.begin()+idx);
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_self, 3, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double self = cur_gen[idx]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		donor_vec[j] = self + (this->F * (a - self)) + (this->F * (b - c));
	}
	return donor_vec;	
}

//Target To Rand Div 2
std::vector<double> TargetToRandDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){

	std::vector<Agent*> cur_gen_ex_self = std::vector<Agent*>(cur_gen.begin(), cur_gen.end());
	cur_gen_ex_self.erase(cur_gen_ex_self.begin()+idx);
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_self, 4, false);

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double self = cur_gen[idx]->get_position()[j];
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		double d = chosen_vectors[3]->get_position()[j];
		double e = chosen_vectors[3]->get_position()[j];
		donor_vec[j] = self + (this->F * (a - self)) + (this->F * (b - c)) + (this->F * (d - e));
	}
	return donor_vec;	
}

//2-OptDiv1
std::vector<double> TwoOptDiv1::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen, 3, false);

	//use the fittest vector as base for donor
	if (chosen_vectors[1]->get_fitness() < chosen_vectors[0]->get_fitness()){
		std::swap(chosen_vectors[0], chosen_vectors[1]);
	}

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		donor_vec[j] = a + (this->F * (b - c));
	}
	return donor_vec;	
}

//2-OptDiv2
std::vector<double> TwoOptDiv2::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen, 5, false);

	//use the fittest vector as base for donor
	if (chosen_vectors[1]->get_fitness() < chosen_vectors[0]->get_fitness()){
		std::swap(chosen_vectors[0], chosen_vectors[1]);
	}

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double c = chosen_vectors[2]->get_position()[j];
		double d = chosen_vectors[3]->get_position()[j];
		double e = chosen_vectors[4]->get_position()[j];
		donor_vec[j] = a + (this->F * (b - c)) + (this->F * (d - e));
	}
	return donor_vec;	
}

// Desmu (stochastic levy-flight)
std::vector<double> Desmu::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	std::vector<Agent*> cur_gen_ex_first = std::vector<Agent*>(cur_gen.begin() + 1, cur_gen.end());
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen_ex_first, 2, false);

	//calculate stepsize
	double step = tools.rand_mantegna(this->sig_u,this->sig_v,alpha);
	//additionally scale because the paper said so
	float scale_factor = step * tools.rand_double_unif(0.5, 1.0); 

	//calculate donor vector
	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		// scale_factor = tools.rand_double_unif(0.0,1.0);
		double a = chosen_vectors[0]->get_position()[j];
		double b = chosen_vectors[1]->get_position()[j];
		double best = cur_gen[0]->get_position()[j];
		donor_vec[j] = a + (this->F * scale_factor) * (best - b);
	}
	return donor_vec;
	
}

//Bea
void Bea::mutate_segment(const std::vector<Agent*>& cur_gen, size_t idx, std::vector<Agent*> chosen_vectors,
		std::vector< std::vector<double> >& clones, std::vector<double>& fitness, unsigned int start_index, unsigned int end_index){
	(void)idx; //Suppress warning of unneeded function parameter for this operator

	//change segment using Bea or TTR/1
	for (size_t i = 0; i < clones.size(); ++i){
		if(*budget > 0){
			if (i == 0 && tools.rand_double_unif(0.0,1.0) < this->Pbea){
				//Bea
				for (size_t j = start_index; j < end_index; j++) {
					double self = clones[i][j];
					clones[i][j] = self + (this->locality * tools.rand_double_unif(target_function->bounds().lb[j], target_function->bounds().ub[j]));
				}
			} else {
				//calculate segment using TTR/1
				for (size_t j = start_index; j < end_index; j++) {
					double self = clones[i][j];
					double a = chosen_vectors[0]->get_position()[j];
					double b = chosen_vectors[1]->get_position()[j];
					double c = chosen_vectors[2]->get_position()[j];
					clones[i][j] = self + (this->F * (c - self)) + (this->F * (a - b));
				}
			}
			clones[i] = boundary_correction->apply(clones[i]); //not mentioned in paper but should probably be included if we are evaluating
			//evaluate		
			fitness[i] = (*target_function)(clones[i]);
			*budget -= 1;
		}
	}


	unsigned int best_clone = std::distance(fitness.begin(), std::max_element(std::begin(fitness), std::end(fitness)));

	//copy segment of best clone into others
	for (size_t i = 0; i < clones.size(); ++i){
		if(i != best_clone){
			for (size_t j = start_index; j < end_index; j++) {
				clones[i][j] = clones[best_clone][j];
			}
		}
	}
}

std::vector<double> Bea::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	int remainder = this->dim % this->Nsegments;
	int remainder_settled = 0;

	//reserve space for clones
	std::vector< std::vector<double> > clones;
	for (int i = 0; i < Nclones; ++i){
		clones.push_back(cur_gen[idx]->get_position());
	}
	std::vector<double> fitness;
	fitness.reserve(clones.size());


	// select 3 uniform random vectors in advance for consistency TODO:ambiguous if each segment should have new random idx's
	std::vector<Agent*> chosen_vectors = tools.pick_random(cur_gen, 3, false);

	// Divide up the vector in roughly equal segments
	for (size_t i = 0; i < Nsegments; ++i){
		if (remainder == 0){
			unsigned int start_index = (i*segment_size) + remainder_settled;
			unsigned int end_index = ((i+1) * segment_size) + remainder_settled;
			// apply mutation on just this segment
			mutate_segment(cur_gen, idx, chosen_vectors, clones, fitness, start_index, end_index);
		} else {
			unsigned int start_index = (i*segment_size) + remainder_settled + 1;
			unsigned int end_index = ((i+1) * segment_size) + remainder_settled + 1;
			// apply mutation on just this segment
			mutate_segment(cur_gen, idx, chosen_vectors, clones, fitness, start_index, end_index);
			remainder--;
			remainder_settled++;
		}
	}

	//all data is copied into every clone so we can pass the first
	return clones[0];
	
}

//DIRMUT
std::vector<double> DirMut::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	std::vector<double> donor_vec(this->dim, 0.0);
	if (this->vector_pool_ptr == NULL){
			std::cerr << "NO Vector pool found. Prepare for errors." << std::endl;
		}
		std::vector<std::vector<double>> vector_pool = *vector_pool_ptr;

	// if global improvement occured last iteration
	if(this->improved){		
		size_t r1, r2;
		// take a random base vector
		do {
			r1 = tools.rand_int_unif(0, this->n);
		} while (r1 == idx);
		// pull a random difference vector from the pool
		r2 = tools.rand_int_unif(0, vector_pool.size());
		// apply scaled difference vector to base vector
		donor_vec = tools.vec_sum(cur_gen[r1]->get_position() , tools.vec_scale(vector_pool[r2], this->F));

	} else{
		// use normal operator
		donor_vec = this->base_operator->apply(cur_gen, idx);
	}


	//clear vector pool at the end of each iteration
	if((idx < this->n) == false){
		vector_pool.clear();
		this->improved = false;
	}
	return donor_vec;
	
}

//RandomSearch
std::vector<double> RandomSearch::apply(std::vector<Agent*> const& cur_gen, size_t idx){
	(void)idx; //Suppress warning of unneeded function parameter for this operator
	(void)cur_gen; //Suppress warning of unneeded function parameter for this operator

	std::vector<double> donor_vec(this->dim, 0.0);
	for (size_t j = 0; j < this->dim; j++) {
		donor_vec[j] = tools.rand_double_unif(this->target_function->bounds().lb[j], this->target_function->bounds().ub[j]);
	}
	return donor_vec;	
}

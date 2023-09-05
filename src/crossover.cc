#include "../include/crossover.h"


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
	
	int crossover_point = tools.rand_int_unif(0,this->dim);
	//TODO: add the fixed variable
	for (size_t i = 0; i < this->dim; ++i){
		//TODO: look at rand generation
		float chance = tools.rand_double_unif(0.0, 1.0);
		if ( (chance <= Cr)||(i == crossover_point) )		{
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

// Exponential
CROSSOVER Exponential::get_type() {
  return EXPONENTIAL;
}

// Apply crossover on all agents, returns selection
std::vector<double> Exponential::apply(const std::vector<double> cur_pos, std::vector<double> donor_vec){
	// std::cout << "co apply dim: " << this->dim << std::endl;

	std::vector<double> new_pos = cur_pos;
	new_pos.reserve(this->dim);

	//TODO:sanity check, are cur_pos and donor vec same dim as 'n'
	if (cur_pos.size() != this->dim){
		std::cerr << "co apply invalid posiiton size\n";
	}
	if (donor_vec.size() != this->dim){
		std::cerr << "co apply invalid donor size\n";
	}

	int crossover_point = tools.rand_int_unif(0,this->dim);
	new_pos[crossover_point] = donor_vec[crossover_point];
	int current_index = crossover_point+1;
	// bool returned = false;

	while (current_index % this->dim < crossover_point){
		if (tools.rand_double_unif(0.0,1.0) <= this->Cr){
			new_pos[current_index] = donor_vec[current_index];
		}else{
			break;
		}
		current_index++;
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


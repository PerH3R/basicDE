#include "../include/tools.h"

void Tools::set_seed(int seed){
	if(seed==-1){
		auto current_time = time(NULL);
		rng.seed(current_time);
	}else{
		rng.seed(seed);
	}
}

template <typename T>
double Tools::get_euclidean_distance(std::vector<T> pos1,
							  std::vector<T> pos2){
	//TODO

	return -1.0;
}

double Tools::rand_double_unif(double const begin, double const end){
	std::uniform_real_distribution<double> dist(begin, end);
	return dist(rng);
}

int Tools::rand_int_unif(int const begin, int const end){
	std::uniform_int_distribution<int> dist(begin, end-1);
	return dist(rng);
}
			
double Tools::rand_double_norm(double const mean, double const stdDev){
	std::normal_distribution<double> N(mean, stdDev);
	return N(rng);
}

Tools tools;
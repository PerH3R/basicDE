#include "../include/tools.h"

void Tools::set_seed(int seed){
	if(seed==-1){
		auto current_time = time(NULL);
		rng.seed(current_time);
	}else{
		rng.seed(seed);
	}
}


double Tools::rand_double_unif(double const begin, double const end){
	std::uniform_real_distribution<double> dist(begin, end);
	return dist(rng);
}

int Tools::rand_int_unif(int const begin, int const end){
	std::uniform_int_distribution<int> dist(begin, end-1); // for [begin, end) dist
	return dist(rng);
}
			
double Tools::rand_double_norm(double const mean, double const stdDev){
	std::normal_distribution<double> N(mean, stdDev);
	return N(rng);
}

double Tools::rand_mantegna(double const sig_u, double const sig_v, double const alpha){
	double u_dist = this->rand_double_unif(0, sig_u);
	double v_dist = this->rand_double_unif(0, sig_v);

	return u_dist / std::pow(std::abs(v_dist), (1/alpha));
}

std::vector<double> Tools::vec_sub(std::vector<double> first, std::vector<double> second){
	if (first.size() == second.size()){
		std::vector<double> result;
		for (int i = 0; i < first.size(); ++i){
		 	result.push_back(first[i]-second[i]);
		}
		return result; 
	}else{
		std::cerr << "ERROR: Dimensions don't match. Provided vectors have dimensions " << first.size() << " and " << second.size(); 
		return first;
	}
}

std::vector<double> Tools::vec_scale(std::vector<double> vec, double scalar){
	std::vector<double> result;
	for (int i = 0; i < vec.size(); ++i){
	 	result.push_back(vec[i]*scalar);
	}
	return result; 
}

std::vector<double> Tools::vec_sum(std::vector<double> first, std::vector<double> second){
	if (first.size() == second.size()){
		std::vector<double> result;
		for (int i = 0; i < first.size(); ++i){
		 	result.push_back(first[i]+second[i]);
		}
		return result; 
	}else{
		std::cerr << "ERROR: Dimensions don't match. Provided vectors have dimensions " << first.size() << " and " << second.size(); 
		return first;
	}
}

Tools tools;
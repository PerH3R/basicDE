#pragma once

//#include <thread>
#include <vector>
#include <memory>
#include <random>
#include "time.h"

//TODO
#include<iostream>

class Tools{
public:
	Tools(){ 
		auto current_time = time(NULL);
		std::srand(current_time);
	};

	void set_seed(int seed=-1);

	template <typename T>
	double get_euclidean_distance(std::vector<T> pos1,
								  std::vector<T> pos2);

	double rand_double_unif(double const begin, double const end);

	int rand_int_unif(int const start, int const end);
			
	double rand_double_norm(double const mean, double const stdDev);


private:
	std::mt19937 rng;
	std::default_random_engine dev;

};

extern Tools tools;
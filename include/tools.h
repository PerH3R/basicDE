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

	double rand_double_unif(double const begin, double const end);

	// [start, end)
	int rand_int_unif(int const start, int const end);
			
	double rand_double_norm(double const mean, double const stdDev);

	//calculate difference in position
	std::vector<double> vec_sub(std::vector<double> first, std::vector<double> second);


	std::vector<double> vec_scale(std::vector<double> vec, double scalar);


	std::vector<double> vec_sum(std::vector<double> first, std::vector<double> second);


private:
	std::mt19937 rng;
	std::default_random_engine dev;

};

extern Tools tools;
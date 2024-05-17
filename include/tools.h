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

	double rand_double_unif(double const& begin, double const& end);

	// [start, end)
	int rand_int_unif(int const& begin, int const& end);
			
	double rand_double_norm(double const& mean, double const& stdDev);

	double rand_mantegna(double const& sig_u, double const& sig_v, double const& alpha);

	bool extract_bit(double value, int idx){
		return (value >> idx) & 1
	};

	//calculate difference in position
	std::vector<double> vec_sub(std::vector<double> const& first, std::vector<double> const& second);


	std::vector<double> vec_scale(std::vector<double> const& vec, double const& scalar);


	std::vector<double> vec_sum(std::vector<double> const& first, std::vector<double> const& second);

	template<typename T>
	double vec_avg(std::vector<T> const& first){
		double total = 0;
		for (auto i : first){
			total += i;
		}
		return (total / first.size());
	}

	//taken from github->rickboks/auto-DE
	template<typename T>
	std::vector<T> pick_random(std::vector<T> possibilities, int const n, bool const replace){
		std::vector<T> picked;
		picked.reserve(n);
		for (int i = 0; i < n; i++){
			int const r = this->rand_int_unif(0,possibilities.size()-1);
			T const x = possibilities[r];
			if (!replace)
				possibilities.erase(possibilities.begin() + r);
			picked.push_back(x);
		}
		return picked;
	}


private:
	std::mt19937 rng;
	std::default_random_engine dev;

};

extern Tools tools;
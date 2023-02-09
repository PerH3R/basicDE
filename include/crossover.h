#pragma once

// #include "agent.h"
#include <random>

//TODO: remove
#include <iostream>
// class Agent;

enum CROSSOVER {
	BINOMIAL
};

class Crossover {
public:
	Crossover() = default;
    Crossover(size_t dim, float Cr = 0.2);
	virtual ~Crossover() { };
    virtual CROSSOVER get_type() = 0;
    virtual std::vector<double> apply(std::vector<double> cur_pos, std::vector<double> donor_vec) = 0;

protected:
    const size_t dim;
    const float Cr;
};

class Binomial : public Crossover {
public:
    Binomial(size_t dim, float Cr = 0.2);// = default();
    ~Binomial(){};
    CROSSOVER get_type();
    std::vector<double> apply(std::vector<double> cur_pos, std::vector<double> donor_vec);
};

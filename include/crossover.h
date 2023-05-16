#pragma once

// #include "agent.h"
#include <random>
#include "tools.h"
#include "operators.h"

//TODO: remove
#include <iostream>
// class Agent;



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


class Exponential : public Crossover {
public:
    Exponential(size_t dim, float Cr = 0.2);// = default();
    ~Exponential(){};
    CROSSOVER get_type();
    std::vector<double> apply(std::vector<double> cur_pos, std::vector<double> donor_vec);
};

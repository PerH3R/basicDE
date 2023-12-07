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
    Crossover(size_t dim, float Cr = 0.2) : dim(dim), Cr(Cr){ };
	virtual ~Crossover() { };
    virtual CROSSOVER get_type() = 0;
    virtual std::vector<double> apply(const std::vector<double> cur_pos, const std::vector<double> donor_vec) = 0;

    float get_Cr(){return this->Cr;}
    void set_Cr(float new_Cr){this->Cr = new_Cr;}

protected:
    const size_t dim;
    float Cr;
};

class Binomial : public Crossover {
public:
    Binomial(size_t dim, float Cr = 0.2) : Crossover(dim, Cr){ };// = default();
    ~Binomial(){};
    CROSSOVER get_type();
    std::vector<double> apply(const std::vector<double> cur_pos, const std::vector<double> donor_vec);
};


class Exponential : public Crossover {
public:
    Exponential(size_t dim, float Cr = 0.2) : Crossover(dim, Cr){ };// = default();
    ~Exponential(){};
    CROSSOVER get_type();
    std::vector<double> apply(const std::vector<double> cur_pos, const std::vector<double> donor_vec);
};

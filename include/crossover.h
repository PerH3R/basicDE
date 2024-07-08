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
    virtual std::vector<double> apply(const std::vector<double>& cur_pos, const std::vector<double>& donor_vec) = 0;

    // Allow for changing of crossover rate
    float get_Cr(){return this->Cr;}
    void set_Cr(float new_Cr){this->Cr = new_Cr;}

protected:
    const size_t dim;
    float Cr;
};


// Binomial crossover - Each index is randomly picked based on Cr from the respective index of either the base vector or the donor vector
class Binomial : public Crossover {
public:
    Binomial(size_t dim, float Cr = 0.2) : Crossover(dim, Cr){ };// = default();
    ~Binomial(){};
    CROSSOVER get_type() override;
    std::vector<double> apply(const std::vector<double>& cur_pos, const std::vector<double>& donor_vec) override;
};

// Expontential crossover - A random start index is chosen. That index and each subsequent are 
// taken from donor vector until Prand > Cr happens. All other indexes are from base vector.
class Exponential : public Crossover {
public:
    Exponential(size_t dim, float Cr = 0.2) : Crossover(dim, Cr){ };// = default();
    ~Exponential(){};
    CROSSOVER get_type() override;
    std::vector<double> apply(const std::vector<double>& cur_pos, const std::vector<double>& donor_vec) override;
};

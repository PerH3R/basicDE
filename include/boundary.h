#pragma once

#include <random>

#include "ioh.hpp"
#include "operators.h"
#include "tools.h"



class Boundary {
public:
	Boundary() = default;
    Boundary(ioh::problem::RealSingleObjective* target_function);
	virtual ~Boundary() { };
    virtual BOUNDARY get_type() = 0;
    virtual std::vector<double> apply(std::vector<double> position) = 0;

protected:
    ioh::problem::RealSingleObjective* target_function;

};

// Receives a possibly o.o.b. position and return an in-bounds position on the edge of the search space
class Clamp : public Boundary {
public:
    Clamp (ioh::problem::RealSingleObjective* target_function) : Boundary(target_function) {};
    ~Clamp(){};
    BOUNDARY get_type(){return CLAMP;};
    std::vector<double> apply(std::vector<double> pos);
};


// Receives a possibly o.o.b. position and return an in-bounds position that is a reflection with respect to the search space boundary
class Reflect : public Boundary {
public:
    Reflect (ioh::problem::RealSingleObjective* target_function): Boundary(target_function) {};
    ~Reflect(){};
    BOUNDARY get_type(){return REFLECT;};
    std::vector<double> apply(std::vector<double> pos);
};


// Receives a possibly o.o.b. position and returns the o.o.b. aspects randomly initialized within bounds
class Reinit : public Boundary {
public:
    Reinit (ioh::problem::RealSingleObjective* target_function): Boundary(target_function) {};
    ~Reinit(){};
    BOUNDARY get_type(){return REINIT;};
    std::vector<double> apply(std::vector<double> pos);
};

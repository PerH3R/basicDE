#pragma once

#include <random>

#include "ioh.hpp"
#include "operators.h"

//TODO: remove
#include <iostream>



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

class Clamp : public Boundary {
public:
    Clamp (ioh::problem::RealSingleObjective* target_function) : Boundary(target_function) {};
    ~Clamp(){};
    BOUNDARY get_type(){return CLAMP;};
    std::vector<double> apply(std::vector<double> next_gen);
};

class Reflect : public Boundary {
public:
    Reflect (ioh::problem::RealSingleObjective* target_function): Boundary(target_function) {};
    ~Reflect(){};
    BOUNDARY get_type(){return REFLECT;};
    std::vector<double> apply(std::vector<double> next_gen);
};

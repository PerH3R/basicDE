#pragma once

#include <random>

#include "ioh.hpp"

//TODO: remove
#include <iostream>

enum BOUNDARY {
	CLAMP
};

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
    Clamp (ioh::problem::RealSingleObjective* target_function);// = default();
    ~Clamp(){};
    BOUNDARY get_type();
    std::vector<double> apply(std::vector<double> next_gen);
};

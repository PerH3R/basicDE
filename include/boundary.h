#pragma once

#include "agent.h"
#include <random>

//TODO: remove
#include <iostream>
class Agent;

enum BOUNDARY {
	CLAMP
};

class Boundary {
public:
	Boundary() = default;
    Boundary(ioh::problem::bbob::Sphere* target_function);
	virtual ~Boundary() { };
    virtual BOUNDARY get_type() = 0;
    virtual void apply(std::vector<Agent*> next_gen) = 0;

protected:
    ioh::problem::bbob::Sphere* target_function;

};

class Clamp : public Boundary {
public:
    Clamp (ioh::problem::bbob::Sphere* target_function);// = default();
    ~Clamp(){};
    BOUNDARY get_type();
    void apply(std::vector<Agent*> next_gen);
};

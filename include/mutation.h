#pragma once

#include "tools.h"
#include <random>
#include "population.h"

enum MUTATION {
	RANDDIV1
};

class Mutation {
public:
	Mutation() = default;
	Mutation(size_t n, float F = 0.2);
	virtual ~Mutation() {};
	virtual void apply(std::vector<Agent*> next_gen) = 0;

protected:
	float F; //mutation rate
	size_t n; //pop size
};

class Randdiv1 : public Mutation {
public:
    Randdiv1(size_t n, float F = 0.2);
    MUTATION get_type();
	void apply(std::vector<Agent*> next_gen);

};



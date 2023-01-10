#pragma once

#include "tools.h"
#include <random>
#include "agent.h"
// #include "boundary.h"

//TODO: think about this forward declaration
class Agent;

enum MUTATION {
	RANDDIV1
};


class Mutation {
public:
	Mutation() = default;
	Mutation(size_t n, float F = 0.2);
	virtual ~Mutation() { };
	virtual std::vector<double> apply(std::vector<std::shared_ptr<Agent>> cur_gen, size_t idx) = 0;

protected:
	float F; //mutation rate
	size_t n; //pop size
};

class Randdiv1 : public Mutation {
public:
    Randdiv1(size_t n, float F = 0.2);
    ~Randdiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<std::shared_ptr<Agent>> cur_gen, size_t idx);
};



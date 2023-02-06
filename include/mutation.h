#pragma once

#include "tools.h"
#include "agent.h"

//TODO: think about this forward declaration
class Agent;

enum MUTATION {
	RANDDIV1,
	BESTDIV1,
	TARGETTOPBESTDIV1
};


class Mutation {
public:
	Mutation() = default;
	Mutation(size_t dim, size_t n, float F = 0.2);
	virtual ~Mutation() { };
	virtual MUTATION get_type() = 0;
	virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx) = 0;

protected:
	float F; //mutation rate
	size_t n; //pop size
	size_t dim;
};

class Randdiv1 : public Mutation {
public:
    Randdiv1(size_t dim, size_t n, float F = 0.2);
    ~Randdiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class Bestdiv1 : public Mutation {
public:
    Bestdiv1(size_t dim, size_t n, float F = 0.2);
    ~Bestdiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};
class TargetToPBestDiv1 : public Mutation {
public:
    TargetToPBestDiv1(size_t dim, size_t n, float F = 0.2);
    ~TargetToPBestDiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};
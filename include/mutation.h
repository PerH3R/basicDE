#pragma once

#include "tools.h"
#include "agent.h"

//TODO: think about this forward declaration
class Agent;

enum MUTATION {
	RANDDIV1,
	BESTDIV1,
	TTPBESTDIV1,
	TTBESTDIV2,
	TTRANDDIV2,
	TWOOPTDIV1,
	DESMU
};


class Mutation {
public:
	Mutation() = default;
	Mutation(size_t dim, size_t n, float F = 0.2, bool archive=false);
	bool use_archive();
	virtual ~Mutation() { };
	virtual MUTATION get_type() = 0;
	virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx) = 0;

protected:
	float F; //mutation rate
	size_t n; //pop size
	size_t dim;
	bool archive;
};

class Randdiv1 : public Mutation {
public:
    Randdiv1(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~Randdiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class Bestdiv1 : public Mutation {
public:
    Bestdiv1(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~Bestdiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToPBestDiv1 : public Mutation {
public:
    TargetToPBestDiv1(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~TargetToPBestDiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToBestDiv2 : public Mutation {
public:
    TargetToBestDiv2(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~TargetToBestDiv2() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToRandDiv2 : public Mutation {
public:
    TargetToRandDiv2(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~TargetToRandDiv2() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TwoOptDiv1 : public Mutation {
public:
    TwoOptDiv1(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~TwoOptDiv1() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class Desmu : public Mutation {
public:
    Desmu(size_t dim, size_t n, float F = 0.2, bool archive=false);
    ~Desmu() {};
    MUTATION get_type();
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};
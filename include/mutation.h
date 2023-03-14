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
	Mutation(size_t dim, size_t n, float F = 0.2) : dim(dim), n(n), F(F){};
	bool use_archive(){return false;};
	virtual ~Mutation() { };
	virtual MUTATION get_type() = 0;
	virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx) = 0;

protected:
	float F; //mutation rate
	size_t n; //pop size
	size_t dim;
};

class RandDiv1 : public Mutation {
public:
    RandDiv1(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~RandDiv1() {};
    MUTATION get_type(){return RANDDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class BestDiv1 : public Mutation {
public:
    BestDiv1(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~BestDiv1() {};
    MUTATION get_type(){return BESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToPBestDiv1 : public Mutation {
public:
    TargetToPBestDiv1(size_t dim, size_t n, float F = 0.2, bool archive=false) : Mutation(dim, n, F), archive(archive) {};
    ~TargetToPBestDiv1() {};
    MUTATION get_type(){return TTPBESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
	//TODO check if using needed \/
	using Mutation::use_archive;
	bool use_archive();
protected:	
	bool archive;
};

class TargetToBestDiv2 : public Mutation {
public:
    TargetToBestDiv2(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~TargetToBestDiv2() {};
    MUTATION get_type(){return TTBESTDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToRandDiv2 : public Mutation {
public:
    TargetToRandDiv2(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~TargetToRandDiv2() {};
    MUTATION get_type(){return TTRANDDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TwoOptDiv1 : public Mutation {
public:
    TwoOptDiv1(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~TwoOptDiv1() {};
    MUTATION get_type(){return TWOOPTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class Desmu : public Mutation {
public:
    Desmu(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {};
    ~Desmu() {};
    MUTATION get_type(){return DESMU;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};
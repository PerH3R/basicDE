#pragma once

#include "tools.h"
#include "agent.h"
#include "boundary.h"
#include "operators.h"

#include <iostream>

#include <algorithm>
//TODO: think about this forward declaration
class Agent;


class Mutation {
public:
	Mutation() = default;
	Mutation(size_t dim, size_t n, float F = 0.2) : dim(dim), n(n), F(F){};
	virtual bool use_archive(){return false;};
	virtual ~Mutation() { };
	virtual MUTATION get_type() = 0;
	virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx) = 0;
	virtual void update_vector_pool(double best_fitness, std::vector<Agent*> cur_gen, std::vector<Agent*> next_gen){};

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

class Bea : public Mutation {
public:
	Bea(size_t dim, size_t n, Boundary* boundary_correction, ioh::problem::RealSingleObjective* target_function, unsigned int* budget,
			float F = 0.2, float Pbea = 0.8, int Nsegments = 5, int Nclones = 2) : Mutation(dim, n, F), boundary_correction(boundary_correction),
			target_function(target_function), Nclones(Nclones), Pbea(Pbea), Nsegments(Nsegments), budget(budget){

		if (Nsegments > dim){
			std::cerr << "WARNING: Nsegments larger than dim. Defaulting to Nsegments = dim (segment_size = 1)." << std::endl;
			this->Nsegments = this->dim;
		}
		this->segment_size = std::floor(this->dim / this->Nsegments);
	};
    ~Bea() {};
    MUTATION get_type(){return BEA;};
    std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);

private:
	void mutate_segment(std::vector<Agent*> cur_gen, size_t idx, int x1, int x2, int x3,
		std::vector< std::vector<double> >& clones, std::vector<double>& fitness, int start_gene, int end_gene);

	Boundary* boundary_correction; //clamp seems like a very bad option
	ioh::problem::RealSingleObjective* target_function;

	int Nclones; //>2 is useless unless x1-x3 are rerolled for each clone
	float Pbea;
	int Nsegments;
	int segment_size;
	unsigned int* budget;
};

class DirMut : public Mutation {
public:
	DirMut(size_t dim, size_t n, float F = 0.2) : Mutation(dim, n, F) {
		set_base_operator(new RandDiv1(dim, n, F));
	};
    ~DirMut() {delete this->base_operator; this->base_operator=NULL;};
    void set_base_operator(Mutation* new_base_operator){ base_operator = new_base_operator; };
    void update_vector_pool(double best_fitness, std::vector<Agent*> cur_gen, std::vector<Agent*> next_gen);
    MUTATION get_type(){return DIRMUT;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
private:
	Mutation* base_operator;
	std::vector< std::vector<double> > vector_pool; //pool of difference vectors of improved agents	
	bool improved = false;

};

class RandomSearch : public Mutation {
public:
    RandomSearch(size_t dim, size_t n, ioh::problem::RealSingleObjective* target_function, float F = 0.2) : Mutation(dim, n, F), target_function(target_function) {};
    ~RandomSearch() {};
    MUTATION get_type(){return RANDOMSEARCH;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
private:
	ioh::problem::RealSingleObjective* target_function;
};
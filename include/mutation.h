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
	Mutation(size_t dim, size_t n, float F = 0.5) : dim(dim), n(n), F(F){};
	virtual bool use_archive(){return false;};
	virtual ~Mutation() { };
	virtual MUTATION get_type() = 0;
	virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx) = 0;
	// virtual std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx, std::vector<std::vector<double>>& vector_pool) = 0;
	// virtual void update_vector_pool(double best_fitness, std::vector<Agent*> cur_gen, std::vector<Agent*> next_gen){};
	virtual void improved_to_true(){};
	virtual void pass_vector_pool(std::vector<std::vector<double>>& vector_pool){};

	float get_F(){return this->F;}
	size_t get_n(){return this->n;}
	size_t get_dim(){return this->dim;}

	float set_F(float new_F){return this->F;}

protected:
	float F; //mutation rate
	size_t n; //pop size
	size_t dim;
};

class RandDiv1 : public Mutation {
public:
    RandDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~RandDiv1() {};
    MUTATION get_type(){return RANDDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class RandDiv2 : public Mutation {
public:
    RandDiv2(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~RandDiv2() {};
    MUTATION get_type(){return RANDDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class BestDiv1 : public Mutation {
public:
    BestDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~BestDiv1() {};
    MUTATION get_type(){return BESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class BestDiv2 : public Mutation {
public:
    BestDiv2(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~BestDiv2() {};
    MUTATION get_type(){return BESTDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToPBestDiv1 : public Mutation {
public:
    TargetToPBestDiv1(size_t dim, size_t n, float F = 0.5, bool archive=false) : Mutation(dim, n, F), archive(archive) {
    	this->p_ceil = std::max(double(2.0/this->n), 0.2);
    };
    ~TargetToPBestDiv1() {};
    MUTATION get_type(){return TTPBESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
	//TODO check if 'using' is needed \/
	using Mutation::use_archive;
	bool use_archive();
protected:	
	bool archive;
	float p_ceil;
};

class RandToBestDiv1 : public Mutation {
public:
    RandToBestDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~RandToBestDiv1() {};
    MUTATION get_type(){return RTBESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class RandToBestDiv2 : public Mutation {
public:
    RandToBestDiv2(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~RandToBestDiv2() {};
    MUTATION get_type(){return RTBESTDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToBestDiv1 : public Mutation {
public:
    TargetToBestDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~TargetToBestDiv1() {};
    MUTATION get_type(){return TTBESTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToBestDiv2 : public Mutation {
public:
    TargetToBestDiv2(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~TargetToBestDiv2() {};
    MUTATION get_type(){return TTBESTDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToRandDiv1 : public Mutation {
public:
    TargetToRandDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~TargetToRandDiv1() {};
    MUTATION get_type(){return TTRANDDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TargetToRandDiv2 : public Mutation {
public:
    TargetToRandDiv2(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~TargetToRandDiv2() {};
    MUTATION get_type(){return TTRANDDIV2;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class TwoOptDiv1 : public Mutation {
public:
    TwoOptDiv1(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {};
    ~TwoOptDiv1() {};
    MUTATION get_type(){return TWOOPTDIV1;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
};

class Desmu : public Mutation {
public:
    Desmu(size_t dim, size_t n, float F = 0.5, double alpha = 1.0) : Mutation(dim, n, F) {
	    double a_gamma = std::tgamma(1+alpha);
		this->sig_u = std::pow(a_gamma * std::sin(M_PI * (alpha/2)) / alpha * std::tgamma((1+alpha)/2) * std::pow(2,(alpha-1)/2), 1/alpha );
		this->sig_v = 1.0;
    };
    ~Desmu() {};
    MUTATION get_type(){return DESMU;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
private:
	double alpha = 1.0; //scale factor values for given alpha->(mean,sig) : 1.5->(~0.4, 0.7); 1.0->(~2.3, 13.6); 0.2->(~2.4e11, 2.5e13)
	double sig_u;
	double sig_v;
};

class Bea : public Mutation {
public:
	Bea(size_t dim, size_t n, Boundary* boundary_correction, ioh::problem::RealSingleObjective* target_function, unsigned int* budget,
			float F = 0.5, float Pbea = 0.8, int Nsegments = 5, int Nclones = 2, float locality = 0.1) : Mutation(dim, n, F), boundary_correction(boundary_correction),
			target_function(target_function), Nclones(Nclones), Pbea(Pbea), Nsegments(Nsegments), budget(budget), locality(locality){

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
	void mutate_segment(std::vector<Agent*> cur_gen, size_t idx, std::vector<Agent*> chosen_vectors,
		std::vector< std::vector<double> >& clones, std::vector<double>& fitness, int start_gene, int end_gene);

	Boundary* boundary_correction; //clamp seems like a very bad option
	ioh::problem::RealSingleObjective* target_function;

	int Nclones; //>2 is useless unless x1-x3 are rerolled for each clone
	float Pbea; //Probability of applying BEA local uniform random search
	float locality; // [0,1] fraction of the total search space considered local vor BEA l.u.r.s., originally this is not a fraction but why???
	int Nsegments;
	int segment_size;
	unsigned int* budget;
};

class DirMut : public Mutation {
public:
	DirMut(size_t dim, size_t n, float F = 0.5) : Mutation(dim, n, F) {
		set_base_operator(new RandDiv1(dim, n, F));
		vector_pool_ptr = NULL;
	};
    ~DirMut() {delete this->base_operator; this->base_operator=NULL;};
    void set_base_operator(Mutation* new_base_operator){ base_operator = new_base_operator; };
    // void update_vector_pool(double best_fitness, std::vector<Agent*> cur_gen, std::vector<Agent*> next_gen, std::vector< std::vector<double> >& vector_pool);
    void improved_to_true(){this->improved = true;};
    MUTATION get_type(){return DIRMUT;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
	void pass_vector_pool(std::vector<std::vector<double>>& vector_pool){this->vector_pool_ptr = &vector_pool;}
private:
	Mutation* base_operator;
	std::vector<std::vector<double>>* vector_pool_ptr;
	bool improved = false;
};

class RandomSearch : public Mutation {
public:
    RandomSearch(size_t dim, size_t n, ioh::problem::RealSingleObjective* target_function, float F = 0.5) : Mutation(dim, n, F), target_function(target_function) {};
    ~RandomSearch() {};
    MUTATION get_type(){return RANDOMSEARCH;};
	std::vector<double> apply(std::vector<Agent*> cur_gen, size_t idx);
private:
	ioh::problem::RealSingleObjective* target_function;
};
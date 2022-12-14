#pragma once

#include "agent.h"

enum CROSSOVER {
	BINOMIAL
};

class Crossover {
public:
	Crossover();
	virtual ~Crossover();
    virtual CROSSOVER get_crossover() = 0;
    virtual std::shared_ptr<Agent> apply(std::vector<std::shared_ptr<Agent>> population, int target_index) = 0;

private:

};

class Binomial : public Crossover {
public:
    Binomial();
    CROSSOVER get_crossover();
    std::shared_ptr<Agent> apply(std::vector<std::shared_ptr<Agent>> population, int target_index);
};

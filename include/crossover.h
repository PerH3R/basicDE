#include "agent.h"
#include <random>

enum CROSSOVER {
	BINOMIAL
};

class Crossover {
public:
	Crossover() = default;
	virtual ~Crossover();
    virtual CROSSOVER get_crossover() = 0;
    virtual std::shared_ptr<Agent> apply(std::vector<std::shared_ptr<Agent>> current_gen, std::vector<std::shared_ptr<Agent>> next_gen) = 0;

private:

};

class Binomial : public Crossover {
public:
    Binomial() = default();
    CROSSOVER get_crossover();
    std::shared_ptr<Agent> apply(std::vector<std::shared_ptr<Agent>> current_gen, std::vector<std::shared_ptr<Agent>> next_gen);
};

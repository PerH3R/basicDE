#pragma once

//#define _USE_MATH_DEFINES
//#include <cmath>
#include "tools.h"
#include "agent.h"

// all functions implemented
enum SELECTION {
    ELITIST
};

// base class
class Selection {
public:
    Selection() = default;
    Selection(unsigned int pop_size) {};
    virtual ~Selection() {};
    virtual SELECTION get_type() = 0;
    virtual std::vector<std::shared_ptr<Agent>> apply(std::vector<std::shared_ptr<Agent>> current_gen, std::vector<std::shared_ptr<Agent>> next_gen) = 0;
    


protected:
    unsigned int n; //population size
    SELECTION type;
};


//Elitist selection
class Elitist : public Selection {
public:
    Elitist(unsigned int pop_size);
    SELECTION get_type();
    std::vector<std::shared_ptr<Agent>> apply(std::vector<std::shared_ptr<Agent>> current_gen, std::vector<std::shared_ptr<Agent>> next_gen);
};





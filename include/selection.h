#pragma once

//#define _USE_MATH_DEFINES
//#include <cmath>
// #include "tools.h"
#include "agent.h"

// all functions implemented
enum SELECTION {
    ELITIST
};

// base class
class Selection {
public:
    Selection() = default;
    Selection(size_t n);
    virtual ~Selection() {};
    virtual SELECTION get_type() = 0;
    virtual void apply(std::vector<Agent*> current_gen, std::vector<Agent*> next_gen) = 0;
    


protected:
    size_t n; //population size
    SELECTION type;
};


//Elitist selection
class Elitist : public Selection {
public:
    Elitist(size_t n);
    SELECTION get_type();
    void apply(std::vector<Agent*> current_gen, std::vector<Agent*> next_gen);
};





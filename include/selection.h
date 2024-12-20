#pragma once

//#define _USE_MATH_DEFINES
//#include <cmath>
// #include "tools.h"
#include "agent.h"
#include "operators.h"

// all functions implemented


// base class
class Selection {
public:
    Selection() = default;
    Selection(size_t n);
    void add_to_archive_list(std::vector< std::pair<std::vector<double>, double > > &rejected_values, std::vector<double> position, double fitness);
    virtual ~Selection() {};
    virtual SELECTION get_type() = 0;
    virtual std::vector< bool > apply(std::vector<Agent*>& current_gen, std::vector<Agent*>& next_gen) = 0;
    


protected:
    size_t n; //population size
    SELECTION type;
};


// Elitist selection
// Compare index i of current and next generation. 
// If next is better, swap Agents. If not, leave in next_gen to have location overwritten next iteration
class Elitist : public Selection {
public:
    Elitist(size_t n);
    SELECTION get_type();
    std::vector< bool > apply(std::vector<Agent*>& current_gen, std::vector<Agent*>& next_gen);
};





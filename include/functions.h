#pragma once

//TODO: remove parts specific to previous project 

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
//#include "gdcpp.h"

// all functions implemented
enum FUNCTION {
    SPHERE, SCHWEFEL, ZAKHAROV, MICHALEWICZ
};

// base class
class Function {
public:
    Function() {};
    virtual ~Function() {};
    virtual FUNCTION get_function() = 0;
    virtual double local_evaluate(std::vector<double> coordinates) = 0;
    virtual double evaluate(std::vector<double> coordinates) = 0;

    std::pair<double, double> get_range();

protected:
    double barrier(double x);
    std::pair<double, double> range;
    size_t dimension;
};


//multi dimensional problem - bowl shaped
//https://www.sfu.ca/~ssurjano/spheref.html
class Sphere : public Function {
public:
    Sphere();
    FUNCTION get_function();
    double local_evaluate(std::vector<double> coordinates);
    double evaluate(std::vector<double> coordinates);
};


//multi dimensional problem - many local minima
//https://www.sfu.ca/~ssurjano/schwef.html
class Schwefel : public Function {
public:
    Schwefel();
    FUNCTION get_function();
    double local_evaluate(std::vector<double> coordinates);
    double evaluate(std::vector<double> coordinates);
};


//multi dimensional problem - plate shaped
//https://www.sfu.ca/~ssurjano/zakharov.html
class Zakharov : public Function {
public:
    Zakharov();
    FUNCTION get_function();
    double local_evaluate(std::vector<double> coordinates);
    double evaluate(std::vector<double> coordinates);
};

//multi dimensional problem - steep drops Michalewicz
//https://www.sfu.ca/~ssurjano/michal.html
class Michalewicz : public Function {
public:
    Michalewicz();
    FUNCTION get_function();
    double local_evaluate(std::vector<double> coordinates);
    double evaluate(std::vector<double> coordinates);

private:
    // m defines the steepness of valleys and ridges. Recommended: m=10
    int m = 10;
};


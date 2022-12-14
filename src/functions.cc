#include "../include/functions.h"

// Function
std::pair<double, double> Function::get_range(){
  return range;
}

// Simulates a barrier at the edge of the search space
double Function::barrier(double x){
  // x is within range when this function is called
  if(x <= ((range.first + range.second) / 2)){
    if(-log(x-range.first) < 0) return 0;
    return - log(x - range.first);
  }
  else if(x > ((range.first + range.second) / 2)){
    if(-log(range.second - x) < 0) return 0;
    return - log(range.second - x);
  }
  return 0;
}

// Sphere
Sphere::Sphere() {
  Sphere::range = std::make_pair(-100,100);
}

FUNCTION Sphere::get_function() {
  return SPHERE;
}

// Global evaluate function -> no barrier calculation
double Sphere::evaluate(std::vector<double> coordinates){
  double y = 0.0;
  for (size_t i = 0; i < coordinates.size(); i++) { //dimensions
    y += std::pow(coordinates[i],2);
  }
  return y;
}

// Local evaluate function -> with barrier calculation
double Sphere::local_evaluate(std::vector<double> coordinates){
  double y = 0.0;
  for (size_t i = 0; i < coordinates.size(); i++) { //dimensions
    y += std::pow(coordinates[i],2);
    y += barrier(coordinates[i]);
  }
  return y;
}

// Schwefel
Schwefel::Schwefel() {
  Schwefel::range = std::make_pair(-500,500);
}

FUNCTION Schwefel::get_function() {
  return SCHWEFEL;
}

// Global evaluate function -> no barrier calculation
double Schwefel::evaluate(std::vector<double> coordinates){
  double y = 418.9829 * coordinates.size();
  for (size_t j = 0; j < coordinates.size(); j++) {
    y -= coordinates[j] * std::sin( sqrt(std::abs(coordinates[j])) );
  }
  return y;
}

// Local evaluate function -> with barrier calculation
double Schwefel::local_evaluate(std::vector<double> coordinates){
  double y = 418.9829 * coordinates.size();
  for (size_t j = 0; j < coordinates.size(); j++) {
    y -= coordinates[j] * std::sin( sqrt(std::abs(coordinates[j])) );
    y += barrier(coordinates[j]);
  }
  return y;
}

// Zakharov
Zakharov::Zakharov() {
  Zakharov::range = std::make_pair(-5,10);
}

FUNCTION Zakharov::get_function() {
  return ZAKHAROV;
}

// Global evaluate function -> no barrier calculation
double Zakharov::evaluate(std::vector<double> coordinates){
  double y = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    y += std::pow(coordinates[j],2);
  }
  double temp = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    temp += 0.5 * j * coordinates[j];
  }
  y += std::pow(temp,2);
  temp = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    temp += 0.5 * j * coordinates[j];
  }
  y += std::pow(temp,4);
  return y;
}

// Local evaluate function -> with barrier calculation
double Zakharov::local_evaluate(std::vector<double> coordinates){
  double y = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    y += std::pow(coordinates[j],2);
  }
  double temp = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    temp += 0.5 * j * coordinates[j];
  }
  y += std::pow(temp,2);
  temp = 0;
  for (size_t j = 0; j < coordinates.size(); j++) {
    temp += 0.5 * j * coordinates[j];
  }
  y += std::pow(temp,4);
  for (size_t j = 0; j < coordinates.size(); j++) {
    y += barrier(coordinates[j]);
  }
  return y;
}

// Michalewicz
Michalewicz::Michalewicz() {
  Michalewicz::range = std::make_pair(0,M_PI);
}

FUNCTION Michalewicz::get_function(){
  return MICHALEWICZ;
}

// Global evaluate function -> no barrier calculation
double Michalewicz::evaluate(std::vector<double> coordinates){
    double y = 0.0;
    for (size_t i = 0; i < coordinates.size(); i++) {
      double temp = sin(coordinates[i]);
      temp *= std::pow( std::sin( (i*std::pow( coordinates[i], 2)) / M_PI), 2*m);
      y += temp;
    }
    return -y;
}

// Local evaluate function -> with barrier calculation
double Michalewicz::local_evaluate(std::vector<double> coordinates){
  double y = 0.0;
  for (size_t i = 0; i < coordinates.size(); i++) {
    double temp = sin(coordinates[i]);
    temp *= std::pow( std::sin( (i*std::pow( coordinates[i], 2)) / M_PI), 2*m);
    y += temp;
    y += barrier(coordinates[i]);
  }
  return -y;
}

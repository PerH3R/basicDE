#include "../include/boundary.h"

// Selection
Boundary::Boundary(ioh::problem::bbob::Sphere* target_function) : target_function(target_function){

}


// Elitist
Clamp::Clamp(ioh::problem::bbob::Sphere* target_function) : Boundary(target_function) {
    
}

BOUNDARY Clamp::get_type() {
  return CLAMP;
}

// target_function->bounds().lb
// target_function->bounds().ub

// Receives a possibly o.o.b. position and return an in-bounds position
std::vector<double> Clamp::apply(std::vector<double> pos){
	for (size_t i = 0; i < pos.size(); ++i){
		if (pos[i] < this->target_function->bounds().lb[i]){
			pos[i] = this->target_function->bounds().lb[i];
		}else if (pos[i] > this->target_function->bounds().ub[i]){
			pos[i] = this->target_function->bounds().ub[i];
		}
	}
	return pos;
}




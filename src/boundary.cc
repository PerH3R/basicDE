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

// Apply selection on all agents, returns selection
void Clamp::apply(std::vector<Agent*> next_gen){

    for (size_t i = 0; i < next_gen.size(); ++i){
    	std::vector<double> pos = next_gen[i]->get_position();
    	for (size_t i = 0; i < pos.size(); ++i){
    		if (pos[i] < this->target_function->bounds().lb[i]){
    			pos[i] < this->target_function->bounds().lb[i];
    		}else if (pos[i] > this->target_function->bounds().ub[i]){
    			pos[i] = this->target_function->bounds().ub[i];
    		}
    	}
    	next_gen[i]->set_position(pos);
	}
}




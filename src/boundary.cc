#include "../include/boundary.h"

// Selection
Boundary::Boundary(ioh::problem::RealSingleObjective* target_function) : target_function(target_function){

}


// Clamp

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


// Reflect

// target_function->bounds().lb
// target_function->bounds().ub

// Receives a possibly o.o.b. position and return an in-bounds position
std::vector<double> Reflect::apply(std::vector<double> pos){
	for (size_t i = 0; i < pos.size(); ++i){
		while (pos[i] < this->target_function->bounds().lb[i] || pos[i] > this->target_function->bounds().ub[i]){
			if (pos[i] < this->target_function->bounds().lb[i]){
				pos[i] = this->target_function->bounds().lb[i] + (this->target_function->bounds().lb[i] - pos[i]);
			}else if (pos[i] > this->target_function->bounds().ub[i]){
				pos[i] = this->target_function->bounds().ub[i] + (this->target_function->bounds().ub[i] - pos[i]);
			}
			std::cout << i << " " << pos[i] << std::endl;
		}
	}
	return pos;
}

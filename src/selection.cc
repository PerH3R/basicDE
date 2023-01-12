#include "../include/selection.h"

// Selection
Selection::Selection(size_t n) : n(n){

}


// Elitist
Elitist::Elitist(size_t n) : Selection(n) {
    
}

SELECTION Elitist::get_type() {
  return ELITIST;
}

// Apply selection on all agents, returns selection
void Elitist::apply(std::vector<Agent*> current_gen, std::vector<Agent*> next_gen){

    std::cout << "==============" << std::endl << "selecting" << std::endl;
    for (size_t i = 0; i < n; ++i){
        std::cout << current_gen[i]->get_fitness() << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < n; ++i){
        std::cout << next_gen[i]->get_fitness() << " ";
    }
    std::cout << std::endl;
    
    for (size_t i = 0; i < n; i++) {
        if (current_gen[i]->get_fitness() <= next_gen[i]->get_fitness()) {
            //do nothing
        } else {
            current_gen[i]->set_position(next_gen[i]->get_position());
        }
    }
    std::cout << "-------------" << std::endl;
    for (size_t i = 0; i < n; ++i){
        std::cout << current_gen[i]->get_fitness() << " ";
    }
    std::cout << std::endl;
    std::cout << "==============" << std::endl;
    // return selected;
}




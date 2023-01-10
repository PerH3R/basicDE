#include "../include/selection.h"

// Selection


// Elitist
Elitist::Elitist(unsigned int pop_size) {
    this->n = pop_size;
}

SELECTION Elitist::get_type() {
  return ELITIST;
}

// Apply selection on all agents, returns selection
std::vector<Agent*> Elitist::apply(std::vector<Agent*> current_gen, std::vector<Agent*> next_gen){
    std::vector<Agent*> selected;
    for (size_t i = 0; i < n; i++) {
        if (current_gen[i]->get_fitness() >= next_gen[i]->get_fitness()) {
            selected.push_back(current_gen[i]);
        } else {
            selected.push_back(next_gen[i]);
        }
    }
    return selected;
}




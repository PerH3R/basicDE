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
std::vector<std::shared_ptr<Agent>> Elitist::apply(std::vector<std::shared_ptr<Agent>> current_gen, std::vector<std::shared_ptr<Agent>> next_gen){
    std::vector<std::shared_ptr<Agent>> selected;
    for (size_t i = 0; i < n; i++) {
        if (current_gen[i]->get_fitness() >= next_gen[i]->get_fitness()) {
            selected.push_back(current_gen[i]);
        } else {
            selected.push_back(next_gen[i]);
        }
    }
    return selected;
}




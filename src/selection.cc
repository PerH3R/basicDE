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

// Apply selection on all agents
std::vector< bool > Elitist::apply(std::vector<Agent*>& current_gen, std::vector<Agent*>& next_gen){
    // std::vector< std::pair<std::vector<double>, double > > rejected_values;
    // std::vector< Agent* > rejected_values;
    std::vector<bool> kept_indexes;
    
    for (size_t i = 0; i < n; i++) {
        if (current_gen[i]->get_fitness() <= next_gen[i]->get_fitness()) { //current = better
            // add_to_archive_list(rejected_values, next_gen, i);
            kept_indexes.push_back(false); //nothing needs to be done
        } else {                                                           //new = better -> swap
            kept_indexes.push_back(true);
            Agent* temp = current_gen[i];
            current_gen[i] = next_gen[i];
            next_gen[i] = temp;
            temp = NULL;
        }
    }
    return kept_indexes;
}




#include "../include/selection.h"

// Selection
Selection::Selection(size_t n) : n(n){

}

void Selection::add_to_archive_list(std::vector< std::pair<std::vector<double>, double > > &rejected_values, std::vector<double> position, double fitness){
    rejected_values.push_back( std::make_pair(position, fitness));
}


// Elitist
Elitist::Elitist(size_t n) : Selection(n) {
    
}

SELECTION Elitist::get_type() {
  return ELITIST;
}

// Apply selection on all agents
std::vector< std::pair<std::vector<double>, double > > Elitist::apply(std::vector<Agent*> current_gen, std::vector<Agent*> next_gen){
    std::vector< std::pair<std::vector<double>, double > > rejected_values;

    std::cout << std::endl << "==============" << std::endl << "selecting" << std::endl;
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
            add_to_archive_list(rejected_values, next_gen[i]->get_position(), next_gen[i]->get_fitness());
        } else {
            add_to_archive_list(rejected_values, current_gen[i]->get_position(), current_gen[i]->get_fitness());
            current_gen[i]->set_position(next_gen[i]->get_position());

        }
    }
    std::cout << "-------------" << std::endl;
    for (size_t i = 0; i < n; ++i){
        std::cout << current_gen[i]->get_fitness() << " ";
    }
    std::cout << std::endl;
    std::cout << "==============" << std::endl;
    return rejected_values;
}




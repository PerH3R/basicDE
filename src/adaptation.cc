#include "../include/adaptation.h"

Mutation* AdaptationManager::get_mutation_operator(Argparse* argparser, ioh::problem::RealSingleObjective* problem, Boundary* boundary_correction, 
									unsigned int* budget, int pop_size, int mut_op = -1, float F = -1.0){

	size_t archive = std::stoi(argparser->get_values()["-archive"]);

	switch(mut_op){
		case 0:
			return new RandDiv1(this->dim, this->n, this->F);
		case 1:
			return new RandDiv2(this->dim, this->n, this->F);
		case 2:
			return new BestDiv1(this->dim, this->n, this->F);
		case 3:
			return new BestDiv2(this->dim, this->n, this->F);
		case 4:			
			return new TargetToPBestDiv1(this->dim, this->n, this->F, archive);
		case 5:
			return new RandToBestDiv1(this->dim, this->n, this->F);
		case 6:
			return new RandToBestDiv2(this->dim, this->n, this->F);
		case 7:
			return new TargetToBestDiv1(this->dim, this->n, this->F);
		case 8:
			return new TargetToBestDiv2(this->dim, this->n, this->F);
		case 9:
			return new TargetToRandDiv1(this->dim, this->n, this->F);
		case 10:
			return new TargetToRandDiv2(this->dim, this->n, this->F);
		case 11:
			return new TwoOptDiv1(this->dim, this->n, this->F);
		case 12:
			return new Desmu(this->dim, this->n, this->F);
		case 13:
			return new Bea(this->dim, this->n, this->boundary_correction, this->problem, this->budget, this->F);
		case 14:
			return new DirMut(this->dim, this->n, this->F);
		case 15:
			return new RandomSearch(this->dim, this->n, problem, this->F);
		default:
			std::cerr << "Mutation operator " << mut_op << " out of range. Continuing using RandDiv1." << std::endl;
			return new RandDiv1(this->dim, this->n, this->F);		
	}	
}
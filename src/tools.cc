#include "../include/tools.h"

void set_seed(int seed){
	if(seed==-1){
		std::srand(time(NULL));
	}else{
		std::srand(seed);
	}
}
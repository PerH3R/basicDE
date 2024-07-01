#include "../include/argparse.h"


Argparse::Argparse(const int argc, char* argv[]) : argc(argc), argv(argv){
	// std::vector<std::string> flags;

	printArgs();

	int i = 1; //skip main
	
	// Check if values are correct format
	while (i < argc){
		std::cout << i << " " << argv[i] << std::endl;		
		//is a flag
		if (argv[i][0] == '-'){
			std::string argument(argv[i]);

			if ( (argument == "-F" || argument == "-Cr" || argument == "-eps") && (this->isReal(argv[i+1]) ) ){ //real values
				flag_vals[argv[i]] = argv[i+1];
				i+=2; //skip associated value
			}else if ((flag_vals.find(argv[i]) != flag_vals.end()) && (this->isInteger(argv[i+1]))){ //integers (rest)
				flag_vals[argv[i]] = argv[i+1];
				i+=2; //skip associated value
			}else{
				std::cerr << "invalid flag: " << '"' << argv[i] << '"' << std::endl;
				i++;
			}
		}else{
			std::cerr << "invalid formatting at: " << '"' << argv[i] << '"' << std::endl;
			i++; //
		}
	// Check if values in valid range
	if (!checkValidity()){ std::cerr << "Error in passed values " << std::endl;	}

	};

	printArgs();
}

//TODO: add remaing args 
//after correct format check, checks if parameters are in correct range
bool Argparse::checkValidity(){
	for (auto item = flag_vals.cbegin(); item != flag_vals.cend(); ++item){
		if (!((item->first == "-f") && (std::stoi(item->second) > 0 && std::stoi(item->second) <= 24))){return false;}
		else if (!((item->first == "-d") && (std::stoi(item->second) > 0 ))) {return false;}
		else if (!((item->first == "-m") && (std::stoi(item->second) > 0 && std::stoi(item->second) <= NUM_MUTATION_OPERATORS))) {return false;}
		else if (!((item->first == "-F") && (std::stod(item->second) > 0.0 ))) {return false;}
		else if (!((item->first == "-m") && (std::stoi(item->second) > 0 && std::stoi(item->second) <= NUM_CROSSOVER_OPERATORS))) {return false;}
		else if (!((item->first == "-Cr") && (std::stod(item->second) > 0.0 && std::stod(item->second) <= 1.0))) {return false;}
		else if (!((item->first == "-b") && (std::stoi(item->second) > 0 && std::stoi(item->second) <= NUM_BOUNDARY_OPERATORS))) {return false;}
		else if (!((item->first == "-m") && (std::stoi(item->second) > 0 && std::stoi(item->second) <= NUM_SELECTION_OPERATORS))) {return false;}
		else if (!((item->first == "-a") && (std::stoi(item->second) > 0 ))) {return false;} //&& std::stoi(item->second) =< NUM_CROSSOVER_OPERATORS)
		else if (!((item->first == "-r") && (std::stoi(item->second) >= -1 ))) {return false;}
		else if (!((item->first == "-r") && (std::stod(item->second) >= 0.0 ))) {return false;}
		else if (!((item->first == "-lp") && (std::stoi(item->second) >= 0 ))) {return false;}
		else if (!((item->first == "-budget") && (std::stoi(item->second) > 0 ))) {return false;}
		else if (!((item->first == "-budget_multiplier") && (std::stod(item->second) >= 0.0))) {return false;}
		else if (!((item->first == "-pop_size") && (std::stoi(item->second) >= 0 ))) {return false;}
	}
	return true;
}

std::map<std::string, std::string> Argparse::get_values() const{
	return flag_vals;
}

bool Argparse::isReal(const std::string& str){
	std::cout << str;
	for (char c : str){
		if (!(std::isdigit(c) || c == '.' || c == '-')){
			std::cout << "f" << std::endl;
			return false;
		}
	}
	std::cout << "t" << std::endl;
	return true;
}

bool Argparse::isInteger(const std::string& str){
	for (char c : str){
		std::cout << str << " " << c << " " << (std::isdigit(c)) << (c == '-') << std::endl;
		if (!((std::isdigit(c)) || c == '-')){
			return false;
		}
	}
	return true;
}

void Argparse::printArgs(){
	for(auto it = flag_vals.cbegin(); it != flag_vals.cend(); ++it){
    	std::cout << it->first << " " << it->second << "\n";
	}
}

std::string Argparse::getArgsAsString(){
	std::string returnvalue;
	for(auto it = flag_vals.cbegin(); it != flag_vals.cend(); ++it){
    	returnvalue += it->first + " " + it->second + " ";
	}
	return returnvalue;
}
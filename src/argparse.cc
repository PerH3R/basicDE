#include "../include/argparse.h"

Argparse::Argparse(int argc, char* argv[]) : argc(argc), argv(argv)//, 
		//arg_names{"-f", "-d", "-m", "-F", "-c", "-Cr", "-pop_mult", "-i", "-budget", "-archive"}
{
	// std::vector<std::string> flags;

	printArgs();

	int i = 1; //skip main
	while (i < argc){
		std::cout << i << " " << argv[i] << std::endl;		
		//is a flag
		if (argv[i][0] == '-'){
			// if (argv[i] == "-archive"){ //bools
				// flag_vals[argv[i]] = "1";
				// i++; //next flag
			// }else 
			std::string argument(argv[i]);
			if ( (argument == "-F" || argument == "-Cr") && (this->isReal(argv[i+1]) ) ){ //doubles
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
	
	};

	printArgs();
}

std::map<std::string, std::string> Argparse::get_values() const{
	return flag_vals;
}

bool Argparse::isReal(std::string str){
	for (char& c : str){
		if (!(std::isdigit(c) || c == '.')){
			return false;
		}
	}
	return true;
}

bool Argparse::isInteger(std::string str){
	for (char& c : str){
		if (!std::isdigit(c)){
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
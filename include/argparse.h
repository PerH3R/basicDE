#pragma once
#include "operators.h"

#include <iostream>
#include <string>
#include <map>


//TODO: make usable for ranges of numbers


// #include <cctype>

/*
flag				meaning						default					vals

//base
-f					function					1						int 1-24
-d					dimensions					0						int > 0, 0=5,20
-m					base mutation operator		1						int 0-15 
-F					mutation rate 				0.0						real (TODO F \in [2/pop_size, 1] or [2/pop_size, D/pop_size] https://cir.nii.ac.jp/crid/1573387450569887488)
-c					base crossover operator		0						int (TODO -> string)
-Cr					crossover rate 				0.5						real 0.0-1.0
-b					base boundary operator		0 						int 0-2
-s					selection operator			0 						int 0-0
-a					operator adaptor			0 						int 0-...
-r					resample limit				-1						int >= 0, negative will use automatic limit scaled with dimensionality (10+std::pow((std::log(this->dim)),2))
-eps_f				epsilon						0.0 					real, minimum difference between min and max fitnessfor early stopping
-eps_a				epsilon						0.1 					real, chance of random operator selection during MAB adaptation
-MABsel				strategy selection for MAB	0 						int, determines how the operators of next iteration are chosen. 0=best, 1=proportional			
-lp					learning period				50 						int > 0, number of iterations between the updating of each Agents operators

-runs-				repeated runs				1						int
-budget				base allowed funtion evals	10000					int, also lowest value budget can go
-budget_multiplier	b = b * (b_m*dim)			1						real > 0.0
-pop_size			population size				0						int >= 4, 0 >= int < 4 will use automatic population size based on 5*dimension of problem
	
//operator specific
-archive		use archive (ttpb mut)			0						int (if negative, archive size = pop_size)	

*/


class Argparse {
public:
	Argparse(const int argc, char* argv[]);
	~Argparse(){};

	bool isReal(const std::string& str);
	bool isInteger(const std::string& str);
	void printArgs();
	std::string getArgsAsString();
	std::map<std::string, std::string> get_values() const;
	bool checkValidity();


private:
	const int argc;
	char** argv;
	//const std::string arg_names[10];
	std::map<std::string, std::string> flag_vals = {
		{"-f","1"}, 
		{"-d","0"}, 
		{"-m","0"}, 
		{"-F","0.5"}, 
		{"-c","0"}, 
		{"-Cr","0.5"}, 
		{"-b", "0"},
		{"-s", "0"},
		{"-a", "0"},
		{"-r", "-1"},
		{"-eps_f", "0.0"},		
		{"-lp", "50"},
		{"-runs","1"}, 
		{"-budget", "10000"}, 
		{"-budget_multiplier","1"},
		{"-pop_size", "0"},
		{"-archive","0"},
		{"-eps_a","0.1"},
		{"-MABsel","0"},
		{"-credit","1"},
	};


};


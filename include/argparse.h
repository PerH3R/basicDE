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
-d					dimensions					0						int > 0; 0 = 5,20
-m					base mutation operator		1						int 0-16 
-F					mutation rate 				0.0						real (TODO F \in [2/pop_size, 1] or [2/pop_size, D/pop_size] https://cir.nii.ac.jp/crid/1573387450569887488)
-c					base crossover operator		0						int 0-1 0=BINOMIAL 1=EXPONENTIAL 
-Cr					crossover rate 				0.5						real 0.0-1.0
-b					base boundary operator		0 						int 0-2
-s					selection operator			0 						int 0-0
-a					operator adaptor			2 						int 0-2, 0=fixed; 1=random muation operators; 2=MAB based Mutation operator selection
-r					resample limit				-1						int >= 0, negative will use automatic limit scaled with dimensionality (10+std::pow((std::log(this->dim)),2))
-eps_f				epsilon						0.0 					real, minimum difference between min and max fitnessfor early stopping
-eps_a				epsilon						0.1 					real, chance of random operator selection during MAB adaptation
-MABdecay			decay multiplier MAB		0.5 					real [0,1] affect how quick MAB ''forgets'' obtained results higher=faster
-MABsel				strategy selection for MAB	1 						int, determines how the operators of next iteration are chosen. 0=greedy, 1=proportional
-credit				how improvement is scored	3 						int; 0=fitness, 1=tanh, 2=binary, 3=binary+			
-lp					learning period				50 						int > 0, number of iterations between the updating of each Agents operators
-ops				selection of mutops for MAB	65535

-runs-				repeated runs				1						int
-budget				base allowed funtion evals	10000					int, also lowest value budget can go
-budget_multiplier	b = b * (b_m*dim)			1						real > 0.0
-pop_size			population size				0						int >= 4, 0 >= int < 4 will use automatic population size based on 5*dimension of problem
-logQ				log MAB Q-values			0 						boolean (numerical 0/1)	

//operator specific
-archive		use archive (ttpb mut)			0						int (if negative, archive size = pop_size)	

*/


class Argparse {
public:
	// Parse trough passed arguments and their values
	Argparse(const int argc, char* argv[]);
	~Argparse(){};

	// Check correctness of value format
	bool isReal(const std::string& str);
	bool isInteger(const std::string& str);
	// Check if values are in correct range
	bool checkValidity();

	// Print args and values to console
	void printArgs();
	// Get args and values as string
	std::string getArgsAsString();

	// Get searchable map and their values
	std::map<std::string, std::string> get_values() const;
	


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
		{"-a", "2"},
		{"-r", "-1"},
		{"-eps_f", "0.0"},		
		{"-lp", "5"},
		{"-ops", "65535"},
		{"-runs","1"}, 
		{"-budget", "10000"}, 
		{"-budget_multiplier","1"},
		{"-pop_size", "0"},
		{"-archive","0"},
		{"-eps_a","0.1"},
		{"-MABdecay","0.5"},
		{"-MABsel","1"},
		{"-credit","1"},
		{"-logQ","0"}
	};


};


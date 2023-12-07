#pragma once

#include <iostream>
#include <string>
#include <map>


//TODO: make usable for ranges of numbers


// #include <cctype>

/*
flag				meaning						default					vals

//base
-f					function					1						int 1-24
-d					dimensions					5						int > 0
-m					base mutation operator		1						int 0-15 
-F					mutation rate 				0.0						real (TODO F \in [2/pop_size, 1] or [2/pop_size, D/pop_size] https://cir.nii.ac.jp/crid/1573387450569887488)
-c					base crossover operator		0						int (TODO -> string)
-Cr					crossover rate 				0.5						real 0.0-1.0
-b					base boundary operator		0 						int 0-2
-s					selection operator			0 						int 0-0
-a					operator adaptor			0 						int 0-...
-r					resample limit				-1						int >= 0, negative will use automatic limit scaled with dimensionality (10+std::pow((std::log(this->dim)),2))
-eps				epsilon						0.0 					real, minimum difference between min and max fitnessfor early stopping

-runs-				repeated runs				1						int
-budget				base allowed funtion evals	2500					int, also lowest value budget can go
-budget_multiplier	b = b * (b_m*dim)			1						real > 0.0
-pop_size			population size				0						int >= 4, 0 >= int < 4 will use automatic population size based on 5*dimension of problem
	
//operator specific
-archive		use archive (ttpb mut)			0						int (if negative, archive size = pop_size)	

*/

/*
TODO: configurable retry/resample limit
*/

class Argparse {
public:
	Argparse(const int argc, char* argv[]);
	~Argparse(){};

	bool isReal(const std::string str);
	bool isInteger(const std::string str);
	void printArgs();
	std::string getArgsAsString();
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
		{"-a", "0"},
		{"-r", "-1"},
		{"-eps", "0.0"},
		{"-runs","1"}, 
		{"-budget", "10000"}, 
		{"-budget_multiplier","1"},
		{"-pop_size", "0"},
		{"-archive","0"},
	};


};


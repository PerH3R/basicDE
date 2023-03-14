#pragma once

#include <iostream>
#include <string>
#include <map>


//TODO: make usable for ranges of numbers


// #include <cctype>

/*
flag				meaning					default					vals

//base
-f					function				1						1-24
-d					dimensions				5						int
-m					mutation type			1						int (TODO -> string)
-F					mutation rate 			0.2						real
-c					crossover type			1						int (TODO -> string)
-Cr					crossover rate 			0.2						real
-runs				repeated runs			1						int
-budget				mutex with -i			2500					int	
-pop_size			population size			25						int

//operator specific
-archive_size		use archive (ttpb mut)	0						int (0 = no archive)	

*/

class Argparse {
public:
	Argparse(int argc, char* argv[]);
	~Argparse();

	bool isReal(std::string str);
	bool isInteger(std::string str);
	void printArgs();
	std::map<std::string, std::string> get_values() const;


private:
	const int argc;
	char** argv;
	//const std::string arg_names[10];
	std::map<std::string, std::string> flag_vals = {
		{"-f","1"}, 
		{"-d","5"}, 
		{"-m","1"}, 
		{"-F","0.2"}, 
		{"-c","1"}, 
		{"-Cr","0.2"}, 
		{"-runs","1"}, 
		{"-budget", "2500"}, 
		{"-pop_size", "25"},
		{"-archive","0"}
	};


};


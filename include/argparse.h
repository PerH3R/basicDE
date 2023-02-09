#pragma once

#include <iostream>
#include <string>

/*
flag				meaning					default					vals

//base
-f					function				1						1-24
-d					dimensions				5						int
-m					mutation type			1						int (TODO -> string)
-F					mutation rate 			0.2						real
-c					crossover type			1						int (TODO -> string)
-Cr					crossover rate 			0.2						real
-pop_mult			dim*pop_mult=popsize	5						int
-i					i*popsize=budget		100						int
-budget				mutex with -i			2500					int	

//operator specific
-archive			use archive (ttpb mut)	false					bool	

*/

class Argparse {
public:
	Argparse(int argc, char* argv[]);
	~Argparse();


private:
	const int argc;
	char** argv;
	const std::string arg_names[10];


};


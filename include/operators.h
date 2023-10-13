#pragma once

#include <string>

/*
 * 	MUTATION
 */

enum MUTATION {
	RANDDIV1,
	BESTDIV1,
	TTPBESTDIV1,
	TTBESTDIV2,
	TTRANDDIV2,
	TWOOPTDIV1,
	DESMU,
	BEA,
	DIRMUT,
	RANDOMSEARCH
	};

const std::string MUTATION_NAMES[]  = {
	"rand/1",
	"best/1",
	"ttpb/1",
	"ttb/2",
	"ttr/2",
	"2opt/1",
	"desmu",
	"BEA",
	"DIRMUT",
	"random search"
	};

/*
 * 	BOUNDARY
 */

enum BOUNDARY {
	CLAMP,
    REFLECT,
    REINIT
};


/*
 * 	CROSSOVER
 */

enum CROSSOVER {
	BINOMIAL,
    EXPONENTIAL
};

/*
 * 	SELECTION
 */

enum SELECTION {
    ELITIST
};
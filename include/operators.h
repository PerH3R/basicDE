#pragma once

#include <string>

/*
 * 	MUTATION
 */

enum MUTATION {
	RANDDIV1,
	RANDDIV2,
	BESTDIV1,
	BESTDIV2,
	TTPBESTDIV1,
	RTBESTDIV1,
	RTBESTDIV2,
	TTBESTDIV1,
	TTBESTDIV2,
	TTRANDDIV1,
	TTRANDDIV2,
	TWOOPTDIV1,
	TWOOPTDIV2,
	DESMU,
	BEA,
	DIRMUT,
	RANDOMSEARCH,
	NUM_MUTATION_OPERATORS,
	};

const std::string MUTATION_NAMES[]  = {
	"rand/1",
	"rand/2",
	"best/1",
	"best/2",
	"ttpb/1",
	"rand-to-best/1",
	"rand-to-best/2",
	"target-to-best/1",
	"target-to-best/2",
	"target-to-rand/1",
	"target-to-rand/2",
	"2opt/1",
	"2opt/2",
	"desmu",
	"BEA",
	"DIRMUT",
	"Random search"
	};

/*
 * 	BOUNDARY
 */

enum BOUNDARY {
	CLAMP,
    REFLECT,
    REINIT,
    NUM_BOUNDARY_OPERATORS = REINIT,
};

const std::string BOUNDARY_NAMES[]  = {
	"Clamp",
	"Reflect",
	"Reinitialize"
};

/*
 * 	CROSSOVER
 */

enum CROSSOVER {
	BINOMIAL,
    EXPONENTIAL,
    NUM_CROSSOVER_OPERATORS = EXPONENTIAL,
};

const std::string CROSSOVER_NAMES[] = {
	"Bin",
	"Exp"
};

/*
 * 	SELECTION
 */

enum SELECTION {
    ELITIST,
    NUM_SELECTION_OPERATORS = ELITIST,
};

const std::string SELECTION_NAMES[] = {
	"Elitist"
};

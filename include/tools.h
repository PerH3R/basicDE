#pragma once

//#include <thread>
#include <vector>
#include <memory>
#include <random>
#include "time.h"

void set_seed(int seed=-1);

double get_euclidean_distance(std::vector<double> pos1,
							  std::vector<double> pos2);


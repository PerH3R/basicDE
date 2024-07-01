#include "tools.h"

//Credit calculates improvement scores for individual agents

class Credit{
public:
	Credit(int lp) : lp(lp) { };
	virtual ~Credit() { };
	virtual double get_credit(const std::vector< 
								std::tuple<
									std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>
								>
							>& history,
							std::vector<double> mean_position,
							const double& previous_best_fitness
							) = 0;
protected:
	int lp; //learning period
};

class FitnessImprovement : public Credit{
public:
	FitnessImprovement(int lp) : Credit(lp) {};
	~FitnessImprovement() {};
	double get_credit(const std::vector< 
						std::tuple<
							std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>
						>
					>& history,
					std::vector<double> mean_position,
					const double& previous_best_fitness
					){
		(void)mean_position; //silence warning
		(void)previous_best_fitness;

		const double last_fitness = std::get<1>(history[history.size() - 1]);
		const double first_fitness = std::get<1>(history[history.size() - this->lp]); //why does rbegin iterator straight up not work :'(
		double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		return fitness_improvement;
	}
};

class TanhFitnessImprovement : public Credit{
public:
	TanhFitnessImprovement(int lp) : Credit(lp) {};
	~TanhFitnessImprovement() {};
	double get_credit(const std::vector< 
						std::tuple<
							std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>
						>
					>& history,
					std::vector<double> mean_position,
					const double& previous_best_fitness
					){
		(void)mean_position; //silence warning
		(void)previous_best_fitness;

		const double last_fitness = std::get<1>(history[history.size() - 1]);
		const double first_fitness = std::get<1>(history[history.size() - this->lp]); //why does rbegin iterator straight up not work :'(
		double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		return std::tanh(fitness_improvement); //TODO is log ok here
	}
};

class BinaryImprovement : public Credit{
public:
	BinaryImprovement(int lp) : Credit(lp) {};
	~BinaryImprovement() {};
	double get_credit(const std::vector< 
						std::tuple<
							std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>
						>
					>& history,
					std::vector<double> mean_position,
					const double& previous_best_fitness
					){
		(void)mean_position; //silence warning
		(void)previous_best_fitness;

		const double last_fitness = std::get<1>(history[history.size() - 1]);
		const double first_fitness = std::get<1>(history[history.size() - this->lp]); //why does rbegin iterator straight up not work :'(
		double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		if(fitness_improvement > 0){
			return 1;
		}
		return 0;
	}
};

class R2Improvement : public Credit{
public:
	R2Improvement(int lp) : Credit(lp) {};
	~R2Improvement() {};
	double get_credit(const std::vector< 
						std::tuple<
							std::vector<double>, double, std::shared_ptr<Crossover>, std::shared_ptr<Mutation>, std::shared_ptr<Boundary>
						>
					>& history,
					std::vector<double> mean_position,
					const double& previous_best_fitness
					){
		(void)mean_position; //silence warning

		const double last_fitness = std::get<1>(history[history.size() - 1]);
		if (last_fitness < previous_best_fitness){
			return 10;
		}
		const double first_fitness = std::get<1>(history[history.size() - this->lp]); //why does rbegin iterator straight up not work :'(		
		double fitness_improvement = std::abs(first_fitness - last_fitness); //abs just in case someone want to maximise or smth
		if(fitness_improvement > 0){
			return 1;
		}
		return 0; 
	}
};

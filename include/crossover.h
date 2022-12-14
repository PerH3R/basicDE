

enum CROSSOVER {
	BINOMIAL
};

class Crossover {
public:
	Crossover();
	virtual ~Crossover();
    virtual CROSSOVER get_crossover() = 0;
    virtual Agent* apply(std::vector<Agent*> population, int target_index) = 0;

private:

};

class Binomial : public Crossover {
public:
    Binomial();
    CROSSOVER get_crossover();
    Agent* apply(std::vector<Agent*> population, int target_index);
};

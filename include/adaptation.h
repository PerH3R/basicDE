#pragma once

/*

*/

class AdaptationManager{
public:

private:
	std::vector<double> F_history;
	std::vector<double> Cr_history;

}


//no adaptation. for control
class FixedManager : public AdaptationManager{
public:
	
private:
	double const F;
	double const Cr;
}
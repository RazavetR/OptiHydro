#pragma once

#include <vector>

class Turbine
{
private :
	
public:
	std::vector<double> Etats;
	std::vector<double> VariableDecision;
	std::vector<double> Production;
	std::vector<double> ChuteNette;
	int DebitMax = 160;
	int Discretisation = 5;


	Turbine();
	
	
	
	~Turbine();
};


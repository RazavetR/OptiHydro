#pragma once

#include <vector>

class Turbine
{
private :
	
public:
	//Vecteur contenant les Etats possibles de la Turbine
	std::vector<double> Etats;
	//Vecteur contenant les Variables de D�cision de la Turbine
	std::vector<double> VariableDecision;
	//Vecteur contenant les Productions possibles de la Turbine
	std::vector<double> Production;
	//Vecteur contenant les Productions possibles de la Turbine
	std::vector<double> ChuteNette;
	//Matrice de D�cision permettant de calculer la production en fonction du d�bit restant � turbiner et du d�bit que la turbine pourrait turbiner
	std::vector<std::vector<double>> MatriceDecision;
	//Debit Max de la turbine
	int DebitMax = 100;
	//Discr�tisation du d�bit (ici 5m^3 sec)
	int Discretisation = 5;


	Turbine();	
	~Turbine();
};


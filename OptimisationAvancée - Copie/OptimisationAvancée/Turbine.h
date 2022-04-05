#pragma once

#include <vector>

class Turbine
{
private :
	
public:
	//Vecteur contenant les Etats possibles de la Turbine
	std::vector<double> Etats;
	//Vecteur contenant les Variables de Décision de la Turbine
	std::vector<double> VariableDecision;
	//Vecteur contenant les Productions possibles de la Turbine
	std::vector<double> Production;
	//Vecteur contenant les Productions possibles de la Turbine
	std::vector<double> ChuteNette;
	//Matrice de Décision permettant de calculer la production en fonction du débit restant à turbiner et du débit que la turbine pourrait turbiner
	std::vector<std::vector<double>> MatriceDecision;
	//Debit Max de la turbine
	int DebitMax = 100;
	//Discrétisation du débit (ici 5m^3 sec)
	int Discretisation = 5;


	Turbine();	
	~Turbine();
};


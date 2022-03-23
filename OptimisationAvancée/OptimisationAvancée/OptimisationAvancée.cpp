// OptimisationAvancée.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//


#include <iostream>
#include <vector>
#include "Turbine.h"

void Backward(std::vector<Turbine> &Centrale, double ElevationAmmont, double ElevationAval);

double ProductionT1(double uneHauteur, double unDebit);
double ProductionT2(double uneHauteur, double unDebit);
double ProductionT3(double uneHauteur, double unDebit);
double ProductionT4(double uneHauteur, double unDebit);
double ProductionT5(double uneHauteur, double unDebit);

double HauteurChutteNette(double uneElevationAmmont, double uneElevationAval, double unDebit);

int main()
{
	double ElevationAmmont;
	double DebitTotale ;
	double ElevationAvale = DebitTotale * 0.5;


	std::vector<Turbine> Centrale;
	Centrale[Centrale.size] = Turbine();
	Backward(Centrale, ElevationAmmont, ElevationAvale);


    std::cout << "Hello World!\n"; 
}



// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtrie Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

void Backward(std::vector<Turbine> & Centrale, double ElevationAmmont, double ElevationAval)
{
	Centrale[4].Production.resize(Centrale[4].DebitMax / Centrale[4].Discretisation +1);
	Centrale[4].ChuteNette.resize(Centrale[4].DebitMax / Centrale[4].Discretisation + 1);
	Centrale[4].Etats.resize(Centrale[4].DebitMax / Centrale[4].Discretisation + 1);
	Centrale[4].VariableDecision.resize(Centrale[4].DebitMax / Centrale[4].Discretisation + 1);

	for (int i = 0; i < Centrale[4].Production.size; i++) {
		Centrale[4].Etats[i] = i * Centrale[4].Discretisation;
		Centrale[4].ChuteNette[i] = HauteurChutteNette(ElevationAmmont, ElevationAval, Centrale[4].Etats[i]);
		Centrale[4].VariableDecision[i] = Centrale[4].Etats[i];

		Centrale[Centrale.size].Production[i] = ProductionT5(Centrale[4].ChuteNette[i], Centrale[4].Etats[i]);
	}
	Centrale[Centrale.size];
}

double ProductionT1(double uneHauteur, double unDebit)
{
	return 0.0;
}

double ProductionT2(double uneHauteur, double unDebit)
{
	return 0.0;
}

double ProductionT3(double uneHauteur, double unDebit)
{
	return 0.0;
}

double ProductionT4(double uneHauteur, double unDebit)
{
	return 0.0;
}

double ProductionT5(double uneHauteur, double unDebit)
{
	return 0.0;
}

double HauteurChutteNette(double uneElevationAmmont, double uneElevationAval, double unDebit)
{
	return uneElevationAmmont - uneElevationAval - 0.5 * pow(10,-5) * unDebit * unDebit;
}


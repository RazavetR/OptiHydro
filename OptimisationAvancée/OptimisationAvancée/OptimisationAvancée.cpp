// OptimisationAvancée.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//


#include <iostream>
#include <vector>
#include "Turbine.h"

void Backward(std::vector<Turbine> &Centrale, double ElevationAmmont, double ElevationAval, double DebitTotale);
void Forward(std::vector<Turbine> Centrale, std::vector<double>& Solution, int DebitTotale);

double ProductionT1(double uneHauteur, double unDebit);
double ProductionT2(double uneHauteur, double unDebit);
double ProductionT3(double uneHauteur, double unDebit);
double ProductionT4(double uneHauteur, double unDebit);
double ProductionT5(double uneHauteur, double unDebit);

double HauteurChutteNette(double uneElevationAmmont, double uneElevationAval, double unDebit);
void RemplissageMatrice(Turbine & uneTurbine, int NumeroTurbine, std::vector<Turbine> uneCentrale);
void RemplissageDecisionProduction(Turbine& uneTurbine);

int main()
{
	//On définit nos variable d'entrée
	double ElevationAmont = 137.89;
	double DebitTotal = 469.84;

	// On applique la fonction permettant d'estimer l'élévation en aval en fonction du débit total
	double ElevationAval = -1.461 * pow(10,-6) * DebitTotal * DebitTotal + 7.064 * pow(10,-3) * DebitTotal + 99.96;
	std::cout << "Elevation aval calculée: " << ElevationAval << "\n";

	//On Initialise notre Centrale
	std::vector<Turbine> Centrale;
	Centrale.resize(5);
	for (int i = 0; i < Centrale.size();i++) {
		Centrale[i] = Turbine();
	}
	
	//On définit les valeurs de DebitMax
	Centrale[1].DebitMax = 160;
	Centrale[2].DebitMax = 160;
	Centrale[3].DebitMax = 160;
	Centrale[4].DebitMax = 160;
	Centrale[0].DebitMax = 160;

	//On utilise notre fonction pour lancer notre phase arrière
	Backward(Centrale, ElevationAmont, ElevationAval, DebitTotal);

	//On initialise notre vecteur Solution qui va contenir les débit turbiné de chaque turbine
	std::vector<double>vSolution;
	vSolution.resize(5);

	//On utilise notre fonction Forward pour lancer notre phase avant
	Forward(Centrale, vSolution, DebitTotal);

	//On affiche notre solution
	for (int i = 0; i < vSolution.size();i++) {
		std::cout << "La Turbine " << i << " devra turbiner " << vSolution[i]*5 <<" m^3/sec \n";
	}
    
}



void Backward(std::vector<Turbine>& Centrale, double ElevationAmont, double ElevationAval, double DebitTotal)
{
	
	//On commence notre fonction de Backward avec notre dernière Turbine
	Centrale[4].Production.resize(DebitTotal / Centrale[4].Discretisation + 1);
	Centrale[4].ChuteNette.resize(DebitTotal / Centrale[4].Discretisation + 1);
	Centrale[4].Etats.resize(DebitTotal / Centrale[4].Discretisation + 1);
	Centrale[4].VariableDecision.resize(DebitTotal / Centrale[4].Discretisation + 1);

	for (int i = 0; i < Centrale[4].Production.size(); i++) {
		//On remplit nos valeurs pour la turbine 5
		Centrale[4].Etats[i] = i * Centrale[4].Discretisation;
		Centrale[4].ChuteNette[i] = HauteurChutteNette(ElevationAmont, ElevationAval, Centrale[4].Etats[i]);
		Centrale[4].VariableDecision[i] = Centrale[4].Etats[i];

		//On vérifie si le débit ne passe pas le débit max de la turbine.
		if (Centrale[4].Etats[i] <= Centrale[4].DebitMax) {
			Centrale[4].Production[i] = ProductionT5(Centrale[4].ChuteNette[i], Centrale[4].Etats[i]);
		}
		else {
			Centrale[4].Production[i] = ProductionT5(Centrale[4].ChuteNette[Centrale[4].DebitMax / 5], Centrale[4].DebitMax);
		}
	}


	//On initialise les valeurs des tableaux pour les turbines 4 à 2
	for (int eTurbine = 3; eTurbine > 0; eTurbine--) {

		
		Centrale[eTurbine].Production.resize(DebitTotal / Centrale[eTurbine].Discretisation + 1);
		Centrale[eTurbine].ChuteNette.resize(Centrale[eTurbine].DebitMax / Centrale[eTurbine].Discretisation + 1);
		Centrale[eTurbine].Etats.resize(Centrale[eTurbine].DebitMax / Centrale[eTurbine].Discretisation + 1);
		Centrale[eTurbine].VariableDecision.resize(DebitTotal / Centrale[eTurbine].Discretisation + 1);

		//On intialise la Matrice de Decisions qui va nous aider à trouver la quantité que l'on va turbiner en fonction de la quantité reçus
		Centrale[eTurbine].MatriceDecision.resize(DebitTotal / Centrale[eTurbine].Discretisation + 1);
		for (int eResize = 0; eResize < Centrale[eTurbine].MatriceDecision.size(); eResize++) {
			Centrale[eTurbine].MatriceDecision[eResize].resize(eResize+1);
		}

		//On rempli nos Vecteur d'Etats et de Chutte Nette
		for (int i = 0; i < Centrale[eTurbine].Etats.size(); i++) {

			Centrale[eTurbine].Etats[i] = i * Centrale[eTurbine].Discretisation;
			Centrale[eTurbine].ChuteNette[i] = HauteurChutteNette(ElevationAmont, ElevationAval, Centrale[eTurbine].Etats[i]);
		}

		//On remplie notre Matrice de Décision
		RemplissageMatrice(Centrale[eTurbine], eTurbine, Centrale);
		//On remplie notre vecteur de Variable de décision
		RemplissageDecisionProduction(Centrale[eTurbine]);
	}

	//On intialise nos tableaux pour la Turbine 1
	Centrale[0].Production.resize(DebitTotal / Centrale[0].Discretisation + 1);
	Centrale[0].ChuteNette.resize(Centrale[0].DebitMax / Centrale[0].Discretisation + 1);
	Centrale[0].Etats.resize(Centrale[0].DebitMax / Centrale[0].Discretisation + 1);
	Centrale[0].VariableDecision.resize(1);

	int debit = 0;
	int indice = 0;
	
	//On initialise le vecteur contenant la production de notre Turbine en fonction du débit qu'elle turbinera
	std::vector<double> vProduction;
	vProduction.resize(DebitTotal / Centrale[0].Discretisation + 1);

	for (int i = 0; vProduction.size(); i++) {
		debit = i * Centrale[0].Discretisation;

		//On vérifie que notre débit ne dépasse pas le débit max de notre Turbine
		if (debit <= Centrale[0].DebitMax) {
			Centrale[0].Etats[i] = Centrale[0].DebitMax;
			Centrale[0].ChuteNette[i] = HauteurChutteNette(ElevationAmont, ElevationAval, debit);
			//On calcul notre production en fonction du débit turbiné et du débit turbiné restant 
			vProduction[i] = ProductionT1(Centrale[0].ChuteNette[i], debit) + Centrale[1].Production[DebitTotal/5 - i];
		}
		else {
			// On calcul notre production en fonction du débit max et du débit turbiné restant
			indice = Centrale[0].DebitMax / Centrale[0].Discretisation;
			std::cout << "indice = " << indice << ", size = " << Centrale[0].ChuteNette.size() << "\n";
			vProduction[i] = ProductionT1(Centrale[0].ChuteNette[indice], Centrale[0].DebitMax) + Centrale[1].Production[(DebitTotal - Centrale[0].DebitMax) / 5];
		}
	}

	int scoreTotale = 0;
	int Decision = 0;
	//On cherche la plus grande valeure de production
	for (int eDecision = 0; eDecision < vProduction.size(); eDecision++) {
		if (vProduction[eDecision] > scoreTotale) {
			scoreTotale = vProduction[eDecision];
			if (eDecision * 5 <= Centrale[0].DebitMax) {
				Decision = eDecision;
			}
		}
	}
	//On renseigne notre variable de décision
	Centrale[0].VariableDecision[0] = Decision;
	Centrale[0].Production[0] = scoreTotale;
}

void Forward(std::vector<Turbine> Centrale, std::vector<double>& Solution, int DebitTotale)
{
	//Initialisation de la variable contenant le débit restant à débiter
	int debitRestant = DebitTotale/5;

	//On remplit notre vecteur solution contenant débit turbiné par chaque turbine
	Solution[0] = Centrale[0].VariableDecision[0];
	for (int i = 1; i < 5;i++) {
		//On met à jour notre débit restant avant de récupéré le débit turbiné
		debitRestant -= Solution[i - 1];
		Solution[i] = Centrale[i].VariableDecision[debitRestant];
	}
}
	
//Fonction retournant la Production de la Turbine 1 en fonctio d'une Hauteur et d'un Débit
double ProductionT1(double HCN, double Q)
{
	return (13.44 * HCN + 0.1889 * Q - 0.1935 * HCN * HCN - 0.02236 * HCN * Q + 0.005538 * Q * Q + 0.0004944 * HCN * HCN * Q - 3.527 * pow(10,-5) * HCN * Q * Q - 1.594 * pow(10,-5) * Q * Q * Q - 233.3);
}

//Fonction retournant la Production de la Turbine 2 en fonctio d'une Hauteur et d'un Débit
double ProductionT2(double HCN, double Q)
{
	return (0.05147 * HCN * HCN + 0.01775 * HCN * Q - 1.112 * pow(10,-4) * Q * Q - 4.138 * HCN - 0.2848 * Q + 81.59);
}

//Fonction retournant la Production de la Turbine 3 en fonctio d'une Hauteur et d'un Débit
double ProductionT3(double HCN, double Q)
{
	return (0.1159 * HCN * HCN + 7.561 * pow(10,-3) * HCN * Q - 5.412 * pow(10,-4) * Q * Q - 7.882 * HCN - 0.1156 * Q - 134);
}

//Fonction retournant la Production de la Turbine 4 en fonctio d'une Hauteur et d'un Débit
double ProductionT4(double HCN, double Q)
{
	return (0.292 * HCN * HCN + 0.06985 * HCN * Q + 2.132 * pow(10,-4) * Q * Q - 28.21 * HCN - 2.143 * Q + 629);
}

//Fonction retournant la Production de la Turbine 5 en fonctio d'une Hauteur et d'un Débit
double ProductionT5(double HCN, double Q)
{
	return (-0.05268 * HCN * HCN + 0.01091 * HCN * Q - 4.945 * pow(10,-4) * Q * Q + 3.403 * HCN - 0.01165 * Q - 54.64);
}

double HauteurChutteNette(double uneElevationAmmont, double uneElevationAval, double unDebit)
{
	return uneElevationAmmont - uneElevationAval - 0.5 * pow(10,-5) * unDebit * unDebit;
}

void RemplissageMatrice(Turbine& uneTurbine, int NumeroTurbine, std::vector<Turbine> uneCentrale)
{
	int debit=0;
	int debit2 = 0;

	for (int i = 0; i < uneTurbine.MatriceDecision.size(); i++) {
		
		debit = i * uneTurbine.Discretisation;
		
		//On vérifie que la valeur de débit restant à turbiner reçu par notre turbine ne dépasse pas le Débit Max de notre turbine
		if (debit <= uneTurbine.DebitMax) {
			for (int y = 0; y < uneTurbine.MatriceDecision[i].size(); y++) {
				//On Remplis notre matrice qui prends en colonne ce qu'il reste à débiter et en ligne ce que notre turbine va  débiter
				//On calcul donc la production de cette turbine pour le débit turbiné et on y ajoute ce qu'aurait produit la turbine suivante avec le débit restant
				if (NumeroTurbine == 3) {
					uneTurbine.MatriceDecision[i][y] = ProductionT4(uneTurbine.ChuteNette[y], uneTurbine.Etats[y]) + uneCentrale[NumeroTurbine + 1].Production[i - y];
				}
				if (NumeroTurbine == 2) {
					uneTurbine.MatriceDecision[i][y] = ProductionT3(uneTurbine.ChuteNette[y], uneTurbine.Etats[y]) + uneCentrale[NumeroTurbine + 1].Production[i - y];
				}
				if (NumeroTurbine == 1) {
					uneTurbine.MatriceDecision[i][y] = ProductionT2(uneTurbine.ChuteNette[y], uneTurbine.Etats[y]) + uneCentrale[NumeroTurbine + 1].Production[i - y];
				}
			}
		}
		else {
			for (int y = 0; y < uneTurbine.MatriceDecision[i].size(); y++){
				debit2 = y * uneTurbine.Discretisation;
				//On Remplis notre matrice qui prends en colonne ce qu'il reste à débiter et en ligne ce que notre turbine va  débiter
				//On vérifie que l'on ne puisse pas débiter plus que le débit max de notre turbine
				if (debit2 <= uneTurbine.DebitMax) {
					//On calcul donc la production de cette turbine pour le débit turbiné et on y ajoute ce qu'aurait produit la turbine suivante avec le débit restant

					if (NumeroTurbine == 3) {	
						uneTurbine.MatriceDecision[i][y] = ProductionT4(uneTurbine.ChuteNette[y], y * 5) + uneCentrale[NumeroTurbine + 1].Production[i - y];
					}
					if (NumeroTurbine == 2) {
						uneTurbine.MatriceDecision[i][y] = ProductionT3(uneTurbine.ChuteNette[y], y * 5) + uneCentrale[NumeroTurbine + 1].Production[i - y];
					}
					if (NumeroTurbine == 1) {
						uneTurbine.MatriceDecision[i][y] = ProductionT2(uneTurbine.ChuteNette[y], y * 5) + uneCentrale[NumeroTurbine + 1].Production[i - y];
					}
				}
				else {
					//On calcul donc la production de cette turbine pour le débit max qu'elle peut turbiner et on y ajoute ce qu'aurait produit la turbine suivante avec le débit restant

					if (NumeroTurbine == 3) {
						uneTurbine.MatriceDecision[i][y] = ProductionT4(uneTurbine.ChuteNette[uneTurbine.ChuteNette.size() - 1], uneTurbine.DebitMax) + uneCentrale[NumeroTurbine + 1].Production[i - uneTurbine.DebitMax/5];
					}
					if (NumeroTurbine == 2) {
						uneTurbine.MatriceDecision[i][y] = ProductionT3(uneTurbine.ChuteNette[uneTurbine.ChuteNette.size() - 1], uneTurbine.DebitMax) + uneCentrale[NumeroTurbine + 1].Production[i - uneTurbine.DebitMax / 5];
					}
					if (NumeroTurbine == 1) {
						uneTurbine.MatriceDecision[i][y] = ProductionT2(uneTurbine.ChuteNette[uneTurbine.ChuteNette.size()- 1], uneTurbine.DebitMax) + uneCentrale[NumeroTurbine + 1].Production[i - uneTurbine.DebitMax / 5];
					}
				}
			}
		}
	}
}

void RemplissageDecisionProduction(Turbine& uneTurbine)
{
	//Pour chaque Etat possible, on vérifie quelle débit produit par cette turbine, en prenant en compte la production des turbine suivantes, est le plus grand
	for (int eEtat=0; eEtat < uneTurbine.MatriceDecision.size();eEtat++) {
		int score = 0;
		int Decision = 0;
		for (int eDecision = 0; eDecision < uneTurbine.MatriceDecision[eEtat].size(); eDecision++) {
			if (uneTurbine.MatriceDecision[eEtat][eDecision] > score) {
				score = uneTurbine.MatriceDecision[eEtat][eDecision];
				if (eDecision * 5 <= uneTurbine.DebitMax) {
					Decision = eDecision;
				}
				
			}
		}
		//Cette plus grande valeure devient notre Variable de décision
		uneTurbine.VariableDecision[eEtat] = Decision;
		uneTurbine.Production[eEtat] = score;
	}
}





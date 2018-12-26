/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: victor
 *
 * Created on September 30, 2018, 8:08 PM
 */


#include <ctime>
#include "link.h"
#include "node.h"
#include "readNetwork.h"
#include "MCFP.h"
#include "route.h"
#include "RandomRoute.h"
#include "restrictedNetwork.h"
#include "optimization.h"
#include "consolidateRoutes.h"
#include "RepairRoutes.h"
#include "objectiveEvaluation.h"
#include "localsearch.h"
#include "Shaking.h"
#include <thread>

using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	double incumbent, icbt_prime;
	int neighbor=0;
	srand(time(NULL));
	cout << "Start!" << endl;
	readNetwork("C:/Users/victo/Downloads/");
	cout << "Finish reading the network!" << endl;
	clock_t start = clock();
	genInitialRouteLinkSet();
	createResNetwork();
	GUROBIinstance optModel;
	optModel.createModel();
	optModel.solveModel();
	incumbent = optModel.setPostiveLinkResults();
	//optModel.setSolution();
	//incumbent = unmetCost();
	cout << incumbent << '\n';
	
	vector<rebalancingRoute> stationSeqSet;
	
	do{
		neighbor++;
		cout << "\nNeighbor " << neighbor << '\n';	
		stationSeqSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			rebalancingRoute RR = Shaking(routes[i], neighbor);
			stationSeqSet.push_back(RR);
		}		

		const int MAXLS = 20;  // maximum number of local search
		int lsCounter = 0;		// localsearch counter
		do {
			lsCounter++;
			int nodeNB, linkNB; //node neighbor and link neighbor

			//intra operations
			for (int i = 0; i<routes.size(); i++) {
				if (stationSeqSet[i].length >= 3) {
					stationSeqSet[i].route = stationSeqSet[i].oroute; //set to original route
					stationSeqSet[i].intraNS(1 + rand() % min(3, stationSeqSet[i].length - 1)); // node neighbor restricted to 1 to 3
					stationSeqSet[i].intraRI(1 + rand() % min(3, stationSeqSet[i].length - 2)); // link neighbor restricted to 1 to 3
					stationSeqSet[i].intraIC(1 + rand() % min(3, stationSeqSet[i].length - 3)); // link neighbor restricted to 1 to 3				
				}
			}
			//inter operations
			for (int i = 0; i < routes.size()/2; i++) {
				int r1, r2, x;
				r1 = rand() % routes.size();
				do {
					r2 = rand() % routes.size();
				} while (r1 == r2);
				
				x = rand() % 3;

				switch (x) {
					if (stationSeqSet[r1].length > 3 && stationSeqSet[r1].length > 3) {
						case 0:
							//cout << "\ncase 0\n";
							stationSeqSet[r1].interNS(1 + rand() % 3, stationSeqSet[r2]);
						case 1:
							//cout << "\ncase 1\n";
							stationSeqSet[r1].interCR(1 + rand() % 3, stationSeqSet[r2]);
						case 2:
							//cout << "\ncase 2\n";
							stationSeqSet[r1].interCT(stationSeqSet[r2]); 
					}
					else {
						break;
					}
				}
				if (abs(stationSeqSet[r1].length - stationSeqSet[r2].length) > 10) {
					stationSeqSet[r1].interSH(1 + rand() % 3, stationSeqSet[r2]); // only used when unbalanced routes exist
				}
			}
			routeLinkSet.clear();
			for (int i = 0; i < routes.size(); i++) {
				vector<string> canR = constructRoute(stationSeqSet[i].route);
				for (auto iter = canR.begin(); iter != canR.end(); iter++) {
					routeLinkSet.push_back((*iter));
				}
			}

			createResNetwork();
			optModel.updateModel();
			optModel.solveModel();
			//optModel.setSolution();
			icbt_prime = optModel.setPostiveLinkResults();
			//icbt_prime = unmetCost();
			cout << icbt_prime << '\n';

			if (icbt_prime <= incumbent) { // move to new solution, set shaking neighbor back to 1, update incumbent solution
				cout << icbt_prime/20 << '\n';
				incumbent = icbt_prime; 
				routes.clear();
				for (int i = 0; i < stationSeqSet.size(); i++) {
					routes.push_back(stationSeqSet[i].route);
				}
				neighbor = 0;
				break;
			}
		} while (lsCounter < MAXLS);

	} while (neighbor<5);

	cout << '\n';
	for (auto iter = routes.begin(); iter != routes.end(); iter++) {
		vector<string> constructedR;
		constructedR = constructRoute(*(iter));
		for (auto it = constructedR.begin(); it != constructedR.end(); it++) {
			cout << *(it) << '\t';
		}
		cout << '\n';
	}

	optModel.deleteModel();
	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "\nTotal time" << time;
	return 0;
}

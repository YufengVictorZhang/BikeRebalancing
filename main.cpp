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

using namespace std;


int main(int argc, char** argv) {
	
	double incumbent, icbt_prime;
	int neighbor=0;

	cout << "Start!" << endl;
	readNetwork("C:/Users/victo/Downloads/");
	cout << "Finish reading the network!" << endl;
	clock_t start = clock();
	genInitialRouteLinkSet();
	createResNetwork();
	GUROBIinstance optModel;
	optModel.createModel();
	optModel.solveModel();
	optModel.setPostiveLinkResults();
	optModel.setSolution();
	incumbent = unmetCost();
	cout << incumbent << '\n';
	
	vector<rebalancingRoute> stationSeqSet;
	
	do{
		neighbor++;
		cout << "\nIteration " << neighbor << '\n';	
		stationSeqSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			rebalancingRoute RR = Shaking(routes[i], neighbor);
			stationSeqSet.push_back(RR);
		}		

		const int MAXLS = 50;  // maximum number of local search
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
			optModel.setSolution();
			optModel.setPostiveLinkResults();
			icbt_prime = unmetCost();
			cout << icbt_prime << '\n';

			if (icbt_prime <= incumbent) { // move to new solution, set shaking neighbor back to 1, update incumbent solution
				cout << icbt_prime << '\n';
				incumbent = icbt_prime; 
				routes.clear();
				for (int i = 0; i < stationSeqSet.size(); i++) {
					routes.push_back(stationSeqSet[i].route);
				}
				neighbor = 0;
				break;
			}
		} while (lsCounter < MAXLS);

	} while (neighbor<=5);

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

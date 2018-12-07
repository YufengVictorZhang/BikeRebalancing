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
//#include "RestrictedMCFP.h"
//#include "respartialMCFP.h"
#include "consolidateRoutes.h"
#include "objectiveEvaluation.h"

#include "RepairRoutes.h"
#include "localsearch.h"

using namespace std;




int main(int argc, char** argv) {
	
	cout << "Start!" << endl;
	readNetwork("C:/Users/victo/Downloads/");
	cout << "Finish reading the network!" << endl;
	genInitialRouteLinkSet();
	createResNetwork();
	GUROBIinstance optModel;
	optModel.createModel();
	optModel.solveModel();
	optModel.printResults();
	optModel.setSolution();
	double c = unmetCost();
	cout << c << '\n';
	consolidateRoutes();
	for (int i = 0; i <= 5; i++) {
		cout << "\nIteration " << i << '\n';
		vector<rebalancingRoute> stationSeqSet;
		stationSeqSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			vector<string> stationSeq = getStationSequence(routes[i]);
			rebalancingRoute *RR = new rebalancingRoute(stationSeq);
			stationSeqSet.push_back(*RR);
			delete RR;
			/*for (auto iter = stationSeqSet.begin(); iter != stationSeqSet.end(); iter++) {
				cout << "\n Route"<< distance(stationSeqSet.begin(), iter) <<":\n";
				for (auto it = iter->route.begin(); it != iter->route.end(); it++) {
					cout << (*it) << '\t';
				}
			}*/
		}


		stationSeqSet[0].intraRI(3);
		stationSeqSet[1].intraIC(3);
		stationSeqSet[2].intraNS(2);
		stationSeqSet[0].interCR(3, stationSeqSet[1]);
		stationSeqSet[1].interNS(3, stationSeqSet[2]);
		stationSeqSet[2].interSH(2, stationSeqSet[3]);
		stationSeqSet[3].interCT(stationSeqSet[4]);

		routeLinkSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			vector<string> canR = constructRoute(stationSeqSet[i].route);
			for (auto iter = canR.begin(); iter != canR.end(); iter++) {
				routeLinkSet.push_back((*iter));
			}
		}
		
		createResNetwork();
		optModel.updateModel();
		optModel.setSolution();
		c = unmetCost();
		cout << c << '\n';
	}

	//printRepairedRoute();
	//for (int i = 0; i <= 10; i++) {
	//	generateRouteSet();
	//	createResNetwork();
	//	//solveResMCFP();
	//	solveResParMCFP();
	//	cout << "Wait!" << '\n';
	//	consolidateRoutes();
	//	vector<string> station = getStationSequence(routes[0]);
	//	vector<string> station2 = getStationSequence(routes[1]);
	//  rebalancingRoute reb_route(station);
	//  rebalancingRoute reb_route2(station2);
	//	for (auto iter = station.begin(); iter != station.end(); iter++) {
	//		cout << (*iter) << '\t';
	//	}
	//	cout << '\n';
	//	rebalancingRoute reb_route(station);
	//	rebalancingRoute reb_route2(station2);

	//	reb_route.intraRI(3);
	//	reb_route.intraIC(3);
	//	reb_route.intraNS(2);
	//	reb_route.interCR(3, reb_route2);
	//	reb_route.interNS(3, reb_route2);
	//	reb_route.interSH(2, reb_route2);
	//	reb_route.interCT(reb_route2);
	//}
	//
	//double c = unmetCost();
	//cout << c << '\n';
	//c = totalCost();
	//cout << c << '\n';
	optModel.deleteModel();
	return 0;
	
}

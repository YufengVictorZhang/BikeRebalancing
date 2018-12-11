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
#include "RepairRoutes.h"
#include "objectiveEvaluation.h"

#include "localsearch.h"

using namespace std;


int main(int argc, char** argv) {
	
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
	double c = unmetCost();
	cout << c << '\n';
	consolidateRoutes();
	//constructRoute(routes[0]);
	//constructRoute(routes[1]);

	for (int i = 0; i <= 10; i++) {
		cout << "\nIteration " << i << '\n';
		vector<rebalancingRoute> stationSeqSet;
		stationSeqSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			rebalancingRoute *RR = new rebalancingRoute(routes[i]);
			stationSeqSet.push_back(*RR);
			delete RR;
		}		
		
		stationSeqSet[0].intraRI(3);
		stationSeqSet[0].intraRI(2);
		stationSeqSet[0].intraRI(1);
		stationSeqSet[1].intraIC(3);
		stationSeqSet[0].intraIC(3);
		stationSeqSet[0].intraIC(2);
		stationSeqSet[0].intraIC(1);
		stationSeqSet[3].intraIC(1);
		stationSeqSet[1].intraIC(1);
		stationSeqSet[2].intraIC(1);
		stationSeqSet[0].intraNS(2);
		stationSeqSet[1].intraNS(2);
		stationSeqSet[2].intraNS(2);
		stationSeqSet[2].intraNS(1);
		
		cout<<"this is a new branch\n"


		stationSeqSet[0].interCR(1, stationSeqSet[1]);
		stationSeqSet[1].interCR(1, stationSeqSet[2]);
		stationSeqSet[0].interCR(2, stationSeqSet[1]);
		stationSeqSet[1].interCR(2, stationSeqSet[2]);
		stationSeqSet[0].interCR(3, stationSeqSet[1]);
		stationSeqSet[1].interCR(3, stationSeqSet[2]);
		
		
		stationSeqSet[0].interNS(1, stationSeqSet[1]);
		stationSeqSet[1].interNS(1, stationSeqSet[2]);
		stationSeqSet[2].interNS(1, stationSeqSet[3]);
		stationSeqSet[0].interNS(2, stationSeqSet[1]);
		stationSeqSet[1].interNS(2, stationSeqSet[2]);
		stationSeqSet[2].interNS(2, stationSeqSet[3]);
		stationSeqSet[0].interNS(3, stationSeqSet[1]);
		stationSeqSet[1].interNS(3, stationSeqSet[2]);
		stationSeqSet[2].interNS(3, stationSeqSet[3]);

		stationSeqSet[0].interCT(stationSeqSet[1]);
		stationSeqSet[1].interCT(stationSeqSet[2]);
		stationSeqSet[2].interCT(stationSeqSet[3]);
		stationSeqSet[5].interCT(stationSeqSet[1]);

		stationSeqSet[0].interSH(1, stationSeqSet[1]);
		stationSeqSet[0].interSH(2, stationSeqSet[1]);
		stationSeqSet[0].interSH(3, stationSeqSet[1]);
		stationSeqSet[4].interSH(3, stationSeqSet[1]);
		stationSeqSet[5].interSH(3, stationSeqSet[2]);
		stationSeqSet[6].interSH(3, stationSeqSet[3]);

		routeLinkSet.clear();
		for (int i = 0; i < routes.size(); i++) {
			vector<string> canR = constructRoute(stationSeqSet[i].route);
			//cout << i << '\n';
			for (auto iter = canR.begin(); iter != canR.end(); iter++) {
				routeLinkSet.push_back((*iter));
			}
		}

		/*for (auto iter = routeLinkSet.begin(); iter != routeLinkSet.end(); iter++) {
			cout << (*iter) << '\t';
		}
		cout << '\n';*/

		createResNetwork();
		optModel.updateModel();
		optModel.solveModel();
		optModel.setSolution();
		optModel.setPostiveLinkResults();
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
	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "\nTotal time" << time;
	return 0;
	
}

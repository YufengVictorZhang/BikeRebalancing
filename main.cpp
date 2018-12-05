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
//#include "RestrictedMCFP.h"
#include "respartialMCFP.h"
#include "consolidateRoutes.h"
#include "objectiveEvaluation.h"
#include "RepairRoutes.h"
#include "localsearch.h"

using namespace std;

int main(int argc, char** argv) {
    
    //clock_t begin = clock();
	cout << "Start!" << endl;
	//readNetwork("C:/users/zhan4879/Google Drive/Bike Sharing System/Niceride Station Distance Calculation/");
	readNetwork("C:/Users/victo/Downloads/");
	cout << "Finish reading the network!" << endl;
	generateRouteSet();
	createResNetwork();
	//solveResMCFP();
	solveResParMCFP();
	cout << "Wait!" << '\n';
	consolidateRoutes();
	vector<string> station = getStationSequence(routes[0]);
	vector<string> station2 = getStationSequence(routes[1]);
	for (auto iter = station.begin(); iter != station.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << '\n';
	
	//printRepairedRoute();
	for (int i = 0; i <= 10; i++) {
		rebalancingRoute reb_route(station);
		rebalancingRoute reb_route2(station2);
		reb_route.intraRI(3);
		reb_route.intraIC(3);
		reb_route.intraNS(2);
		reb_route.interCR(3, reb_route2);
	}
	
	//double c = unmetCost();
	//cout << c << '\n';
	//c = totalCost();
	//cout << c << '\n';
	return 0;
	
}

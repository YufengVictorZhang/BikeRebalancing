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
//#include "MCFP.h"
#include "route.h"
#include "RandomRoute.h"
#include "restrictedNetwork.h"
#include "RestrictedMCFP.h"

using namespace std;

int main(int argc, char** argv) {
    
    //clock_t begin = clock();
	cout << "Start!" << endl;
	//readNetwork("C:/users/zhan4879/Google Drive/Bike Sharing System/Niceride Station Distance Calculation/");
	readNetwork("C:/Users/victo/Downloads/");
	cout << "Finish reading the network!" << endl;
	generateRouteSet();
	createResNetwork();
	solveResMCFP();
	cout << "Wait!" << '\n';

	/*list<string> route1 = randomPath();

	for (auto iter = route1.begin(); iter != route1.end(); iter++) {
		cout << *iter << '\n';
	}*/
	//cout << route1 << '\n';
	//int counter = 0;
	//for (auto iter = links.begin(); iter != links.end(); iter++) {
	//	//cout << (*iter).first << '\t' << (*iter).second->getType() << '\n' ;
	//	if ((*iter).second->getType() == "transship") {
	//		counter++;
	//	}
	//}
	//cout << counter << '\n';
    //clock_t end = clock();	
	//solveResMCFP();

	/*cout << "Finish solving MCFP!" << endl;
	updateMCFPCost(10.0);
	cout << "Finish updating MCFP cost!" << endl;
	SP("D1");
	cout << "Finish SP" << endl;
	list<string>* path;
	path = getPath("D2");
	cout << path->size() << endl;*/

	/*for (auto iter = (*path).begin(); iter != (*path).end(); iter++) {
		cout << (*iter) << '\n';
	}
	cout << links["D1,31_0"]->getMCFPsol()<<'\n';
	cout << links["31_0,32_1"]->getMCFPsol() << '\n';
	cout << links["32_3,123_4"]->getMCFPsol() << '\n';
	cout << links["123_7,129_8"]->getMCFPsol() << '\n';*/

//	for (auto iter = links.begin(); iter != links.end(); iter++) {
//		
//		vector<string> tokens;
//		stringstream ss((*iter).first);
//		while (ss.good()) {
//			string substr;
//			getline(ss, substr, ',');
//			stringstream sub(substr);
//			while (getline(sub, substr, '_')) {
//				tokens.push_back(substr);
//			}
//		}
//		
//		if (tokens[0] == "D1") {
//			cout << (*iter).first << '\t';
//			cout << links[(*iter).first]->getMCFPsol() << '\n';
//		}
//		
//	}
//	//cout << (*path) << endl;
//	//cout << links[(*path).front()+","+(path[1])]->getMCFPsol() << endl;
//	//cout << links["99_53,99_54"]->getMCFPsol()<<endl;
//	//cout << double(end - begin) / CLOCKS_PER_SEC << '\n';
//	
//	
//	return 0;
//
}

#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <list>
#include <ctime>
#include <stdlib.h>

using namespace std;

vector<string> routeLinkSet;   //used to restore generated vehicle routes, including the initially randomaly generated ones

vector< vector<string> > routes;


vector<string> getStationSequence(vector<string> &route) {
	vector<string> station;
	string lastStation = "";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		string link = (*iter);
		string fromNode, toNode, fromStation, toStation;
		fromNode = link.substr(0, link.find(","));
		toNode = link.substr(link.find(",") + 1, link.length());
		fromStation = fromNode.substr(0, fromNode.find("_"));
		toStation = toNode.substr(0, toNode.find("_"));

		if (fromStation == toStation || fromStation != lastStation) {
			station.push_back(fromStation);
		}
		station.push_back(toStation);
		lastStation = toStation;
	}
	return station;
}

const vector<string> randomRoute() {	
	vector<string> route;
	//route.clear();
	string currentNode, currentLink;
	currentNode = "D1";
	currentLink = "";
	while(currentNode!="D2"){
		int size, index;
		size = int(nodes[currentNode]->getOutLinks().size());
		index = rand() % size;
		currentLink = nodes[currentNode]->getOutLinks()[index];
		route.push_back(currentLink);
		currentNode = currentLink.substr(currentLink.find(",")+1, currentLink.length());
	}
	/*cout << "RandomRoute\n";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		cout << *(iter) << '\t';
	}
	cout << '\n';*/
	
	return route;
}

void genInitialRouteLinkSet() {
	routeLinkSet.clear();
	srand(time(NULL));
	for (int i = 0; i<10; i++) {
		vector<string> singleroute;
		vector <string> routewodepot;
		singleroute = randomRoute();
		routewodepot.insert(routewodepot.begin(),singleroute.begin()+1,singleroute.end()-1);
		routes.push_back(getStationSequence(routewodepot));
		for (auto iter = singleroute.begin(); iter!= singleroute.end(); iter++) {
			routeLinkSet.push_back(*iter);
			//cout << *iter << '\n';
		}
	}
}

//void genRouteLinkSet() {
//	routeLinkSet.clear();
//	for (auto iter = ) {
//		routeLinkSet.push_back((*iter));
//	}
//}


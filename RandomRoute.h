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

list<string> routeLinkSet;   //used to restore generated vehicle routes, including the initially randomaly generated ones

const list<string> randomRoute() {	
	list<string> route;
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
	return route;
}

void genInitialRouteLinkSet() {
	routeLinkSet.clear();
	srand(time(NULL));
	for (int i = 0; i<10; i++) {
		list<string> singleroute;
		singleroute = randomRoute();
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


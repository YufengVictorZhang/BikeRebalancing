#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <list>
#include <time.h>

using namespace std;

list<string> routeSet;
list<string> route;

const list<string>& randomPath() {	
	route.clear();
	srand(time(NULL));
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

void generateRouteSet() {
	routeSet.clear();
	for (int i = 0; i <10; i++) {
		list<string> singleroute;
		singleroute = randomPath();
		for (auto iter = singleroute.begin(); iter!= singleroute.end(); iter++) {
			routeSet.push_back(*iter);
		}
	}
}


#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <list>

using namespace std;


const list<string>& randomPath() {	
	list<string> path;
	string currentNode, currentLink;
	currentNode = "D1";
	currentLink = "";
	while(currentNode!="D2"){
		int size, index;
		size = int(nodes[currentNode]->getOutLinks().size());
		index = rand() % size;
		cout << index;
		currentLink = nodes[currentNode]->getOutLinks()[index];
		cout << currentLink << '\n';
		path.push_back(currentLink);
		currentNode = currentLink.substr(1, currentLink.find(","));
	}	
	return path;
}
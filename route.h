#pragma once
#ifndef ROUTE_H
#define ROUTE_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>

using namespace std;

const string Depot_1 = "D1";  //Depot at time zero
const string Depot_2 = "D2";  // Depot at the ending time

void SP(string _node) {  //label correcting algorithm
	cout << "Starting SP algorithm!" << endl;

	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		nodes[(*iter).first]->setLabel(10000.0);
	}
	nodes[_node]->setLabel(0.0);
	
	list<string> SEL;
	SEL.push_back(_node);
	cout << clock() / CLOCKS_PER_SEC << endl;
	while (!SEL.empty()) {
		for (int i = 0; i < nodes[SEL.front()]->getOutLinks().size(); i++){
			//cout << clock() << endl;
			string l = nodes[SEL.front()]->getOutLinks()[i];
			stringstream ss (l);
			vector<string> tokens;
			string buf;
			while (ss.good()) {
				getline(ss, buf, ',');
				tokens.push_back(buf);
			}
			//cout << clock() << endl;
			if (nodes[tokens[1]]->getLabel() > nodes[tokens[0]]->getLabel() + links[l]->getMCFPcost()) {
				SEL.push_back(tokens[1]);
				nodes[tokens[1]]->setPred(tokens[0]);
				nodes[tokens[1]]->setLabel(nodes[tokens[0]]->getLabel() + links[l]->getMCFPcost());
			}
			//cout << clock() << endl;
		}
		SEL.pop_front();
	}
	cout << clock()/ CLOCKS_PER_SEC<<endl;
}


void updateMCFPCost(float _alpha) {
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		links[(*iter).first]->setMCFPcost(links[(*iter).first]->getDistance() - _alpha * links[(*iter).first]->getMCFPsol());
	}
}

list<string>* getPath(string _node) { //_node is destination
	list<string>* path;
	path = new list<string>;
	path->push_front(_node);
	while(nodes[_node]->getPred() != Depot_1) {
		path->push_front(nodes[_node]->getPred());
		_node = nodes[_node]->getPred();
	}
	path->push_front(Depot_1);
	return path;
}

void getResidualMCFP() {
	
}




//void updateCost() {
//
//}

#endif 
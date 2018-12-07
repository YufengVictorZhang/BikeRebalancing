#pragma once

#include <vector>
#include <string>

using namespace std;

double travelCost() {
	double c = 0;
	for (auto iter = routeLinkSet.begin(); iter != routeLinkSet.end(); iter++) {
		string route = (*iter);
		c += links[route]->getDistance();
	}
	return c;
}

double unmetCost() {
	double c = 0;
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		string node = (*iter).first;
		if (node != "D1" && node != "D2") {
			int time = stoi(node.substr(node.find("_") + 1, node.length()));
		
			if (time == 29 || time == 59) {
				double inflow = 0;
				double outflow = 0;
				int demand = nodes[node]->getDemand();

				for (auto iter = nodes[node]->getInLinks().begin(); iter != nodes[node]->getInLinks().end(); iter++) {
					inflow += links[(*iter)]->getMCFPsol();
				}
				
				for (auto iter = nodes[node]->getOutLinks().begin(); iter != nodes[node]->getOutLinks().end(); iter++) {
					outflow += links[(*iter)]->getMCFPsol();
				}
				
				if (demand <= 0) {
					//cout << "demand < 0" << '\t'<<demand<<'\n';
					if (inflow - outflow + demand < 0) {
						//cout << inflow << '\t' << outflow << '\n';
						c += -(inflow - outflow + demand);
					}
				}

				else {
					//cout << "demand > 0" << '\t' << demand << '\n';
					if (nodes[node]->getCapacity() - inflow + outflow < demand) {
						c += -(nodes[node]->getCapacity() - inflow + outflow - demand);
					}
				}
			}
		}
	}
	return alpha * c;
}


double totalCost() {
	double c = 0;
	c = travelCost() + unmetCost();
	return c;
}
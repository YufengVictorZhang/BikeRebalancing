#pragma once


#include <vector>
#include <string>

using namespace std;

//void checkFeasibility(string _fromStation, string _toStation) {
//	
//}

vector<string> getStationSequence(vector<string> &route) {
	vector<string> station;
	string lastStation = "";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		string link = (*iter);
		string fromNode, toNode, fromStation, toStation;
		fromNode = link.substr(0, link.find(","));
		toNode = link.substr(link.find(",")+1, link.length());
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

vector<string> constructRoute(vector<string> &station) {
	vector<string> route;
	vector<string>::iterator iter = station.begin();
	int time = 0;
	while (iter != station.end()-1) {

		string fromNode = (*iter) + "_" + to_string(time);
		if (*iter == *(iter + 1)) {
			time += 1;   //stationary link
		}
		else {
			time += traveltime[(*iter)][*(iter + 1)];
		}
		
		string toNode = (*(iter + 1)) + "_" + to_string(time);
		string link = fromNode + "," + toNode;
		route.push_back(link);
		iter = iter + 1;
	}
	return route;

}

void printRepairedRoute() {
	vector<string> rRoute;
	vector<string> rStation;
	for (auto iter = routes.begin(); iter != routes.end(); iter++) {
		rStation = getStationSequence((*iter));
		rRoute = constructRoute(rStation);
		for (auto iter = rRoute.begin(); iter != rRoute.end(); iter++) {
			cout << (*iter) << '\t';
			cout << '\n';
		}
	}
	
}
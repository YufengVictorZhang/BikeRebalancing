#pragma once


#include <vector>
#include <string>

using namespace std;

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
			//time += traveltime[(*iter)][*(iter + 1)];
			if((traveltime.find(*iter)->second).find(*(iter + 1))!= (traveltime.find(*iter)->second).end()){
				 time += traveltime[(*iter)][*(iter + 1)];
			}
		}
		
		string toNode = (*(iter + 1)) + "_" + to_string(time);
		string link = fromNode + "," + toNode;
		route.push_back(link);
		iter = iter + 1;
	}

	/*cout << '\n';
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		cout << *(iter) << '\t';
	}
	cout << '\n';*/

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
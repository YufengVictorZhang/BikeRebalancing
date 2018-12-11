#pragma once


#include<vector>
#include<string>

using namespace std;

rebalancingRoute& Shaking(const vector<string>& R, int neighbor) {
	vector<string> shakedRoute;
	int loc;
	int flag, counter;
	string newStation,oldStation;
	map<string, map<string, int>>::iterator it = traveltime.begin();
	shakedRoute = R;

	for (int i = 0; i < neighbor; i++) {
		counter = 0;
		flag = 0;
		do {
			counter++;
			loc = rand() % R.size();
			advance(it, rand() % traveltime.size());
			newStation = it->first;
			if (find(shakedRoute.begin(), shakedRoute.end(), newStation) != shakedRoute.end()) {
				oldStation = shakedRoute.at(loc);
				shakedRoute.at(loc) = newStation;
				flag = rebalancingRoute::checkSpaceFeasibility(shakedRoute);
			}
		} while (flag == 0 && oldStation == newStation && counter < MAXITER);
	}
	
	rebalancingRoute *p = new rebalancingRoute(shakedRoute);
	return *p;
}
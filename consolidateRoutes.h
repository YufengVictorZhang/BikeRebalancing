#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


struct MyStruct;
struct less_than_key;

vector<MyStruct> activeLinkSetcopy;
vector< vector<string> > routes;

struct MyStruct
{
	int key;
	string stringValue;
	MyStruct(const string& s) {
		string fromNode = s.substr(0, s.find(","));
		key = stoi(fromNode.substr(fromNode.find("_") + 1, fromNode.length()));
		stringValue = s;
	}
};

struct less_than_key
{
	inline bool operator() (const MyStruct& struct1, const MyStruct& struct2)
	{
		return (struct1.key < struct2.key);
	}
};


//void findNonOverlappingMin(string _curStation) {
//	for (auto iter = activeLinkSetcopy.begin(); iter != activeLinkSetcopy.end(); iter++) {
//		
//	}
//}


void consolidateRoutes() {

	for (auto iter = activeLinkSet.begin(); iter != activeLinkSet.end(); iter++) {
		activeLinkSetcopy.push_back(MyStruct(*iter));
	}
	sort(activeLinkSetcopy.begin(), activeLinkSetcopy.end(), less_than_key());  // sort activeLinkSetcopy

	/*for (auto iter = activeLinkSetcopy.begin(); iter != activeLinkSetcopy.end(); iter++) {
		cout << (*iter).stringValue << '\t';
	}*/


	vector<string> candidateRoute;
	while (!activeLinkSetcopy.empty()) {
		candidateRoute.push_back(activeLinkSetcopy.front().stringValue);
		activeLinkSetcopy.erase(activeLinkSetcopy.begin());
		int flag = 0;

		for (auto iter = activeLinkSetcopy.begin(); iter != activeLinkSetcopy.end();) {

			string Link = candidateRoute.back();
			string toNode = Link.substr(Link.find(",") + 1, Link.length());
			string curNode = toNode.substr(0, toNode.find("_"));
			int curTime = stoi(toNode.substr(toNode.find("_") + 1, toNode.length()));

			string nextLink = (*iter).stringValue;
			string fromNode = nextLink.substr(0, nextLink.find(","));
			string nextNode = fromNode.substr(0, fromNode.find("_"));
			int nextTime = stoi(fromNode.substr(fromNode.find("_") + 1, fromNode.length()));

			if ((curTime == nextTime && curNode == nextNode) || (curTime < nextTime)) {
				candidateRoute.push_back(nextLink);
				iter = activeLinkSetcopy.erase(iter);
				flag = 0;
			}
			else {
				iter++;
				flag = 1;
			}
		}
		routes.push_back(candidateRoute);
		candidateRoute.clear();
	}
	
	cout << "consolidated routes\n";
	for (auto iter = routes.begin(); iter != routes.end(); iter++) {
		cout << "route\t";
		vector<string> R;
		R = *iter;
		for (auto IT = R.begin(); IT != R.end(); IT++) {
			cout << (*IT) << '\t';
		}
		cout << '\n';
	}
}

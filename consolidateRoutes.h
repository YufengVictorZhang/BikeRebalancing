#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


struct MyStruct;
struct less_than_key;

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


//void consolidateRoutes() {
//	vector<MyStruct> positiveLinkSetcopy;
//	positiveLinkSetcopy.clear();
//	routes.clear();
//	for (auto iter = positiveLinkSet.begin(); iter != positiveLinkSet.end(); iter++) {
//		positiveLinkSetcopy.push_back(MyStruct(*iter));
//	}
//	sort(positiveLinkSetcopy.begin(), positiveLinkSetcopy.end(), less_than_key());  // sort activeLinkSetcopy
//
//
//	vector<string> candidateRoute;
//	while (!positiveLinkSetcopy.empty()) {
//		candidateRoute.push_back(positiveLinkSetcopy.front().stringValue);
//		positiveLinkSetcopy.erase(positiveLinkSetcopy.begin());
//		int flag = 0;
//
//		for (auto iter = positiveLinkSetcopy.begin(); iter != positiveLinkSetcopy.end();) {
//
//			string Link = candidateRoute.back();
//			string toNode = Link.substr(Link.find(",") + 1, Link.length());
//			string curNode = toNode.substr(0, toNode.find("_"));
//			int curTime = stoi(toNode.substr(toNode.find("_") + 1, toNode.length()));
//
//			string nextLink = (*iter).stringValue;
//			string fromNode = nextLink.substr(0, nextLink.find(","));
//			string nextNode = fromNode.substr(0, fromNode.find("_"));
//			int nextTime = stoi(fromNode.substr(fromNode.find("_") + 1, fromNode.length()));
//
//			if ((curTime == nextTime && curNode == nextNode) || (curTime < nextTime)) {
//				candidateRoute.push_back(nextLink);
//				iter = positiveLinkSetcopy.erase(iter);
//				flag = 0;
//			}
//			else {
//				iter++;
//				flag = 1;
//			}
//		}
//		routes.push_back(candidateRoute);
//		candidateRoute.clear();
//	}
//	
//	cout << "consolidated routes\n";
//	for (auto iter = routes.begin(); iter != routes.end(); iter++) {
//		cout << "route\t";
//		vector<string> R;
//		R = *iter;
//		for (auto IT = R.begin(); IT != R.end(); IT++) {
//			cout << (*IT) << '\t';
//		}
//		cout << '\n';
//	}
//}


void consolidateRoutes() {
	vector<string> positiveLinkSetcopy;
	positiveLinkSetcopy.clear();
	routes.clear();
	for (auto iter = positiveLinkSet.begin(); iter != positiveLinkSet.end(); iter++) {
		string fromStation, toStation, fromNode, toNode;
		fromNode = (*iter).substr(0, (*iter).find(","));
		toNode = (*iter).substr((*iter).find(",")+1,(*iter).length());
		fromStation = fromNode.substr(0, fromNode.find("_"));
		toStation = toNode.substr(0, toNode.find("_"));
		positiveLinkSetcopy.push_back(fromStation);
		positiveLinkSetcopy.push_back(toStation);
	}

	while (!positiveLinkSetcopy.empty()) {
		vector<string> candidateRoute;
		int time = 0;
		int flag = 0;
		vector<string>::iterator iter = positiveLinkSetcopy.begin();
		candidateRoute.push_back(positiveLinkSetcopy[0]);
		candidateRoute.push_back(positiveLinkSetcopy[1]);
		time += traveltime[positiveLinkSetcopy[0]][positiveLinkSetcopy[1]];
		iter = positiveLinkSetcopy.erase(iter);
		iter = positiveLinkSetcopy.erase(iter);

		while (time< 60-4 && iter!=positiveLinkSetcopy.end()) {  //2*2 = 4
			string curStation = candidateRoute.back();
			string nextStation = *(iter);
			if (curStation == nextStation) {
				candidateRoute.push_back(positiveLinkSetcopy[1]);
				time += traveltime[*iter][*(iter + 1)];
				iter = positiveLinkSetcopy.erase(iter);
				iter = positiveLinkSetcopy.erase(iter);
			}
			else {
				if (accMap[curStation].find(nextStation) != accMap[curStation].end() && accMap[curStation][nextStation].size() < 3) {
					if (accMap[curStation][nextStation].size() == 0) {
						time += traveltime[curStation][nextStation];
						candidateRoute.push_back(*iter);
						candidateRoute.push_back(*(iter + 1));
						time += traveltime[*iter][*(iter + 1)];
						iter = positiveLinkSetcopy.erase(iter);
						iter = positiveLinkSetcopy.erase(iter);
					}
					else {
						string lastStation = curStation;
						for (auto it = accMap[curStation][nextStation].begin(); it != accMap[curStation][nextStation].end(); it++) {
							candidateRoute.push_back(*(it));
							time += traveltime[lastStation][*(it)];
							lastStation = *(it);
						}
						time += traveltime[lastStation][nextStation];
						candidateRoute.push_back(*(iter));
						candidateRoute.push_back(*(iter+1));
						time += traveltime[*iter][*(iter + 1)];
						iter = positiveLinkSetcopy.erase(iter);
						iter = positiveLinkSetcopy.erase(iter);
					}
				}
				else {
					iter += 2;
				}
			}
		}
		cout << time << '\n';
		cout << positiveLinkSetcopy.size()<<'\n';
		routes.push_back(candidateRoute);
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
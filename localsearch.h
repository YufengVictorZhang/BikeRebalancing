/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   localsearch.h
 * Author: victor
 *
 * Created on September 30, 2018, 8:18 PM
 */

#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdlib.h>

using namespace std;
const int MAXITER = 100;

class rebalancingRoute {
		
	public:
		vector<string> route;			//route is comprised of a sequence of non-time-indexed nodes
		vector<string> oroute;			// used to store original route before any local operations
		int length;
		 
		rebalancingRoute(vector<string> &);

		void intraRI(int);				// parameter is number of links in the subsequence
										// reinsert subsequence to a different location
		void intraIC(int);				// interchange two subsequence of links
		//void intraAD(int);			    // adjust route, add new links into the routes
		void intraNS(int);				// node swapping

		void interCR(int, rebalancingRoute &);
		void interNS(int, rebalancingRoute &);
		void interSH(int, rebalancingRoute &);  //shift a subsequence of one route to another route
		void interCT(rebalancingRoute &);  //concatenating two routes by cutting down them into two parts

		int checkTimeFeasibility();
		static int checkSpaceFeasibility(vector<string>&);
		void updateLength();
		void operator= (const vector<string>& vec) {  // overload operator = and update length of it
			route.clear();
			oroute.clear();
			route = vec;
			oroute = vec;
			updateLength();
		}
};

rebalancingRoute::rebalancingRoute(vector<string>& _r) {
	route = _r;
	oroute = _r;
	length = _r.size();
}

void rebalancingRoute::intraRI(int neighbor) {    // reinsert a subsequence to another position of the route

	vector<string> vec;
	vector<string>::iterator iter = route.begin();
	int begin, end, loc = 0;
	int flag = 0;
	int counter = 0;

	do {
		vec.clear();
		counter++;
		begin = rand() % (length - neighbor);
		end = begin + neighbor + 1;

		do {
			loc = rand() % length;
			//cout << begin << '\t' << end << '\t'<< loc << '\n';
		} while (loc >= begin && loc<=end) ;
		
		if (loc < begin) {
			//cout << '\n';
			//for (auto iter = vec.begin(); iter != vec.end(); iter++) {
			//	cout << *(iter) << '\t';
			//}
			//cout << '\n';
			vec.insert(vec.begin(), route.begin(), iter + loc);
			vec.insert(vec.end(), iter + begin, iter + end);
			vec.insert(vec.end(), iter + loc, iter + begin);
			vec.insert(vec.end(), iter + end, route.end());
		}
		else {
			//cout << '\n';
			//for (auto iter = vec.begin(); iter != vec.end(); iter++) {
			//	cout << *(iter) << '\t';
			//}
			//cout << '\n';
			vec.insert(vec.begin(), route.begin(), iter + begin);
			vec.insert(vec.end(), iter + end, iter + loc);
			vec.insert(vec.end(), iter + begin, iter + end);
			vec.insert(vec.end(), iter + loc, route.end());

			/*cout << '\n';
			for (auto iter = vec.begin(); iter != vec.end(); iter++) {
				cout << *(iter) << '\t';
			}
			cout << '\n';*/
		}
		flag = checkSpaceFeasibility(vec);

		if (flag == 1) {
			//cout << "\nIntraRI succeeded!\n";
			*this = vec;
			//route.clear();
			//route.insert(route.begin(), vec.begin(), vec.end());
			//cout << '\n';
			//for (auto iter = route.begin(); iter != route.end(); iter++) {
			//	cout << *(iter) << '\t';
			//}
			//cout << '\n';
		}

	} while (flag == 0 && counter < MAXITER);
}

void rebalancingRoute::intraIC(int neighbor) { //interchange two subsequences

	vector<string> vec;
	vector<string>::iterator iter = route.begin();
	
	int begin1, end1, begin2, end2;
	int flag = 0;
	int counter = 0;

	do{
		counter++;
		vec.clear();
		//shall make sure two subsequences do not overlap
		do {
			begin1 = rand() % (length - neighbor);
			end1 = begin1 + neighbor + 1;
			begin2 = rand() % (length - neighbor);
			end2 = begin2 + neighbor + 1;
		} while ((begin2 == begin1) || (end1>begin2 && end1<end2) || (end2 > begin1 && end2 < end1));

		vec = route;

		/*cout << '\n';
		for (auto iter = vec.begin(); iter != vec.end(); iter++) {
			cout << *(iter) << '\t';
		}
		cout << '\n';
		cout << begin1 << '\t' << end1 << '\t' << begin2 << '\t' << end2 << '\n';
		cout << *(iter+begin1) << '\t' << *(iter + end1-1) << '\t' << *(iter + begin2) << '\t' << *(iter + end2-1) << '\n';
		*/
		
		vector<string>::iterator it = vec.begin();
		std::swap_ranges(it+begin1, it+end1, it+begin2);

		flag = checkSpaceFeasibility(vec);
		if (flag == 1) {
			//cout << "\nIntraIC succeeded!\n";
			*this = vec;
			//route.clear();
			//route.insert(route.begin(), vec.begin(), vec.end());
		}
		
	} while  (flag == 0 && counter < MAXITER);
}


void rebalancingRoute::intraNS(int neighbor) {

	int loc1, loc2, latest1, latest2;
	int counter = 0;
	int flagsum = 0;
	vector<string> vec;

	do {
		counter++;
		vec.clear();
		latest1 = latest2 = 999;
		for (int i = 0; i < neighbor; i++) {
			int flag, c;
			flag = 0;
			c = 0;
			do {
				c++;
				int num = 0;
				do {
					num ++ ;
					loc1 = rand() % length;
					loc2 = rand() % length;
					if (num == MAXITER) {
						break;
					}
				} while (loc2 == loc1 || (latest1 == loc2 && latest2 == loc1));
				vec = route;
				vector<string>::iterator it = vec.begin();
				std::iter_swap(it + loc1, it + loc2);
				flag = checkSpaceFeasibility(vec);
				if (flag == 1) {
					*this = vec;
					flagsum += 1;
				}
			} while (flag == 0 && c<MAXITER/4);
			latest1 = loc1;
			latest2 = loc2;
		}
		if (flagsum == neighbor) {
			//cout << "\nIntraNS succeeded!\n";
			break;
		}
	} while (flagsum != neighbor && counter < MAXITER);
}

void rebalancingRoute::interCR(int neighbor, rebalancingRoute& route2) { //inter crossover

	int counter = 0;
	int flagsum = 0;
	int begin1, begin2, end;
	int flag1, flag2;
	vector<string>::iterator iter1, iter2;
	vector<string> vec1, vec2;

	do {
		counter++;
		vec1.clear();
		vec2.clear();
		flag1 = flag2 = 0;
		begin1 = rand() % (length - neighbor);
		begin2 = rand() % (route2.length - neighbor);
		end = begin1 + neighbor + 1;

		vec1 = route;
		vec2 = route2.route;
		/*cout << '\n';
		for (auto iter = vec1.begin(); iter != vec1.end(); iter++) {
			cout << *(iter) << '\t';
		}
		cout << '\n';
		for (auto iter = vec2.begin(); iter != vec2.end(); iter++) {
			cout << *(iter) << '\t';
		}
		cout << '\n';*/
		iter1 = vec1.begin();
		iter2 = vec2.begin();
		swap_ranges(iter1 + begin1, iter1 + end, iter2 + begin2);
		flag1 = checkSpaceFeasibility(vec1);
		flag2 = checkSpaceFeasibility(vec2);

		if (flag1 == 1 && flag2 == 1) {
			/*cout << "\nnew\n";
			for (auto iter = vec1.begin(); iter != vec1.end(); iter++) {
				cout << *(iter) << '\t';
			}
			cout << '\n';
			for (auto iter = vec2.begin(); iter != vec2.end(); iter++) {
				cout << *(iter) << '\t';
			}
			cout << '\n';*/
			//cout << "\nInterCR succeeded!\n";
			*this = vec1;
			route2 = vec2;
			/*cout << "\nnew\n";
			for (auto iter = route.begin(); iter != route.end(); iter++) {
				cout << *(iter) << '\t';
			}
			cout << '\n';
			for (auto iter = route2.route.begin(); iter != route2.route.end(); iter++) {
				cout << *(iter) << '\t';
			}
			cout << '\n';*/
		}

	} while ((flag1 == 0 || flag2 == 0) && counter < MAXITER);
}


void rebalancingRoute::interNS(int neighbor, rebalancingRoute& route2) {  //node swapping
	
	int loc1, loc2;
	int counter = 0;
	vector<string>::iterator iter1, iter2;
	vector<string> vec1, vec2;
	vector<int> locvec1, locvec2;
	vector<int>::iterator it;
	int flag1, flag2, flagsum;
	flagsum = 0;

	iter1 = route.begin();
	iter2 = route2.route.begin();
	do {
		counter++;
		vec1.clear();
		vec2.clear();
		flag1 = flag2 = 0;
		for (int i = 0; i < neighbor; i++) {
			do {
				loc1 = rand() % length;
				it = find(locvec1.begin(), locvec1.end(), loc1);
			} while (it != locvec1.end());

			do {
				loc2 = rand() % route2.length;
				it = find(locvec2.begin(), locvec2.end(), loc2);
			} while (it != locvec2.end());

			vec1 = route;
			vec2 = route2.route;
			iter1 = vec1.begin();
			iter2 = vec2.begin();

			std::iter_swap(iter1 + loc1, iter2 + loc2);
			flag1 = checkSpaceFeasibility(vec1);
			flag2 = checkSpaceFeasibility(vec2);
			if (flag1 == 1 && flag2 == 1) {
				flagsum++;
				locvec1.push_back(loc1);
				locvec2.push_back(loc2);
				*this = vec1;
				route2 = vec2;
			}

		}
		if (flagsum == neighbor) {
			//cout << "\nInterNS succeeded!\n";
			break;
		}
	} while (flagsum != neighbor && counter < MAXITER);
/*cout << "after route\n";
for (auto iter = route.begin(); iter != route.end(); iter++) {
	cout << (*iter) << '\t';
}
cout << "\n";
for (auto iter = route2.route.begin(); iter != route2.route.end(); iter++) {
	cout << (*iter) << '\t';
}
cout << "\n";*/
}

void rebalancingRoute::interSH(int neighbor, rebalancingRoute& route2) {  // shift a subsequence from longer routes to shorter routes
	int flag1, flag2;
	int break1, end, loc;
	int counter = 0;
	vector<string> vec1, vec2;
	vector<string>::iterator iter1, iter2;
	iter1 = route.begin();
	iter2 = route2.route.begin();

	do {
		counter++;
		vec1.clear();
		vec2.clear();
		break1 = rand() % (max(length, route2.length) - neighbor);
		end = break1 + neighbor + 1;
		loc = rand() % (min(length, route2.length));

		if (length <= route2.length) {
			vec1.insert(vec1.begin(), iter1, iter1 + loc);
			vec1.insert(vec1.end(), iter2 + break1, iter2 + end);
			vec1.insert(vec1.end(), iter1 + loc, route.end());
			vec2.insert(vec2.begin(), iter2, iter2+break1);
			vec2.insert(vec2.end(), iter2+end, route2.route.end());
		}
		else {
			vec1.insert(vec1.begin(), iter1, iter1 + break1);
			vec1.insert(vec1.end(), iter1 + end, route.end());
			vec2.insert(vec2.begin(), iter2, iter2 + loc);
			vec2.insert(vec2.end(), iter1 + break1, iter1 + end);
			vec2.insert(vec2.end(), iter2 + loc, route2.route.end());
		}
		flag1 = checkSpaceFeasibility(vec1);
		flag2 = checkSpaceFeasibility(vec2);
		if (flag1==1 && flag2 ==1){
			//cout << "\nInterSH succeeded!\n";
			*this = vec1;
			route2 = vec2;
		}
	} while ((flag1 == 0 || flag2==0) && counter < MAXITER);
	
	/*cout << "after route\n";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << "\n";
	for (auto iter = route2.route.begin(); iter != route2.route.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << "\n";*/
}


void rebalancingRoute::interCT(rebalancingRoute& route2) { // break routes and concatenate them to construct new routes

	int break1, break2, dif;
	int counter = 0;
	int flag1, flag2;
	vector<string> vec1, vec2;
	vector<string>::iterator iter1, iter2;
	iter1 = route.begin();
	iter2 = route2.route.begin();

	do {
		counter++;
		vec1.clear();
		vec2.clear();
		break1 = rand() % min(length, route2.length);
		dif = min(length - break1, route2.length - break1);
		break2 = rand() % (max(length, route2.length) - min(length, route2.length) + 1 ) + break1;
		
		/*cout << break1 << '\t' << dif << '\t' << break2<<'\n';
		for (auto iter = route.begin(); iter != route.end(); iter++) {
			cout << *(iter) << '\t';
		}
		cout << '\n';
		for (auto iter = route2.route.begin(); iter != route2.route.end(); iter++) {
			cout << *(iter) << '\t';
		}
		cout << '\n';*/

		if (length <= route2.length) {
			vec1.insert(vec1.begin(), iter1, iter1 + break1);
			vec1.insert(vec1.end(), iter2 + break2, route2.route.end()); // problem
			vec2.insert(vec2.begin(), iter2, iter2 + break2);
			vec2.insert(vec2.end(), iter1 + break1, route.end());
		}
		else {
			vec1.insert(vec1.begin(), iter1, iter1 + break2);
			vec1.insert(vec1.end(), iter2 + break1, route2.route.end());
			vec2.insert(vec2.begin(), iter2, iter2 + break1);
			vec2.insert(vec2.end(), iter1 + break2, route.end()); // problem
		}
		
		flag1 = checkSpaceFeasibility(vec1);
		flag2 = checkSpaceFeasibility(vec2);
		if (flag1 == 1 && flag2 == 1) {
			*this = vec1;
			route2 = vec2;
			//cout << "\nInterCT succeeded!\n";
			break;
		}
	} while ((flag1 == 0 || flag2 == 0) && counter < MAXITER);

	/*cout << "after route\n";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << "\n";
	for (auto iter = route2.route.begin(); iter != route2.route.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << "\n";*/
}



int rebalancingRoute::checkSpaceFeasibility(vector<string>& vec) { // vec used to store nodes that should be connected
	vector<string>::iterator iter = vec.begin();
	while (iter != vec.end()-1) {
		if (traveltime.find(*iter)->second.find(*(iter + 1))== traveltime.find(*iter)->second.end()) {
			//cout <<'\n'<< (*iter)<<'\t'<<*(iter + 1) <<"\n Not in map\n";
			return 0;
		}
		iter += 1;
	}
	return 1;
}

int rebalancingRoute::checkTimeFeasibility() {
	int time = 0;
	for (auto iter = route.begin(); iter != route.end()-1; iter++) {
		if (*(iter) == *(iter + 1)) {
			time += 1;
		}
		else {
			time += traveltime.find(*iter)->second.find(*(iter + 1))->second;
		}		
	}

	if (time <= 60) {   //extern int PlanningHorizon;
		return 1;
	}
	return 0;
}

void rebalancingRoute::updateLength() {
	length = route.size();
}
#endif /* LOCALSEARCH_H */
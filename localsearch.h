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


//void maxOperations(path& P){
//    
//    const int depotFlow = 3;
//    int sumInv=0;
//    int pathResDemand = 0;  
//    
//    //vehicle starting from the deport carries the maximum of {3, -negativepathResDemand}
//    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
//        pathResDemand = pathResDemand + (*iter).getDemand();
//    }
//    
//    if (pathResDemand<0){  // set flows for the links connecting depot and the first visited stations
//        P.links.front().setFlow(max(depotFlow, -pathResDemand, P.capacity));
//    }
//    else{
//        P.links.front().setFlow(depotFlow);
//    }
//    
//    int currentFlow=0;
//    for (int i = 0; i<P.nodes.size(); i++){
//        currentFlow = currentFlow + P.links[i].getFlow();
//        if (P.nodes[i].getDemand()<0){
//            max(-P.nodes[i].getDemand(), currentFlow);
//        }
//        else if (P.nodes[i].getDemand()>0){
//            min(-P.nodes[i].getDemand(), P.capacity - currentFlow);
//        }
//        
//    }
//    
//    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
//        sumInv = sumInv + (*iter).getIniInv();
//    }
//    
//    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
//        sumInv = sumInv + (*iter).getIniInv();
//    }
//    
//    for (auto iter = P.nodes.begin(); iter!=P.nodes.end(); iter ++){
//        if ((*iter).getDemand()>0){
//            
//        }
//    }
////    if ()
////    P.capacity - sumInv
//}

//void confrimChange(path& P){
//    int unmet = 0 ;
//    int distance = 0 ;
//    for (auto iter=P.nodes.begin(); iter!=P.nodes.end(); iter++){
//        unmet = unmet + demandMap[(*iter).getNodeId()] - (*iter).getStorage();  // demand is a map given nodes tell demand
//        distance = distance +  distanceMap[(*iter).getNodeId()]; //distance is a map given link return distance
//    }
//    
//    float cost_potential;
//    cost_potential = distanceCoef * distance + demandCoef * unmet;
//    if (cost_potential<P.getCost()){
//        
//    }
//    
//    
//}

class rebalancingRoute {
		
	public:
		vector<string> route;			//route is comprised of a sequence of non-time-indexed nodes
		int length;

		rebalancingRoute(vector<string> &);

		void intraRI(int);				// parameter is number of links in the subsequence
										// reinsert subsequence to a different location
		void intraIC(int);				// interchange two subsequence of links
		//void intraAD(int);				// adjust route, add new links into the routes
		void intraNS(int);				// node swapping

		void interCR(int, rebalancingRoute &);
		void interNS(int, rebalancingRoute &);
		void interSH(int, rebalancingRoute &);
		void interCT(rebalancingRoute &);  //concatenating two routes by cutting down them into two parts

		int checkTimeFeasibility();
		void updateLength();
};

rebalancingRoute::rebalancingRoute(vector<string>& _r) {
	route = _r;
	length = _r.size();
}

void rebalancingRoute::intraRI(int neighbor) {    // reinsertion a subsequence to another position of the route
	//srand(time(NULL));
	vector<string> subsequence;
	vector<string>::iterator iter = route.begin();
	int begin = rand() % ( length - neighbor - 1);
	int end = begin + neighbor + 1;
	int loc = 0;

	subsequence.insert(subsequence.begin(), iter + begin, iter + end);
	route.erase(iter + begin, iter + end);

	for (auto iter = subsequence.begin(); iter != subsequence.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << '\n';

	do {
		loc = rand() % route.size();
	} while (loc == begin);

	route.insert(iter + loc, subsequence.begin(), subsequence.end());

	//for (auto iter = route.begin(); iter != route.end(); iter++) {
	//	cout << (*iter) << '\t';
	//}

	//constructRoute(route);
}

void rebalancingRoute::intraIC(int neighbor) { //interchange two subsequences
	//vector<string> sub1;
	//vector<string> sub2;

	vector<string>::iterator iter = route.begin();
	int begin1 = rand() % (length - neighbor - 1);
	int end1 = begin1 + neighbor + 1;
	int begin2, end2;
	
	//shall make sure two subsequences do not overlap
	do {
		begin2 = rand() % (length - neighbor - 1);
		end2 = begin2 + neighbor + 1;
	} while ((begin2 == begin1) ||  (begin2<end1 && end2>end1) || (begin1<end2 && end2<end1));

	//sub1.insert(sub1.begin(), iter + begin1, iter + end1);
	//sub2.insert(sub2.begin(), iter + begin2, iter + end2);
	
	std::swap_ranges(iter+begin1, iter + end1, iter + begin2);
	
}


void rebalancingRoute::intraNS(int neighbor) {
	
	int loc1, loc2, latest1, latest2;
	int counter = 0;
	latest1 = latest2 = 999;

	vector<string>::iterator iter = route.begin();

	for (int i = 0; i < neighbor; i++) {
		do {
			loc1 = rand() % (length-1);
			loc2 = rand() % (length-1);
		} while (loc2 == loc1 || (latest1==loc2 && latest2==loc1));
		std::iter_swap(iter + loc1, iter + loc2);
	}

	/*cout << "after route\n";
	for (auto iter = route.begin(); iter != route.end(); iter++) {
		cout << (*iter) << '\t';
	}
	cout << "\n";*/
}

//void rebalancingRoute::intraAD (int neighbor, rebalancingRoute& route2) {
//
//}


void rebalancingRoute::interCR(int neighbor, rebalancingRoute& route2) { //inter crossover

	int begin1, begin2, end1;
	vector<string>::iterator iter1;
	vector<string>::iterator iter2;

	begin1 = rand() % (length - neighbor - 1);
	begin2 = rand() % (route2.length - neighbor - 1);
	end1 = begin1 + neighbor + 1;
	
	iter1 = route.begin();
	iter2 = route2.route.begin();
	swap_ranges(iter1 + begin1, iter1 + end1, iter2 + begin2);

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

void rebalancingRoute::interNS(int neighbor, rebalancingRoute& route2) {  //node swapping
	int begin1, begin2;
	vector<string>::iterator iter1;
	vector<string>::iterator iter2;
	vector<int> vec1;
	vector<int> vec2;
	vector<int>::iterator it;

	iter1 = route.begin();
	iter2 = route2.route.begin();

	for (int i = 0; i < neighbor; i++) {

		do {
			begin1 = rand() % (length - 1);
			it = find(vec1.begin(), vec1.end(), begin1);
		} while (it!=vec1.end());

		do {
			begin2 = rand() % (route2.length - 1);
			it = find(vec2.begin(), vec2.end(), begin2);
		} while (it!=vec2.end());

		vec1.push_back(begin1);
		vec2.push_back(begin2);
		std::iter_swap(iter1 + begin1, iter2 + begin2);
	}
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


void rebalancingRoute::interSH(int neighbor, rebalancingRoute& route2) {  // SHIFT, the route have subsequence shifted to should be orignially shorter
	int loc;
	vector<string>::iterator iter1, iter2;

	
	for (int i = 0; i < neighbor; i++) {
		iter1 = route.begin();
		iter2 = route2.route.begin();
		loc = rand() % min(length-1, route2.length-1);
		route.insert(iter1 + loc, *(iter2 + loc));
		route2.route.erase(iter2 + loc);
		this->updateLength();
		route2.updateLength();
	}


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

	int break1, break2;
	int quarter1 = int(length / 4);
	int quarter2 = int(route2.length / 4);
	vector<string>::iterator iter1, iter2;
	vector<string> sequence1, sequence2;
	iter1 = route.begin();
	iter2 = route2.route.begin();

	break1 = rand() % (quarter1 * 2) - quarter1 + int(length / 2);
	break2 = rand() % (quarter2 * 2) - quarter2 + int(route2.length / 2);

	cout << break1 << '\t' << break2 << '\n';
	sequence1.insert(sequence1.begin(), iter1 + break1, iter1 + length);
	sequence2.insert(sequence2.begin(), iter2 + break2, iter2 + route2.length);
	
	route.erase(iter1 + break1, iter1 + length);
	route2.route.erase(iter2 + break2, iter2 + route2.length);

	route.insert(route.end(), sequence2.begin(), sequence2.end());
	route2.route.insert(route2.route.end(), sequence1.begin(), sequence1.end());


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

int rebalancingRoute::checkTimeFeasibility() {
	int time = 0;
	for (auto iter = route.begin(); iter != route.end()-1; iter++) {
		if (*(iter) == *(iter + 1)) {
			time += 1;
		}
		else {
			time += traveltime[(*iter)][(*(iter + 1))];
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
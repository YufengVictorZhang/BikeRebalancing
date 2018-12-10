/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   readNetwork.h
 * Author: victor
 *
 * Created on September 30, 2018, 11:11 PM
 */

#ifndef READNETWORK_H
#define READNETWORK_H

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

map<string, node*> nodes;
map<string, link*> links;
map<string, map<string, int>> traveltime;
map<string, map<string, vector<string>>> accMap;

float demandCoef;
float distanceCoef;

void readParameters(float _distance, float _demand){
    distanceCoef = _distance;
    demandCoef = _demand;
}

void readNetwork(const string loc){
    string nInput, lInput, dInput, aInput;
    string line;
    node* tmpNodePntr;
    link* tmpLinkPntr;
    
    nInput = loc + "NiceRideNodes_May1820.txt";
    lInput = loc + "NiceRideLinks_May1820.txt";
	dInput = loc + "Distance.txt";
	aInput = loc + "accMap.txt"; // accessibility map

    ifstream nodefile (nInput);
    if (nodefile.is_open()){
        while( getline(nodefile, line)){
            
            stringstream ss(line);
            vector<string> tokens;
            string buf;
            while(ss>>buf){
                tokens.push_back(buf);
            }
            
            tmpNodePntr = NULL;
            tmpNodePntr = new node;
            nodes[tokens[0]]= tmpNodePntr;
            nodes[tokens[0]]->initializeNode(tokens[0], tokens[1]);
        }
    }
    nodefile.close();
    
    ifstream linkfile (lInput);
    if (linkfile.is_open()){
        while( getline(linkfile, line)){
      
            stringstream ss(line);
            vector<string> tokens;
            string buf;
            while(ss>>buf){
                tokens.push_back(buf);
            }
            
            tmpLinkPntr = NULL;
            tmpLinkPntr = new link;
            links[tokens[0]+","+tokens[1]]= tmpLinkPntr;
            links[tokens[0]+","+tokens[1]]->initializeLink(tokens[0], tokens[1], tokens[2]);
            
            nodes[tokens[0]]->setOutLinks(tokens[0]+","+tokens[1]);
            nodes[tokens[1]]->setInLinks(tokens[0]+","+tokens[1]);
        }
    }
    linkfile.close();
    
	ifstream distancefile (dInput);
	if (distancefile.is_open()) {
		getline(distancefile, line);
		string fromNode = "";
		map<string, int> dis;

		while (getline(distancefile, line)) {
			stringstream ss(line);
			vector<string> tokens;
			string buf;
			
			while (ss >> buf) {
				tokens.push_back(buf);
			}

			if (tokens[0] != fromNode) {
				if (dis.size() != 0) {
					traveltime[fromNode] = dis;
					dis.clear();
				}
			}
			
			if (stoi(tokens[4]) <= 3) {   // distance greater than 3 are not used as potential links
				dis[tokens[1]] = stoi(tokens[4]);
			}

			fromNode = tokens[0];
		}
		traveltime[fromNode] = dis;
	}
	distancefile.close();


	ifstream accfile(aInput);
	if (accfile.is_open()) {
		map <string, vector<string>> * p = new map<string, vector<string>>();
		string lastorigin = "";

		while (getline(accfile, line)) {
			stringstream ss(line);
			vector<string> tokens;
			string buf;
			while (ss >> buf) {
				tokens.push_back(buf);
			}

			(*p)[tokens[1]].insert((*p)[tokens[1]].begin(), tokens.begin() + 2, tokens.end());


			if (lastorigin != tokens[0] && (*p).size() != 1) {
				accMap[lastorigin] = *p;
				p = new map<string, vector<string>>();
			}

			lastorigin = tokens[0];
		}
		accMap[lastorigin] = *p;
		delete p;
	}
	accfile.close();
}


void setToOriginalNetwork(){
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
		nodes[(*iter).first]->setLabel(0);
		nodes[(*iter).first]->setPred("");
	}
	
	for (auto iter = links.begin(); iter != links.end(); ++iter) {
		links[(*iter).first]->setCost(0);
	}
}

void clearMCFPsol() {
	for (auto iter = links.begin(); iter != links.end(); ++iter) {
		links[(*iter).first]->setMCFPsol(0);
	}
}

#endif /* NEWFILE_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   link.h
 * Author: zhan4879
 *
 * Created on October 8, 2018, 10:37 AM
 */

#ifndef LINK_H
#define LINK_H

#include <vector>
#include <string>

using namespace std;

class link{
    string id;
    float distance = 0;
    float cost = 0;
	int MCFPsol = 0;
	float MCFPcost = 0;
    int cap = 0;
    int flow = 0;
	string type = "";
    
public:
    
    void initializeLink(string fromNode, string toNode, string _distance){   
        id = fromNode+","+toNode;
        distance = stof(_distance);
		string token1 = fromNode.substr(0, fromNode.find("_"));
		string token2 = toNode.substr(0, toNode.find("_"));

		if (token1 == token2) {
			type = "stationary";  //stationary links between same stations
		}
		else{
			if (fromNode == "D1" || toNode == "D2") {
				type = "depot";   // depot to/from links
			}
			else{
				type = "transship";
			}
		}

			
    }
    
    void setFlow(int _flow){
        flow=_flow;
    }

	void setType(string _type) {
		type = _type;
	}
    
	void setMCFPcost(float _c) {
		MCFPcost = _c;
	}

    string getLinkId(){
        return id;
    }
    
    int getFlow(){
        return flow;
    }
    
    float getDistance(){
        return distance;
    }
    
    void setCost(float c){
        cost = c;
    }

	void setMCFPsol(int m) {
		MCFPsol = m;
	}

	int getMCFPsol() {
		return MCFPsol;
	}

	int getMCFPcost() {
		return MCFPcost;
	}

	string getType() {
		return type;
	}
};




#endif /* LINK_H */



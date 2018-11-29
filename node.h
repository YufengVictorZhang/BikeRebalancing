/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.h
 * Author: zhan4879
 *
 * Created on October 8, 2018, 10:36 AM
 */


#ifndef NODE_H
#define NODE_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class node{
    string id;
    int demand = 0;
    int storage;
    int iniInv; 
    int capacity = 18;
	float label = 0.0;
	string pred = "";
    vector <string> outLinks;
    vector <string> inLinks;
    
public:
    void initializeNode(string _id, string _demand){   
        id = _id;
        demand = stoi(_demand);
        storage = 0;
    }

	void setLabel(float _label) {
		label = _label;
	}

	void setPred(string _pred) {
		pred = _pred;
	}

    void setStorage(int _storage){
        storage = _storage;
    }
    
    void setIniInv(int _ini){
        iniInv = _ini;
    }
    
    void setOutLinks(string l){
        outLinks.push_back(l);
    }
    
    void setInLinks(string l){
        inLinks.push_back(l);
    }
    
    const vector<string>& getOutLinks(){
        return outLinks;	
    }
    
    const vector<string>& getInLinks(){
        return inLinks;
    }
    
    string getNodeId(){
        return id;
    }
    
    int getStorage(){
        return storage;
    }
    
    int getIniInv(){
        return iniInv;
    }
    
    int getDemand(){
        return demand;
    }
    
    int getCapacity(){
        return capacity;
    }

	float getLabel() {
		return label;
	}

	string getPred() {
		return pred;
	}
 
};


#endif /* NODE_H */



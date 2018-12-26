#pragma once

#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <string>

using namespace std;

vector<string> positiveLinkSet;
const int alpha = 20;
const int CV = 10;

class GUROBIinstance{

public:
	GRBEnv *env = new GRBEnv();
	GRBModel model = GRBModel(*env);
	GRBVar *y = 0;
	GRBVar *x = 0;
	GRBVar *splus = 0;
	GRBVar *sminus = 0;

	int activeLink = 0;
	int nodesize = 0;
	map<string, int> splusIndexName;
	map<string, int> sminusIndexName;
	map<string, int> yIndexName;
	map<string, int> xIndexName;
	vector<GRBConstr> LinkConstr;
	vector<GRBConstr> NodeConstr;

	void createModel();
	void updateModel();
	void solveModel();
	double setPostiveLinkResults();
	void setSolution();
	int deleteModel();

};


void GUROBIinstance::createModel() {

	model.set(GRB_StringAttr_ModelName, "RPMCFP");
	model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
	model.set(GRB_DoubleParam_MIPGap, 0.03);
	model.set(GRB_IntParam_OutputFlag, 0);
	model.set(GRB_IntParam_Presolve, 2); // aggressive presolve

	nodesize = 0;
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		string node = (*iter).first;
		if (node != "D1" && node != "D2") {
			int time = stoi(node.substr(node.find("_") + 1, node.length()));
			if (time == 29 || time == 59) {
				nodesize++;
			}
		}
	}

	splus = model.addVars(nodesize, GRB_INTEGER);
	sminus = model.addVars(nodesize, GRB_INTEGER);

	int index = 0;
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
		string vname = (*iter).first;
		if (vname != "D2" && vname != "D1") {
			int time = stoi(vname.substr(vname.find("_") + 1, vname.length()));
			if (time == 29 || time == 59) {
				splus[index].set(GRB_DoubleAttr_Obj, alpha);
				splus[index].set(GRB_DoubleAttr_LB, 0.0);
				sminus[index].set(GRB_DoubleAttr_Obj, alpha);
				sminus[index].set(GRB_DoubleAttr_LB, 0.0);
				sminusIndexName[vname] = index;
				splusIndexName[vname] = index;
				++index;
			}
		}
	}

	activeLink = 0;
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1) {
			activeLink++;
		}
	}

	y = model.addVars(activeLink, GRB_INTEGER);
	x = model.addVars(activeLink, GRB_BINARY);

	index = 0;
	for (auto iter = links.begin(); iter != links.end(); ++iter) {
		if ((*iter).second->getResStatus() == 1) {
			string vname;
			vname = (*iter).first;
			x[index].set(GRB_DoubleAttr_Obj, links[(*iter).first]->getDistance());
			x[index].set(GRB_DoubleAttr_LB, 0.0);
			y[index].set(GRB_StringAttr_VarName, vname);
			y[index].set(GRB_DoubleAttr_LB, 0.0);
			xIndexName[vname] = index;
			yIndexName[vname] = index;
			++index;
		}
	}

	//set nodes constraints
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		string node = (*iter).first;
		GRBLinExpr nodeBalance = 0;
		for (int i = 0; i < nodes[node]->getOutLinks().size(); i++) {
			string l = nodes[node]->getOutLinks()[i];
			if (links[l]->getResStatus() == 1) {
				nodeBalance = nodeBalance - y[yIndexName[l]];
			}
		}
		for (int i = 0; i < nodes[node]->getInLinks().size(); i++) {
			string l = nodes[node]->getInLinks()[i];
			if (links[l]->getResStatus() == 1) {
				nodeBalance = nodeBalance + y[yIndexName[l]];
			}
		}

		//nodeBalance += splus[splusIndexName[node]] - sminus[sminusIndexName[node]];

		if (node != "D1" && node != "D2") {
			int time = stoi(node.substr(node.find("_") + 1, node.length()));
			if (time == 29 || time == 59) {
				if (nodes[node]->getDemand() <= 0) { // <
					nodeBalance += splus[splusIndexName[node]];
				}
				else {
					nodeBalance -= sminus[sminusIndexName[node]];
				}
				//nodeBalance += splus[splusIndexName[node]] - sminus[sminusIndexName[node]];
			}
		}
		NodeConstr.push_back(model.addConstr(nodeBalance, GRB_EQUAL, -nodes[node]->getDemand()));
	}


	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1) {
			string toNode;
			toNode = (*iter).first.substr((*iter).first.find(",") + 1, (*iter).first.length());
			LinkConstr.push_back(model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, nodes[toNode]->getCapacity()));
			LinkConstr.push_back(model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, CV * x[xIndexName[(*iter).first]]));
			//model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, nodes[tokens[1]]->getCapacity());
		}
	}

	/*for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1) {
			LinkConstr.push_back(model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, 20 * x[xIndexName[(*iter).first]]));
		}
	}*/
}

void GUROBIinstance::updateModel() {

	xIndexName.clear();
	yIndexName.clear();
	int numBiVars = model.get(GRB_IntAttr_NumBinVars);
	for (int i = 0; i < numBiVars; i++) {
		model.remove(x[i]);
		model.remove(y[i]);
	}
	
	for (auto iter = LinkConstr.begin(); iter != LinkConstr.end(); iter++) {
		model.remove((*iter));
	}
	LinkConstr.clear();
	activeLink = 0;

	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1) {
			activeLink++;
		}
	}

	y = model.addVars(activeLink, GRB_INTEGER);
	x = model.addVars(activeLink, GRB_BINARY);



	//create new yIndexName and xIndexName map
	int index = 0;
	for (auto iter = links.begin(); iter != links.end(); ++iter) {
		if ((*iter).second->getResStatus() == 1) {
			string vname;
			vname = (*iter).first;
			x[index].set(GRB_DoubleAttr_Obj, links[(*iter).first]->getDistance());
			x[index].set(GRB_DoubleAttr_LB, 0.0);
			y[index].set(GRB_StringAttr_VarName, vname);
			y[index].set(GRB_DoubleAttr_LB, 0.0);
			xIndexName[vname] = index;
			yIndexName[vname] = index;
			++index;
		}
	}

	// set new link constraints
	for (auto iter = links.begin(); iter != links.end(); iter++) {

		if ((*iter).second->getResStatus() == 1) {
			string toNode;
			toNode = (*iter).first.substr((*iter).first.find(",") + 1, (*iter).first.length());
			LinkConstr.push_back(model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, nodes[toNode]->getCapacity()));
			LinkConstr.push_back(model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, CV * x[xIndexName[(*iter).first]]));
		}
	}

	// set  new node constraints
	for (auto iter = NodeConstr.begin(); iter != NodeConstr.end(); iter++) {
		model.remove((*iter));
	}
	NodeConstr.clear();

	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		string node = (*iter).first;
		GRBLinExpr nodeBalance = 0;
		for (int i = 0; i < nodes[node]->getOutLinks().size(); i++) {
			string l = nodes[node]->getOutLinks()[i];
			if (links[l]->getResStatus() == 1) {
				nodeBalance = nodeBalance - y[yIndexName[l]];
			}
		}
		for (int i = 0; i < nodes[node]->getInLinks().size(); i++) {
			string l = nodes[node]->getInLinks()[i];
			if (links[l]->getResStatus() == 1) {
				nodeBalance = nodeBalance + y[yIndexName[l]];
			}
		}

		if (node != "D1" && node != "D2") {
			int time = stoi(node.substr(node.find("_") + 1, node.length()));
			if (time == 29 || time == 59) {
				if (nodes[node]->getDemand() <= 0) { // <
					nodeBalance += splus[splusIndexName[node]];
				}
				else {
					nodeBalance -= sminus[sminusIndexName[node]];
				}
			}
		}
		NodeConstr.push_back(model.addConstr(nodeBalance, GRB_EQUAL, -nodes[node]->getDemand()));
	}
	model.update();
}


void GUROBIinstance::solveModel() {

	model.optimize();

}

double GUROBIinstance::setPostiveLinkResults(){
	
	positiveLinkSet.clear();
	//cout << "\nTOTAL COSTS: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1 && (*iter).second->getType() == "transship") {
			if (y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0) {
				positiveLinkSet.push_back((*iter).first);
				//cout << (*iter).first << '\t' << y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) << '\n';
			}
		}
	}

	int c = 0;
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		if (splus[splusIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0) {
			//cout << (*iter).first << '\t' << nodes[(*iter).first]->getDemand() << '\t' <<
			c += splus[splusIndexName[(*iter).first]].get(GRB_DoubleAttr_X);
		}
	}

	return c * alpha;

}

void GUROBIinstance::setSolution(){
	clearMCFPsol();
	for (int i = 0; i < activeLink; i++) {
		if (y[i].get(GRB_DoubleAttr_X) != 0) {
			links[y[i].get(GRB_StringAttr_VarName)]->setMCFPsol(y[i].get(GRB_DoubleAttr_X));
		}
	}
}

int GUROBIinstance::deleteModel() {
	delete[] y;
	delete[] x;
	delete[] splus;
	delete[] sminus;
	delete env;
	return 0;
}



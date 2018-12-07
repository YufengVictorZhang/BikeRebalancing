#pragma once


#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <string>


using namespace std;

const int alpha = 10;

vector<string> positiveLinkSet;

int solveResParMCFP() {
	ofstream logfile("restrictedpartialmcfp.log");

	if (!logfile.is_open()) {
		cout << "Cannot open the logfile for callback message" << endl;
		return 1;
	}

	GRBEnv *env = 0;
	GRBVar *y = 0;
	GRBVar *x = 0;
	GRBVar *splus = 0;
	GRBVar *sminus = 0;

	int activeLink = 0;
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getResStatus() == 1) {
			activeLink++;
		}
	}

	int nodesize = 0;
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		string node = (*iter).first;
		if (node != "D1" && node != "D2") {
			int time = stoi(node.substr(node.find("_") + 1, node.length()));
			if (time == 29 || time == 59) {
				nodesize++;
			}
		}
	}

	try {

		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.set(GRB_StringAttr_ModelName, "RPMCFP");

	
		GRBVar* y = model.addVars(activeLink, GRB_INTEGER);
		GRBVar* x = model.addVars(activeLink, GRB_BINARY);
		//GRBVar* splus = model.addVars(nodes.size(), GRB_INTEGER);
		//GRBVar* sminus = model.addVars(nodes.size(), GRB_INTEGER);

		
		GRBVar* splus = model.addVars(nodesize, GRB_INTEGER);
		GRBVar* sminus = model.addVars(nodesize, GRB_INTEGER);

		map<string, int> yIndexName;
		map<string, int> xIndexName;
		map<string, int> splusIndexName;
		map<string, int> sminusIndexName;

		int index = 0;
		for (auto iter = links.begin(); iter != links.end(); ++iter) {
			if ((*iter).second->getResStatus() == 1) {
				string vname;
				vname = (*iter).first;
				x[index].set(GRB_DoubleAttr_Obj, links[(*iter).first]->getDistance());
				//x[index].set(GRB_StringAttr_VarName, vname);
				x[index].set(GRB_DoubleAttr_LB, 0.0);
				y[index].set(GRB_StringAttr_VarName, vname);
				y[index].set(GRB_DoubleAttr_LB, 0.0);
				xIndexName[vname] = index;
				yIndexName[vname] = index;
				++index;
			}
		}

		//index = 0;
		//for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
		//	string vname = (*iter).first;
		//	splus[index].set(GRB_DoubleAttr_Obj, alpha);
		//	//splus[index].set(GRB_StringaAttr_VarName, vname);
		//	splus[index].set(GRB_DoubleAttr_LB, 0.0);
		//	sminus[index].set(GRB_DoubleAttr_Obj, alpha);
		//	//sminus[index].set(GRB_Stringattr_VarName, vname);
		//	sminus[index].set(GRB_DoubleAttr_LB, 0.0);
		//	sminusIndexName[vname] = index;
		//	splusIndexName[vname] = index;
		//	++index;
		//}

		index = 0;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			string vname = (*iter).first;
			if (vname != "D2" && vname != "D1") {
				int time = stoi(vname.substr(vname.find("_") + 1, vname.length()));
				if (time == 29 || time == 59) {
					//splus[index].set(GRB_DoubleAttr_Obj, alpha);
					splus[index].set(GRB_DoubleAttr_Obj, 0.0);
					splus[index].set(GRB_DoubleAttr_LB, 0.0);
					sminus[index].set(GRB_DoubleAttr_Obj, alpha);
					sminus[index].set(GRB_DoubleAttr_LB, 0.0);
					sminusIndexName[vname] = index;
					splusIndexName[vname] = index;
					++index;
				}
			}
		}

		model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

		//set constraints
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
			
			if (node!="D1" && node!="D2") {
				int time = stoi(node.substr(node.find("_") + 1, node.length()));
				if (time == 29 || time == 59) {
					if (nodes[node]->getDemand() <= 0) {
						nodeBalance += splus[splusIndexName[node]];
					}
					else {
						nodeBalance -= sminus[sminusIndexName[node]];
					}
					//nodeBalance += splus[splusIndexName[node]] - sminus[sminusIndexName[node]];
				}
			}
		
			model.addConstr(nodeBalance, GRB_EQUAL, -nodes[node]->getDemand());
		}

		for (auto iter = links.begin(); iter != links.end(); iter++) {

			if ((*iter).second->getResStatus() == 1) {
				vector<string> tokens;
				stringstream ss((*iter).first);
				while (ss.good()) {
					string substr;
					getline(ss, substr, ',');
					tokens.push_back(substr);
				}
				model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, nodes[tokens[1]]->getCapacity());

			}
		}

		for (auto iter = links.begin(); iter != links.end(); iter++) {
			if ((*iter).second->getResStatus() == 1) {
				model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, 20 * x[xIndexName[(*iter).first]]);
			}
		}

		model.set(GRB_DoubleParam_MIPGap, 0.03);
		model.optimize();

		cout << "\nTOTAL COSTS: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

		positiveLinkSet.clear();
		for (auto iter = links.begin(); iter != links.end(); iter++) {
			if ((*iter).second->getResStatus() == 1 && (*iter).second->getType() == "transship") {
				if (y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0) {
					//cout << (*iter).first<<'\t'<< y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) << '\n';
					positiveLinkSet.push_back((*iter).first);
				}
			}
		}

		for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
			if (sminus[sminusIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0 ||
				splus[splusIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0) {
				cout << (*iter).first << '\t' << nodes[(*iter).first]->getDemand() << '\t'<<
					splus[splusIndexName[(*iter).first]].get(GRB_DoubleAttr_X) << '\t' <<
					sminus[sminusIndexName[(*iter).first]].get(GRB_DoubleAttr_X) << '\n';
			}
		}


		clearMCFPsol();
		for (int i = 0; i < activeLink; i++) {
			if (y[i].get(GRB_DoubleAttr_X) != 0) {
				links[y[i].get(GRB_StringAttr_VarName)]->setMCFPsol(y[i].get(GRB_DoubleAttr_X));
				//cout << y[i].get(GRB_StringAttr_VarName)<<'\t' << y[i].get(GRB_DoubleAttr_X) << '\n';
			}
		}
	}
	
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}

	catch (...) {
		cout << "exception during optimization" << endl;
	}


	delete[] y;
	delete env;
	return 0;
}



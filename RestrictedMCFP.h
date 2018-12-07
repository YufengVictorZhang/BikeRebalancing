#pragma once

#ifndef RestrictedMCFP_H
#define RestrictedMCFP_H

#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <string>


using namespace std;


vector<string> positiveLinkSet;

int solveResMCFP() {
	ofstream logfile("restrictedmcfp.log");

	if (!logfile.is_open()) {
		cout << "Cannot open the logfile for callback message" << endl;
		return 1;
	}

	GRBEnv *env = 0;
	GRBVar *y = 0;
	GRBVar *x = 0;

	try {

		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.set(GRB_StringAttr_ModelName, "RMCFP");

		//create variables
		int activeLink = 0;
		for (auto iter = links.begin(); iter != links.end(); iter++) {
			if ((*iter).second->getResStatus() == 1) {
				activeLink++;
			}
		}

		GRBVar* y = model.addVars(activeLink, GRB_INTEGER);
		GRBVar* x = model.addVars(activeLink, GRB_BINARY);
		GRBVar* Splus = model.addVars(nodes.size(), GRB_INTEGER);
		GRBVar* Sminus = model.addVars(nodes.size(), GRB_INTEGER);


		map<string, int> yIndexName;
		map<string, int> xIndexName;

		int index = 0;
		for (auto iter = links.begin(); iter != links.end(); ++iter) {
			if ((*iter).second->getResStatus() == 1) {
				string vname;
				vname = (*iter).first;
				x[index].set(GRB_DoubleAttr_Obj, links[(*iter).first]->getDistance());
				x[index].set(GRB_StringAttr_VarName, vname);
				x[index].set(GRB_DoubleAttr_LB, 0.0);
				y[index].set(GRB_StringAttr_VarName, vname);
				y[index].set(GRB_DoubleAttr_LB, 0.0);
				xIndexName[vname] = index;
				yIndexName[vname] = index;
				++index;
			}

		}

		model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

		//set constraints
		for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
			GRBLinExpr nodeBalance = 0;
			for (int i = 0; i < nodes[(*iter).first]->getOutLinks().size(); i++) {
				string l = nodes[(*iter).first]->getOutLinks()[i];
				if (links[l]->getResStatus() == 1) {
					nodeBalance = nodeBalance - y[yIndexName[l]];
				}
			}
			for (int i = 0; i < nodes[(*iter).first]->getInLinks().size(); i++) {
				string l = nodes[(*iter).first]->getInLinks()[i];
				if (links[l]->getResStatus() == 1) {
					nodeBalance = nodeBalance + y[yIndexName[l]];
				}
			}
			model.addConstr(nodeBalance, GRB_EQUAL, -nodes[(*iter).first]->getDemand());
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
				model.addConstr(y[yIndexName[(*iter).first]], GRB_LESS_EQUAL, 30 * x[xIndexName[(*iter).first]]);
			}
		}

		//model.set(GRB_DoubleParam_MIPGap, 0.05);
		model.optimize();

		cout << "\nTOTAL COSTS: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

		//set MCFP solutions to the links in the network (only contains transshipment flows)
		//clearMCFPsol();
		//for (int i = 0; i < activeLink; i++) {
		//	if (y[i].get(GRB_DoubleAttr_X) != 0) {
		//		vector<string> tokens;
		//		stringstream ss(y[i].get(GRB_StringAttr_VarName));
		//		while (ss.good()) {
		//			string substr;
		//			getline(ss, substr, ',');
		//			stringstream sub(substr);
		//			getline(sub, substr, '_');
		//			tokens.push_back(substr);
		//		}
		//		if (tokens[0] != tokens[1]) {
		//			links[y[i].get(GRB_StringAttr_VarName)]->setMCFPsol(y[i].get(GRB_DoubleAttr_X));
		//			//cout << y[i].get(GRB_StringAttr_VarName) << '\n'<< y[i].get(GRB_DoubleAttr_X)<<endl;
		//		}
		//	}
		//}

		for (auto iter = links.begin(); iter != links.end(); iter++) {
			if ((*iter).second->getResStatus() == 1 && (*iter).second->getType() == "transship") {
				if (y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) != 0) {
					//cout << (*iter).first<<'\t'<< y[xIndexName[(*iter).first]].get(GRB_DoubleAttr_X) << '\n';
					positiveLinkSet.push_back((*iter).first);
				}
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

#endif /* RestrictedMCFP_H */


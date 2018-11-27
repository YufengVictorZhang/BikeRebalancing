/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MCFP.h
 * Author: zhan4879
 *
 * Created on October 8, 2018, 12:03 PM
 */

#ifndef MCFP_H
#define MCFP_H

#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;


int solveMCFP(){
    ofstream logfile ("mcfp.log");
    
    if(!logfile.is_open()){
        cout<<"Cannot open the logfile for callback message"<<endl;
        return 1;
    }
 
    GRBEnv *env = 0;
    GRBVar *y = 0;
    
    try{

        env = new GRBEnv();
        GRBModel model = GRBModel(*env);
        model.set(GRB_StringAttr_ModelName, "MCFP");
        
        //create variables
        GRBVar* y = model.addVars(links.size(), GRB_INTEGER);
        
        map<string, int> varIndexName;
        
        int index = 0;
        for (auto iter= links.begin(); iter!=links.end(); ++iter){
            string vname;
            vname = (*iter).first;
            y[index].set(GRB_DoubleAttr_Obj, links[(*iter).first]->getDistance());
            y[index].set(GRB_StringAttr_VarName, vname);
            y[index].set(GRB_DoubleAttr_LB, 0.0);
            varIndexName[(*iter).first]=index;
            ++index;
        }
        
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
        
        //set constraints
        for (auto iter=nodes.begin(); iter!=nodes.end(); iter++){
            GRBLinExpr nodeBalance = 0;
            for(int i=0; i<nodes[(*iter).first]->getOutLinks().size(); i++){
                string l = nodes[(*iter).first]->getOutLinks()[i];
                nodeBalance = nodeBalance - y[varIndexName[l]];
            }
            for(int i=0; i<nodes[(*iter).first]->getInLinks().size(); i++){
                string l = nodes[(*iter).first]->getInLinks()[i];
                nodeBalance = nodeBalance + y[varIndexName[l]];
            }
            model.addConstr(nodeBalance, GRB_EQUAL, -nodes[(*iter).first]->getDemand());
        }
        
        for (auto iter=links.begin(); iter!=links.end(); iter++){
            vector<string> tokens;
            stringstream ss((*iter).first);
            while (ss.good()){
                string substr;
                getline (ss, substr, ',');
                tokens.push_back(substr);
            }
            model.addConstr(y[varIndexName[(*iter).first]], GRB_LESS_EQUAL, (nodes[tokens[1]]->getCapacity()));
        }
        
        
        model.optimize();

		cout << "\nTOTAL COSTS: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		

		//set MCFP solutions to the links in the network (only contains transshipment flows)
		clearMCFPsol();
		for (int i = 0; i < links.size(); i++) {
			if (y[i].get(GRB_DoubleAttr_X) != 0) {
				vector<string> tokens;
				stringstream ss(y[i].get(GRB_StringAttr_VarName));
				while (ss.good()) {
					string substr;
					getline(ss, substr, ',');
					stringstream sub(substr);
					getline(sub, substr, '_');
					tokens.push_back(substr);
				}
				if (tokens[0] != tokens[1]) {
					links[y[i].get(GRB_StringAttr_VarName)]->setMCFPsol(y[i].get(GRB_DoubleAttr_X));
					//cout << y[i].get(GRB_StringAttr_VarName) << '\n'<< y[i].get(GRB_DoubleAttr_X)<<endl;
				}	
			}
		}
    }
    catch (GRBException e)
    {
        cout << "Error code = " << e.getErrorCode() <<endl;
        cout << e.getMessage() << endl;
    }
   
    catch (...){
        cout<<"exception during optimization"<<endl;
    }


    delete [] y;
    delete env;
    return 0;
}


//int solveMCFP(){
//    cout<<"Yes!"<<endl;
//}
#endif /* MCFP_H */


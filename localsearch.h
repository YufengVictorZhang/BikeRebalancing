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
#include "path.h"

void maxOperations(path& P){
    
    const int depotFlow = 3;
    int sumInv=0;
    int pathResDemand = 0;  
    
    //vehicle starting from the deport carries the maximum of {3, -negativepathResDemand}
    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
        pathResDemand = pathResDemand + (*iter).getDemand();
    }
    
    if (pathResDemand<0){  // set flows for the links connecting depot and the first visited stations
        P.links.front().setFlow(max(depotFlow, -pathResDemand, P.capacity));
    }
    else{
        P.links.front().setFlow(depotFlow);
    }
    
    int currentFlow=0;
    for (int i = 0; i<P.nodes.size(); i++){
        currentFlow = currentFlow + P.links[i].getFlow();
        if (P.nodes[i].getDemand()<0){
            max(-P.nodes[i].getDemand(), currentFlow);
        }
        else if (P.nodes[i].getDemand()>0){
            min(-P.nodes[i].getDemand(), P.capacity - currentFlow);
        }
        
    }
    
    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
        sumInv = sumInv + (*iter).getIniInv();
    }
    
    for (auto iter = P.nodes.begin(); iter !=P.nodes.end(); iter ++){
        sumInv = sumInv + (*iter).getIniInv();
    }
    
    for (auto iter = P.nodes.begin(); iter!=P.nodes.end(); iter ++){
        if ((*iter).getDemand()>0){
            
        }
    }
//    if ()
//    P.capacity - sumInv
}


confrimChange(path& P){
    int unmet = 0 ;
    int distance = 0 ;
    for (auto iter=P.nodes.begin(); iter!=P.nodes.end(); iter++){
        unmet = unmet + demandMap[(*iter).getNodeId()] - (*iter).getStorage();  // demand is a map given nodes tell demand
        distance = distance +  distanceMap[(*iter).getNodeId()]; //distance is a map given link return distance
    }
    
    float cost_potential;
    cost_potential = distanceCoef * distance + demandCoef * unmet;
    if (cost_potential<P.getCost()){
        
    }
    
    
}


intraDR( path & P){ // operating on path P
    for (auto iter=P.nodes.begin(); iter!=P.nodes.end(); iter++){
      continue;   
    }
        
 
}; 
//    friend intraNW( path &); //intra node swapping
//    friend intraEE( path &); //intra edge exchange
//    friend intraAI( path &); //intra adjust instructions
//    friend interCR(path &, path &); //inter crossover
//    friend interNW(path &, path &); //inter node swap
//    friend interEE(path &, path &); //inter edge exchange
//

#endif /* LOCALSEARCH_H */
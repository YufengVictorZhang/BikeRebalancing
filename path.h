/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   path.h
 * Author: victor
 *
 * Created on September 30, 2018, 8:20 PM
 */


#include <iostream>
#include <string>
#include <vector>

using namespace std;

class path{
    int capacity;
    vector<node> nodes;
    vector<link> links;
    int cost;
    

public:
    void setCapacity(int cap){
        capacity = cap;
    }
    
    void printCapacity(){
        cout<<capacity<<'\n';
    }
    template <typename T> string type_name();
    
    void buildPath(node* &n, link* &l, int nlength, int llength){ //when pass an array to function array is decayed to a pointer so that sizeof operator doesn't work, always carry length of array around
        //cout<< typeid(l).name()<<'\n';
      nodes.assign( &n[0], &n[nlength]);
      links.assign( &l[0], &l[llength]);

    }
    

    void printPath(){
        for (auto it=nodes.begin(); it != nodes.end(); ++it){
            cout << ' ' << (*it).getNodeId();
        }
     
        for (auto it=links.begin(); it != links.end(); ++it){
            cout << ' ' << (*it).getLinkId();
        }
           
    }
    
    void setCost(){
        int unmet = 0;
        float dis = 0;
//        for (auto iter = nodes.begin(); iter!=nodes.end(); iter++){
//            unmet = unmet + demandMap[*iter] - (*iter).getStorage();
//            dis = dis + distanceMap[*iter];
//        }
//        cost = unmet * demandCoef + dis * distanceCoef;
    }
    
    
    float getCost(){
        return cost;
    }
    
    friend maxOperations (path &); //calculate the maximum operations at a station and decide the flows
    friend confrimChange( path &); //confirm a change of path
    friend intraDR( path &); //intra delete-reinsert
    friend intraNW( path &); //intra node swapping
    friend intraEE( path &); //intra edge exchange
    friend intraAI( path &); //intra adjust instructions
    friend interCR(path &, path &); //inter crossover
    friend interNW(path &, path &); //inter node swap
    friend interEE(path &, path &); //inter edge exchange
  
};
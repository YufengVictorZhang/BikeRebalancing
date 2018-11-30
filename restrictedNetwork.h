#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
using namespace std;


//map<string, node*> rNode;
//map<string, link*> rLink;
//map<string, node*> rNodeIst;
//map<string, link*> rLinkIst;

//void copyNetwork() {
//	
//	rNode.insert(nodes.begin(), nodes.end());
//	rLink.insert(links.begin(), links.end());
//	
//	for (auto iter = rNode.begin(); iter!= rNode.end(); iter++) {
//		vector<string> outlink = (*iter).second->getOutLinks();
//		vector<string>::iterator Iter = outlink.begin();
//		while (Iter!=outlink.end()) {
//			if (rLink[*Iter]->getType() == "transship") {
//				Iter = outlink.erase(Iter);
//			}
//			else {
//				Iter++;
//			}
//		}
//	}
//
//	map<string,link*>::iterator  iter= rLink.begin();
//	while (iter!= rLink.end()) {
//		if ((*iter).second->getType() == "transship") {
//			iter = rLink.erase(iter);
//		}
//		else {
//			iter++;
//		}
//	}
//}


void createResNetwork() {
	
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getType() == "transship") {    // for stationary links and depot links, 
														   // they are always in the restriced network
			(*iter).second->resetResNetwork();
		}
	}

	for (auto iter = routeSet.begin(); iter != routeSet.end(); iter++) {
		if (links[(*iter)]->getType() == "transship") {    
			links[(*iter)]->setResNetwork();
		}
	}

}
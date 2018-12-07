#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
using namespace std;

void createResNetwork() {
	
	for (auto iter = links.begin(); iter != links.end(); iter++) {
		if ((*iter).second->getType() == "transship") {    // for stationary links and depot links, 
														   // they are always in the restriced network
			(*iter).second->resetResNetwork();
		}
	}

	for (auto iter = routeLinkSet.begin(); iter != routeLinkSet.end(); iter++) {
		if (links[(*iter)]->getType() == "transship") {    
			links[(*iter)]->setResNetwork();
		}
	}

}
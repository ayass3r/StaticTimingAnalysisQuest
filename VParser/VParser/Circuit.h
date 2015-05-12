//
//  Circuit.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Circuit__
#define __VParser__Circuit__


//#include "Edge.h"
#include "wire.h"
#include "gate.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <fstream>

class Circuit
{  
private:
    wire* w;
    gate* g;
    //Edge* e;
    std::map<std::string, wire*> wireMap;
    std::map<std::string, gate*> gateMap;
    //std::vector<gate*> gates;
    std::string pGate;
public:
    Circuit();
    void parseLine(std::string);
    void openFile(std::string);
    void printGates();
    void printWires();
};

#endif /* defined(__VParser__Circuit__) */

//
//  Circuit.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Circuit__
#define __VParser__Circuit__

#include "Edge.h"
#include "wire.h"
#include "gate.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <fstream>
#include <queue>

class Circuit
{  
private:
    wire* w;
    gate* g;
    std::string pGate, pGate2;
    std::map<std::string, wire*> wireMap;
    std::map<std::string, gate*> gateMap;
    std::vector<Edge*> vEdges;
    
public:
    Circuit();
    void createRoot();
    //void createEnd();
    //void connect(bool);
    void generateEges();
    bool parseLine(std::string);
    void openFile(std::string);
    bool parseSkewLine(std::string);
    void openSkewFile(std::string);
    bool parseCapLine(std::string);
    void openCapFile(std::string);
    void printGates();
    void printWires();
    void calcGatesOutputLoad();
    std::vector<gate*> topSort();
};

#endif /* defined(__VParser__Circuit__) */

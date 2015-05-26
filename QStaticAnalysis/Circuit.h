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
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <queue>
#include <fstream>

class Circuit
{  
private:
    wire* w;
    gate* g;
    std::string pGate, pGate2;
    std::map<std::string, wire*> wireMap;
    std::map<std::string, gate*> gateMap;
    std::vector<Edge*> vEdges;
    std::vector<gate*> Paths;
    float PathDelay;
    void printPaths(float);
    float clkPeriod;
public:
    Circuit();
    float getClk();
    void createRoot();
    void openConFile(std::string);
    bool parseConFile(std::string);
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
    void trvGate(std::vector<gate*>);
    void getPaths(gate*);
    void WrapPrintPaths();
    float calcReqTime(gate*);
};

#endif /* defined(__VParser__Circuit__) */

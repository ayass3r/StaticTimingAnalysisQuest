//
//  Circuit.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Circuit__
#define __VParser__Circuit__


#include "Node.h"
#include "Edge.h"
#include "wire.h"
#include "gate.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

class Circuit
{  
private:
    wire* w;
    std::map<std::string, wire*> wireMap;
    std::map<std::string, int> gateMap;
    std::vector<gate*> gates;
public:
    Circuit();
    void parseLine(std::string);
    void openFile(std::string);
    void printGates();
    void printWires();
};

#endif /* defined(__VParser__Circuit__) */

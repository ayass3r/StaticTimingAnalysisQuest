//
//  Edge.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/15/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Edge__
#define __VParser__Edge__

#include <stdio.h>
#include "gate.h"
#include <iostream>
#include <string>

class Edge
{
private:
    
    gate* gSource;
    gate* gDestination;
    std::string pin;
    bool topVisited;
public:
    std::string wireName;
    Edge(gate*, gate*, std::string, std::string);
    void setTopVisited(bool);
    bool getTopVisited();
    std::string getPin();
    gate* getSource();
    gate* getDestination();
};

#endif /* defined(__VParser__Edge__) */

//
//  Node.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Node.h"

Node::Node(std::string name)
{
    nodeName = name;
    loadCapacitance = 0.0;
    outSlewRate = 0.0;
    delay = 0.0;
}
std::string Node::getName()
{
    return nodeName;
}
float Node::calDelay(float inSlewRate)
{
    return 5.0;
}
float Node::calOutSlewRate()
{
    return 5.0;
}

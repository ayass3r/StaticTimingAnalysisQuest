//
//  Node.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Node__
#define __VParser__Node__

#include <stdio.h>
#include <iostream>
#include <string>

class Node
{
private:
    std::string nodeName; //name of the node
    //gate &refGate -> this is a refrence to a gate in the liberty file
    //inPort &refinPort -> this is a refrence to an input port
    //outPort &refoutPort -> this is a refrence to an output port
    float loadCapacitance; // total load capacitance on the output  pin
    float outSlewRate; //slew rate of the output pin
    float delay; //delay of the gate
public:
    Node(std::string);
    
    std::string getName(); //get name of the gate
    
    float calDelay(float inSlewRate); //calcu
    float calOutSlewRate();
};

#endif /* defined(__VParser__Node__) */

//
//  gate.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/11/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "gate.h"

gate::gate(std::string name, std::string type)
{
    gateName = name;
    gateType = type;
    isFlip = false;
    in1 = NULL;
    in2 = NULL;
    out1 = NULL;
}
void gate::setIsFlip(bool dflag)
{
    isFlip = dflag;
}
void gate::setIn1(wire *w1)
{
    in1 = w1;
}
wire* gate::getIn1()
{
    return in1;
}
void gate::setIn2(wire *w1)
{
    in2 = w1;
}
wire* gate::getIn2()
{
    return in2;
}
void gate::setOut(wire *w1)
{
    out1 = w1;
}
wire* gate::getOut()
{
    return out1;
}
std::string gate::getName()
{
    return gateName;
}
std::string gate::getType()
{
    return gateType;
}
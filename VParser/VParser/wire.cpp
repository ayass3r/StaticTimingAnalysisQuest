//
//  wire.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "wire.h"


wire::wire(std::string name, std::string type)
{
    wireName = name;
    wireType = type;
    assignWire = NULL;
    netCapacitance = 0.0;
    inSlewRate = 0.0;
}
std::string wire::getName()
{
    return wireName;
}
std::string wire::getWireType()
{
    return wireType;
}
void wire::setAssign(wire *w)
{
    assignWire = w;
}
wire* wire::getAssign()
{
    return assignWire;
}
void wire::setNetCap(float value)
{
    netCapacitance = value;
}
float wire::getNetCap()
{
    return netCapacitance;
}
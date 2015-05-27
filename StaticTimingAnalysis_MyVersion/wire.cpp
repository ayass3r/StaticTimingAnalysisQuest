#include "wire.h"


wire::wire(std::string name, std::string type)
{
    wireName = name;
    wireType = type;
    assignWire = NULL;
    wSource = NULL;
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
void wire::setWSource(gate* g)
{
    wSource = g;
}
gate* wire::getWSource()
{
    return wSource;
}
void wire::setWDestination(gate* g)
{
    wDestinations.push_back(g);
}
std::vector<gate* > wire::getWDestinations()
{
    return wDestinations;
}

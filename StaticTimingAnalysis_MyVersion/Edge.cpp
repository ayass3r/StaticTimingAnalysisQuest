#include "Edge.h"

Edge::Edge(gate* source, gate* destination, std::string name, int p)
{
    if(source == NULL || destination==NULL)
        std::cout << "Null pointer: " << name << " "<< std::endl ;
    gSource = source;
    gDestination = destination;
    topVisited = false;
    wireName = name;
    pin = p;
}

gate* Edge::getSource()
{
    return gSource;
}
gate* Edge::getDestination()
{
    return gDestination;
}
void Edge::setTopVisited(bool v)
{
    topVisited = v;
}
bool Edge::getTopVisited()
{
    return topVisited;
}
int Edge::getPin()
{
    return pin;
}
void Edge::setSlewRate(float c)
{
    inSlewRate =c;
}
float Edge::getSlewRate(){
    return inSlewRate;
}
void Edge::setNCapacitance(float c)
{
    netCapacitance = c;
}
float Edge::getNCapacitance()
{
    return netCapacitance;
}


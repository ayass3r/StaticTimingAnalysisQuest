//
//  Edge.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/15/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Edge.h"

Edge::Edge(gate* source, gate* destination, std::string name)
{
    if(source == NULL || destination==NULL)
        std::cout << "Null pointer" << name << " "<< std::endl ;
    gSource = source;
    gDestination = destination;
    topVisited = false;
    wireName = name;
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
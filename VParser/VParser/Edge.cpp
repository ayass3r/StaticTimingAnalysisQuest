//
//  Edge.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Edge.h"

Edge::Edge(std::string name)
{
    edgeName = name;
    destination = NULL;
    source = NULL;
    weightDelay = 0.0;
}
void Edge::setDestination(Node *n)
{
    destination = n;
}
Node* Edge::getDestination()
{
    return destination;
}
void Edge::setSource(Node *n)
{
    source = n;
}
Node* Edge::getSource()
{
    return source;
}
void Edge::setWeightDelay(float w)
{
    weightDelay = w;
}
float Edge::getWeightDelay()
{
    return weightDelay;
}

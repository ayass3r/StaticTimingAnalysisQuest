//
//  Edge.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/15/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Edge.h"

Edge::Edge(gate* source, gate* destination)
{
    gSource = source;
    gDestination = destination;
}
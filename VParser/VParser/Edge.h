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

class Edge
{
private:
    gate* gSource;
    gate* gDestination;
    bool topVisited;
public:
    Edge(gate*, gate*);
    void setTopVisited(bool);
    bool getTopVisited();
};

#endif /* defined(__VParser__Edge__) */

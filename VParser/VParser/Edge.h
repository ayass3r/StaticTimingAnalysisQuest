//
//  Edge.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/12/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Edge__
#define __VParser__Edge__

#include <stdio.h>
#include <vector>
#include "wire.h"
#include "Node.h"

class Edge
{
private:
    wire* wSource;
    Node* eSource;
    std::vector<Node*> eDestinations;
};
#endif /* defined(__VParser__Edge__) */

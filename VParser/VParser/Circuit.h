//
//  Circuit.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Circuit__
#define __VParser__Circuit__

#include <stdio.h>
#include "Edge.h"
#include "Node.h"
#include <string>
#include <iostream>
#include <vector>

class Circuit
{
private:
    std::vector<Node> inNodes;
    std::vector<Node> outNodes;
    std::vector<Node> midNodes;
    std::vector<Edge> edges;
public:
    
};

#endif /* defined(__VParser__Circuit__) */

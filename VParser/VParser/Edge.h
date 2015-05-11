//
//  Edge.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Edge__
#define __VParser__Edge__

#include <stdio.h>
#include "Node.h"
#include <string>
#include <iostream>

class Edge
{
private:
    std::string edgeName;
    Node* destination;
    Node* source;
    float weightDelay;
public:
    Edge(std::string);
    ~Edge();
    
    void setDestination(Node*);
    Node* getDestination();
    
    void setSource(Node*);
    Node* getSource();
    
    void setWeightDelay(float);
    float getWeightDelay();
    
    friend class Node;
};

#endif /* defined(__VParser__Edge__) */

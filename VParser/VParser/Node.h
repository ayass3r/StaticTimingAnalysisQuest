//
//  Node.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__Node__
#define __VParser__Node__

#include <stdio.h>
#include <iostream>
#include <string>

class Node
{
private:
    std::string name;
    std::string type;
    float weightDelay;
public:
    Node(std::string);
    void setWeightDelay(float);
    float getWeightDelay();
};

#endif /* defined(__VParser__Node__) */

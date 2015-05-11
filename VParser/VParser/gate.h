//
//  gate.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/11/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__gate__
#define __VParser__gate__

#include <stdio.h>
#include <string>
#include "wire.h"

class gate
{
public:
    std::string gateName;
    std::string gateType;
    bool isFlip;
    wire* in1;
    wire* in2;
    wire* out1;
};

#endif /* defined(__VParser__gate__) */

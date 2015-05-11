//
//  gate.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/11/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "gate.h"

gate::gate(std::string name, std::string type)
{
    gateName = name;
    gateType = type;
    isFlip = false;
    in1 = NULL;
    in2 = NULL;
    out1 = NULL;
}
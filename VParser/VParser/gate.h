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
    gate(std::string, std::string);
    
    void setIn1(wire*);
    wire* getIn1();
    
    std::string getName();
    std::string getType();
    
    void setIsFlip(bool);
    bool getIsFlip();
    
    void setIn2(wire*);
    wire* getIn2();
    
    void setOut(wire*);
    wire* getOut();
    
private:
    bool isFlip;
    wire* in1;
    wire* in2;
    wire* out1;
    std::string gateName;
    std::string gateType;
};

#endif /* defined(__VParser__gate__) */

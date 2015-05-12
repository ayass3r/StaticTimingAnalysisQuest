//
//  wire.h
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#ifndef __VParser__wire__
#define __VParser__wire__

#include <stdio.h>
#include <string>

class wire
{
private:
    std::string wireName;
    std::string wireType;
    float netCapacitance;
    float inSlewRate;
    wire* assignWire;
    
public:
    wire(std::string, std::string);
    
    std::string getName();
    std::string getWireType();
    
    void setAssign(wire*);
    wire* getAssign();
    
    void setNetCap(float);
    float getNetCap();
};

#endif /* defined(__VParser__wire__) */

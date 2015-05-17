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

class gate
{
    
private:
    bool isFlip, fIsIn; //flip is input or output
    
    std::string in1;
    std::string in2;
    std::string out1;
    std::string gateName;
    std::string gateType;
    float loadCapacitance; // total load capacitance on the output  pin
//    float outSlewRate; //slew rate of the output pin
    float delay; //delay of the gate
    float clkSkew;
    float fInSlew;
    
public:
    gate(std::string, std::string);
    
    void setFInSlew(float);
    float getFInSlew();
    
    void setFlipIn(bool);
    bool getFlipIn();
    
    
    void setDelay(float);
    float getDelay();
    
    void setIn1(std::string);
    std::string getIn1();
    
    std::string getName();
    std::string getType();
    
    void setIsFlip(bool);
    bool getIsFlip();
    
    void setIn2(std::string);
    std::string getIn2();
    
    void setOut(std::string);
    std::string getOut();

    void setFlipSkew(float);
    float getFlipSkew();

    void setLoadCap(float);
    float getLoadCap();
};

#endif /* defined(__VParser__gate__) */

#ifndef __VParser__Edge__
#define __VParser__Edge__

#include <stdio.h>
#include "gate.h"
#include <iostream>
#include <string>

class Edge
{
private:

    gate* gSource;
    gate* gDestination;
    int pin;
    bool topVisited;
    float netCapacitance;
    float inSlewRate;
public:
    void setNCapacitance(float);
    float getNCapacitance();

    void setSlewRate(float);
    float getSlewRate();

    std::string wireName;
    Edge(gate*, gate*, std::string, int);
    void setTopVisited(bool);
    bool getTopVisited();
    int getPin();
    gate* getSource();
    gate* getDestination();
};

#endif /* defined(__VParser__Edge__) */

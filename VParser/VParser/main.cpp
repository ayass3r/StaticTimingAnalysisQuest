//
//  main.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include <iostream>
#include <vector>
#include <exception>
#include "Circuit.h"

#define NETLIST "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/Test4.v"
#define NETCAP "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/Cap4.txt"
#define SKEW "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/skew4.txt"
#define CONSTRAINT "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/ConFile4.txt"

int main(int argc, const char * argv[]) {

    
    Circuit myTest;
    //std::vector<gate*> gSorted;
    
    try {
        myTest.createRoot();
        std::cout << "----------Parsing NetList File ...-------------\n";
        myTest.openFile(NETLIST);
        std::cout << "----------Done Parsing NetList-----------------\n";
        
        std::cout << "----------Parsing  Skew  File ...--------------\n";
        myTest.openSkewFile(SKEW);
        std::cout << "----------Done Parsing Skew File---------------\n";
        
        std::cout << "----------Parsing Capacitance File...----------\n";
        myTest.openCapFile(NETCAP);
        std::cout << "----------Done Parsing Capacitance File--------\n";
        
        std::cout << "----------Parsing Constraint File...-----------\n";
        myTest.openConFile(CONSTRAINT);
        std::cout << "----------Done Parsing Constraint File...------\n";
        
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    
    
    myTest.printGates();
    myTest.printWires();
    
    myTest.generateEges();
    myTest.topSort();
    
//    gSorted = myTest.topSort();
//    for(int i = 0; i < gSorted.size(); i++)
//        std::cout << i << ". " << gSorted[i]->getName()<<std::endl;
    myTest.WrapPrintPaths();
    return 0;
}

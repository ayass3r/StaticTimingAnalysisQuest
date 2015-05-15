//
//  main.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Circuit.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Circuit myTest;
    std::vector<gate*> gSorted;
    std::string filePath;
    //filePath = "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/VP/VP/code4.v";
    filePath = "/Users/macbookpro/Downloads/booth.g.v";
    myTest.createRoot();
    //myTest.createEnd();
    myTest.openFile(filePath);
    std::cout<<"--------------Done Parsing--------------\n";
    
    myTest.printGates();
    myTest.printWires();
    
    myTest.generateEges();
    myTest.topSort();
//    gSorted = myTest.topSort();
//    for(int i = 0; i < gSorted.size(); i++)
//        std::cout << i << ". " << gSorted[i]->getName()<<std::endl;
    return 0;
}

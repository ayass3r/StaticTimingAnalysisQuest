//
//  main.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include <iostream>
#include "Circuit.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Circuit myTest;
    std::string filePath;
    filePath = "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/VP/VP/code4.v";
    
    myTest.openFile(filePath);
    std::cout<<"--------------Done Parsing--------------\n";
    
    myTest.printGates();
    
    return 0;
}

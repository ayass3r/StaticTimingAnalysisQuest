#include <QCoreApplication>
//
//  main.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/9/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include <exception>

#include "Circuit.h"
//#include "STA.cpp"

/*#define LIBERTY "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/osu035.lib"
#define NETLIST "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/Test6.v"
#define NETCAP "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/Cap6.txt"
#define SKEW "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/skew6.txt"
#define CONSTRAINT "/Users/macbookpro/Desktop/Khodary/Courses/DDII/Projects/StaticTimingAnalysisQuest/TestCases/ConFile6.txt"
*/

#define LIBERTY "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysisQuest/osu035.lib"
#define NETLIST "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysisQuest/TestCases/Test6.v"
#define NETCAP "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysisQuest/TestCases/Cap6.txt"
#define SKEW "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysisQuest/TestCases/skew6.txt"
#define CONSTRAINT "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysisQuest/TestCases/ConFile6.txt"
int main()
{
    Circuit myTest;
    //std::vector<gate*> gSorted;
    try {

        myTest.createRoot();
        std::cout << "----------------Parsing NetList File ...-------------\n";
        myTest.openFile(NETLIST);
        std::cout << "----------------Done Parsing NetList-----------------\n";

        std::cout << "----------------Parsing  Skew  File ...--------------\n";
        myTest.openSkewFile(SKEW);
        std::cout << "----------------Done Parsing Skew File---------------\n";

        std::cout << "----------------Parsing Capacitance File...----------\n";
        myTest.openCapFile(NETCAP);
        std::cout << "----------------Done Parsing Capacitance File--------\n";

        std::cout << "----------------Parsing Constraint File...-----------\n";
        myTest.openConFile(CONSTRAINT);
        std::cout << "----------------Done Parsing Constraint File...------\n";

        std::cout << "----------------Printing Gates-----------------------\n";
        myTest.printGates();
        std::cout << "----------------Done Printing Gates------------------\n";

        std::cout << "----------------Printing Wires-----------------------\n";
        myTest.printWires();
        std::cout << "----------------Done Printing Wires------------------\n";

        std::cout << "----------------Generating Edges---------------------\n";
        myTest.generateEges();
        std::cout << "----------------Edges Generated----------------------\n";

        std::cout << "----------------Graph Toplogical Sort----------------\n";
        myTest.topSort();
        std::cout << "----------------Graph Done Sorting-------------------\n";

//    gSorted = myTest.topSort();
//    for(int i = 0; i < gSorted.size(); i++)
//        std::cout << i << ". " << gSorted[i]->getName()<<std::endl;
        std::cout << "----------------Finding all Paths--------------------\n";
        myTest.WrapPrintPaths();
        std::cout << "----------------All paths displayed------------------\n";

    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}


#include <QCoreApplication>

#include <exception>

#include "Circuit.h"


#define LIBERTY "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysis_MyVersion/osu035.lib"
#define NETLIST "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysis_MyVersion/TestCases/Test2.v"
#define NETCAP "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysis_MyVersion/TestCases/Cap2.txt"
#define SKEW "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysis_MyVersion/TestCases/skew2.txt"
#define CONSTRAINT "D:/AUC/Spring 2015/Digital 2/Projects/Project2/StaticTimingAnalysis_MyVersion/TestCases/ConFile2.txt"
int main()
{
    Circuit myTest;
    std::vector<gate*> gSorted;
    try {

        myTest.createRoot();
        std::cout << "----------------Parsing Liberty File ...-------------\n\n";
        myTest.parselib(LIBERTY);
        std::cout << "----------------Done Parsing Liberty-----------------\n\n";
        std::cout << "----------------Parsing NetList File ...-------------\n\n";
        myTest.openFile(NETLIST);
        std::cout << "----------------Done Parsing NetList-----------------\n\n";

        std::cout << "----------------Parsing  Skew  File ...--------------\n\n";
        myTest.openSkewFile(SKEW);
        std::cout << "----------------Done Parsing Skew File---------------\n\n";

        std::cout << "----------------Parsing Capacitance File...----------\n\n";
        myTest.openCapFile(NETCAP);
        std::cout << "----------------Done Parsing Capacitance File--------\n\n";

        std::cout << "----------------Parsing Constraint File...-----------\n\n";
        myTest.openConFile(CONSTRAINT);
        std::cout << "----------------Done Parsing Constraint File---------\n\n";

        //std::cout << "----------------Printing Gates...--------------------\n\n";
        //myTest.printGates();
        //std::cout << "----------------Done Printing Gates------------------\n\n";

        //std::cout << "----------------Printing Wires...---------------------\n\n";
        //myTest.printWires();
        //std::cout << "----------------Done Printing Wires------------------\n\n";

        std::cout << "----------------Generating Edges...---------------------\n\n";
        myTest.generateEges();
        std::cout << "----------------Edges Generated----------------------\n\n";

        std::cout << "----------------Graph Toplogical Sort...--------------\n\n";
        gSorted = myTest.topSort();
        std::cout << "----------------Graph Done Sorting--------------------\n\n";
        std::cout << "----------------Calculate Gate Output Load...---------\n\n";
        myTest.calcGatesOutputLoad();
        std::cout << "----------------Done Calculating Gate Output Load-----\n\n";
        std::cout << "----------------Calculate Gate Delays...--------------\n\n";
        myTest.trvGate(gSorted);
        std::cout << "----------------Done Calculating Gate Delays----------\n\n";
        std::cout << "----------------Finding all Paths...------------------\n\n";
        myTest.WrapPrintPaths();
        std::cout << "----------------All paths displayed------------------\n";

    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}


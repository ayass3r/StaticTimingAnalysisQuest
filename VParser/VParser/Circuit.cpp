//
//  Circuit.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Circuit.h"


Circuit::Circuit()
{
    w = NULL;
    g = NULL;
    pGate = "";
}
void Circuit::parseLine(std::string line)
{
    std::regex sWire("\\s*(wire|input|output) +(\\w+)\\s*;\\s*");
    std::regex mWire("\\s*(wire|input|output)\\s+\\[(\\d+)\\:(\\d+)\\]\\s*(\\w+)\\s*;\\s*");
    std::regex rGate("\\s*(\\w+)\\s+(\\w+)\\s*\\(\\s*");
    std::regex in1("\\s*.(CLK|A)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*,\\s*");
    std::regex in2("\\s*.(B|D)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*,\\s*");
    std::regex rOut("\\s*.(Y|Q)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*");
    std::regex assign1("\\s*assign\\s+(\\w+\\[\\d+\\]|\\w+)\\s+\\=\\s+(\\w+\\[\\d+\\]|\\w+)\\s*;\\s*");
    std::smatch results;
    
    if(std::regex_match(line, results, sWire))
    {
        std::cout << results.str(1) << " Found " <<results.str(2) << std::endl;
        w = new wire(results.str(2), results.str(1));
        if(results.str(1) == "input"){
            g = new gate(results.str(2), results.str(1));
            g->setIn1("root");
            g->setOut(results.str(2));
            w->setWSource(g);
            wireMap["root"]->setWDestination(g);
            gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        }
        wireMap.insert(std::pair<std::string, wire*> (results.str(2), w));
    }
    else if (std::regex_match(line, results, mWire))
    {
        std::cout << results.str(1) << " bus Found " <<results.str(2) << ":"<< results.str(3) << ' '<< results.str(4) << std::endl;
        for(int i = std::atoi(results.str(3).c_str()); i <= std::atoi(results.str(2).c_str()); i++){
            std::string x = results.str(4);
            x += '[' + std::to_string(i) + ']';
            w = new wire(x, results.str(1));
            if(results.str(1) == "input"){
                g = new gate(x, results.str(1));
                g->setIn1("root");
                g->setOut(x);
                w->setWSource(g);
                wireMap["root"]->setWDestination(g);
                gateMap.insert(std::pair<std::string, gate*> (x, g));
            }
            wireMap.insert(std::pair<std::string, wire*>(x, w));
        }
    }
    else if (std::regex_match(line, results, rGate))
    {
        pGate = results.str(2);
        std::cout << "Gate Found " << results.str(1) << ' ' <<results.str(2) << std::endl;
        g = new gate(results.str(2), results.str(1));
        gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        gateMap[results.str(2)]->setIsFlip((results[1] == "DFFPOSX1") ? true : false);
    }
    else if (std::regex_match(line, results, in1)){
        std::cout<<"Input: "<< results.str(2) << " found" << std::endl;
        gateMap[pGate]->setIn1(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate]);
    }
    
    else if (std::regex_match(line, results, in2)){
        std::cout<<"Input: "<< results.str(2) << " found" << std::endl;
        gateMap[pGate]->setIn2(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate]);
    }
    
    else if (std::regex_match(line, results, rOut)){
        std::cout<<"Output: "<< results.str(2) << " found" << std::endl;
        gateMap[pGate]->setOut(results.str(2));
        wireMap[results.str(2)]->setWSource(gateMap[pGate]);
    }
    
    else if (std::regex_match(line, results, assign1))
    {
        std::cout<<"Assign: "<< results.str(1) << " to" << results.str(2)<< " found" <<std::endl;
        wire *temp1, *temp2;
        temp1 = wireMap[results.str(1)];
        temp2 = wireMap[results.str(2)];
        temp1->setAssign(temp2);
    }
}
void Circuit::createRoot()
{
    g = new gate("root", "0");
    w = new wire("root", "0");
    g->setOut("root");
    w->setWSource(g);
    gateMap.insert(std::pair<std::string, gate*> ("root", g));
    wireMap.insert(std::pair<std::string, wire*> ("root", w));
}
void Circuit::openFile(std::string filePath)
{
    std::ifstream ins;
    std::string line;
    
    ins.open(filePath);
    if(ins.fail()){
        throw std::invalid_argument("File path invaild!\n");
    }
    else{
        while(!ins.eof())
        {
            std::getline(ins, line);
            //try
            //{
                parseLine(line);
            //}
            //catch(std::exception& e){
                //throw std::invalid_argument("file formate invalid");
            //}
        }
    }
}
void Circuit::printGates()
{
    for(std::map<std::string, gate*>::iterator i = gateMap.begin(); i != gateMap.end(); i++)
    {
        std::cout << i->second->getType() << ' ' << i->second->getName() << std::endl;
        std::cout << '\t' << "iPort1: " << i->second->getIn1() /*<< " type: " << gates[i]->getIn1()->getWireType() */ << std::endl;
        if(i->second->getIn2() != "")
            std::cout << '\t' << "iPort2: " << i->second->getIn2() /* << " type: " << gates[i]->getIn2()->getWireType() */ << std::endl;
        
        std::cout << '\t' << "oPort3: " << i->second->getOut() /* << " type: " << gates[i]->getOut()->getWireType() */ << std::endl;
    }
    std::cout << "---------------Done Printing Gates----------------\n";
}
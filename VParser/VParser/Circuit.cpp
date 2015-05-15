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
    pGate2 = "";
}
void Circuit::parseLine(std::string line)
{
    std::regex sWire("\\s*(wire|input|output) +(\\w+)\\s*;\\s*");
    std::regex mWire("\\s*(wire|input|output)\\s+\\[(\\d+)\\:(\\d+)\\]\\s*(\\w+)\\s*;\\s*");
    std::regex rGate("\\s*(\\w+)\\s+(\\w+)\\s*\\(\\s*");
    std::regex in1("\\s*.(A)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*,\\s*");
    std::regex in2("\\s*.(B|D)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*,\\s*");
    std::regex rOut("\\s*.(Y|Q)\\((\\w+\\[\\d+\\]|\\w+)\\)\\s*");
    std::regex assign1("\\s*assign\\s+(\\w+\\[\\d+\\]|\\w+)\\s+\\=\\s+(\\w+\\[\\d+\\]|\\w+)\\s*;\\s*");
    std::smatch results;
    
    if(std::regex_match(line, results, sWire))
    {
        std::cout << results.str(1) << " Found " <<results.str(2) << std::endl;
        w = new wire(results.str(2), results.str(1));
        if(results.str(1) != "wire"){
            g = new gate(results.str(2), results.str(1));
            if(results.str(1) == "input"){
                g->setIn1("root");
                g->setOut(results.str(2));
                w->setWSource(g);
                wireMap["root"]->setWDestination(g);
            }
            else {
                g->setIn1(results.str(2));
                w->setWDestination(g);
            }
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
            if(results.str(1) != "wire"){
                g = new gate(x, results.str(1));
                if(results.str(1) == "input"){
                    g->setIn1("root");
                    g->setOut(x);
                    w->setWSource(g);
                    wireMap["root"]->setWDestination(g);
                    gateMap.insert(std::pair<std::string, gate*> (x, g));
                }
                else {
                    g->setIn1(x);
                    w->setWDestination(g);
                }
            }
            wireMap.insert(std::pair<std::string, wire*>(x, w));
        }
    }
    else if (std::regex_match(line, results, rGate))
    {
        pGate2 = pGate = results.str(2);
        std::cout << "Gate Found " << results.str(1) << ' ' <<results.str(2) << std::endl;
        if(results.str(1) == "DFFPOSX1"){
            pGate += "_input";
            pGate2 += "_output";
            g = new gate(pGate, results.str(1));
            g->setIn1("root");
            wireMap["root"]->setWDestination(g);
            gateMap.insert(std::pair<std::string, gate*> (pGate, g));
            gateMap[pGate]->setIsFlip(true);
            
            g = new gate(pGate2, results.str(1));
            gateMap.insert(std::pair<std::string, gate*> (pGate2, g));
        }
        else{
            g = new gate(results.str(2), results.str(1));
            gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        }
        
    }
    else if (std::regex_match(line, results, in1)){
        std::cout<<"InputPin1: "<< results.str(2) << " found" << std::endl;
        gateMap[pGate2]->setIn1(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    
    else if (std::regex_match(line, results, in2)){
        std::cout<<"InputPin2: "<< results.str(2) << " found" << std::endl;
        gateMap[pGate2]->setIn2(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    
    else if (std::regex_match(line, results, rOut)){
        std::cout<<"OutputPin: "<< results.str(2) << " found" << std::endl;
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
    
    std::cout << gateMap["root"]->getName() << std::endl;
}
/*void Circuit::createEnd()
{
    g = new gate("end", "1");
    w = new wire("end", "1");
    g->setIn1("end");
    w->setWDestination(g);
    gateMap.insert(std::pair<std::string, gate*> ("end", g));
    wireMap.insert(std::pair<std::string, wire*> ("end", w));
}*/
void Circuit::generateEges()
{
    Edge* e;
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
    {
        for(int j = 0; j < i->second->getWDestionations().size(); j++)
            e = new Edge(i->second->getWSource(), i->second->getWDestionations()[j]);
    }
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
        std::cout << '\t' << "iPort1: " << i->second->getIn1() << std::endl;
        if(i->second->getIn2() != "")
            std::cout << '\t' << "iPort2: " << i->second->getIn2() << std::endl;
        
        std::cout << '\t' << "oPort3: " << i->second->getOut() << std::endl;
    }
    std::cout << "---------------Done Printing Gates----------------\n";
}
void Circuit::printWires()
{
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
    {
        std::cout << i->second->getWireType() << ' ' << i->second->getName() << std::endl;
        
        if(i->second->getWSource() != NULL)
            std::cout << '\t' << "wSource: " << i->second->getWSource()->getName() << std::endl;
        else std::cout << "NO CONNECTION TO SOURCE\n";
        
        for(int x = 0; x < i->second->getWDestionations().size(); x++)
            std::cout << '\t' << "wDestinations: " << i->second->getWDestionations()[x]->getName() << std::endl;
    }
    std::cout << "---------------Done Printing Wires----------------\n";
}
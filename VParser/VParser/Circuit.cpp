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
void Circuit::openCapFile(std::string filepath)
{
    std::ifstream ins;
    std::string line;
    
    ins.open(filePath);
    if(ins.fail()){
        throw std::invalid_argument("Cap File path invaild!\n");
    }
    else{
        while(!ins.eof())
        {
            std::getline(ins, line);
            if(!parseCapLine(line))
                std::cout << "\nNOT PARSED!: " << line << "\n\n";
        }
    }
    ins.close();
}
bool Circuit::parseCapLine(std::string line)
{
    std::regex capLine("\\s*(\\S+)\\s+(\\d*.\\d*)");
    std::smatch results;
    std::string::size_type sz;

    if (std::regex_match(line, results, capLine))
    {
        std::cout<<results.str(1)<<results.str(2)<<std::endl;
        float x = stof(results.str(2), &sz);
        wireMap[results.str(1)]->setNetCap(x);
        return true;
    }
    else return false;
}
void Circuit::openSkewFile(std::string filepath)
{
    std::ifstream ins;
    std::string line;
    
    ins.open(filePath);
    if(ins.fail()){
        throw std::invalid_argument("Skew File path invaild!\n");
    }
    else{
        while(!ins.eof())
        {
            std::getline(ins, line);
            if(!parseSkewLine(line))
                std::cout << "\nNOT PARSED!: " << line << "\n\n";
        }
    }
    ins.close();
}
bool Circuit::parseSkewLine(std::string line)
{
    std::regex skewLine("\\s*(\\S+)\\s+(\\d*.\\d*)");
    std::smatch results;
    std::string::size_type sz;

    if (std::regex_match(line, results, skewLine))
    {
        std::cout<<results.str(1)<<results.str(2)<<std::endl;
        float x = stof(results.str(2), &sz);
        if (gateMap[results.str(1)]->getIsFlip())
            gateMap[results.str(1)]->setFlipSkew(x);
        else 
        {
            std::cout << "Flip Flop doesn't exist or Gate is not flip flop"<< std::endl;
            return false;
        }
        return true;
    }
    else return false;
}
bool Circuit::parseLine(std::string line)
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
        std::cout << results.str(1) <<results.str(2) << std::endl;
        w = new wire(results.str(2), results.str(1));
        if(results.str(1) != "wire")
        {
            g = new gate(results.str(2), results.str(1));
            if(results.str(1) == "input")
            {
                g->setIn1("root");
                g->setOut(results.str(2));
                w->setWSource(g);
                wireMap["root"]->setWDestination(g);
            }
            else
            {
                g->setIn1(results.str(2));
                w->setWDestination(g);
            }
            gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        }
        wireMap.insert(std::pair<std::string, wire*> (results.str(2), w));
    }
    else if (std::regex_match(line, results, mWire))
    {
        std::cout << results.str(1) << " bus " <<results.str(2) << ":"<< results.str(3) << ' '<< results.str(4) << std::endl;
        for(int i = std::atoi(results.str(3).c_str()); i <= std::atoi(results.str(2).c_str()); i++)
        {
            std::string x = results.str(4);
            x += '[' + std::to_string(i) + ']';
            w = new wire(x, results.str(1));
            if(results.str(1) != "wire")
            {
                g = new gate(x, results.str(1));
                if(results.str(1) == "input")
                {
                    g->setIn1("root");
                    g->setOut(x);
                    w->setWSource(g);
                    wireMap["root"]->setWDestination(g);
                    gateMap.insert(std::pair<std::string, gate*> (x, g));
                }
                else
                {
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
        std::cout << "\nGate " << results.str(1) << ' ' <<results.str(2) << std::endl;
        if(results.str(1) == "DFFPOSX1")
        {
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
        else
        {
            g = new gate(results.str(2), results.str(1));
            gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        }
    }
    else if (std::regex_match(line, results, in1))
    {
        std::cout<<"InputPin1: "<< results.str(2) << std::endl;
        gateMap[pGate2]->setIn1(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    else if (std::regex_match(line, results, in2))
    {
        std::cout<<"InputPin2: "<< results.str(2) << std::endl;
        gateMap[pGate2]->setIn2(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    else if (std::regex_match(line, results, rOut))
    {
        std::cout<<"OutputPin: "<< results.str(2) << std::endl;
        gateMap[pGate]->setOut(results.str(2));
        wireMap[results.str(2)]->setWSource(gateMap[pGate]);
    }
    else if (std::regex_match(line, results, assign1))
    {
        std::cout<<"Assign: "<< results.str(1) << " to" << results.str(2) <<std::endl;
        wire *temp1, *temp2;
        temp1 = wireMap[results.str(1)];
        temp2 = wireMap[results.str(2)];
        temp1->setAssign(temp2);
    }
    else return false;
    
    return true;
}
void Circuit::createRoot()
{
    g = new gate("root", "0");
    w = new wire("root", "0");
    g->setOut("root");
    w->setWSource(g);
    gateMap.insert(std::pair<std::string, gate*> ("root", g));
    wireMap.insert(std::pair<std::string, wire*> ("root", w));
    
    //std::cout << gateMap["root"]->getName() << std::endl;
}
void Circuit::generateEges()
{
    Edge* e;
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
    {
        for(int j = 0; j < i->second->getWDestionations().size(); j++)
        {
            e = new Edge(i->second->getWSource(), i->second->getWDestionations()[j], i->second->getName());
            vEdges.push_back(e);
        }
    }
//    for (int i = 0; i < vEdges.size(); i++)
//        std::cout << i << ". "<<vEdges[i]->wireName <<"\nSource: "<< vEdges[i]->getSource()->getName() << "\nDestination: " << vEdges[i]->getDestination()->getName()<< std::endl;
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
            if(!parseLine(line))
                std::cout << "\nNOT PARSED!: " << line << "\n\n";
        }
    }
    ins.close();
}

std::vector<gate*> Circuit::topSort()
{
    std::queue<gate*> S;
    std::vector<gate*> L;
    gate* n;
    bool setToS;
    S.push(gateMap["root"]);
    while (S.size())
    {
        n = S.front();
        S.pop();
        L.push_back(n);
  //      for (int i = 0; i < vEdges.size(); i++)
 //       {
//            std::cout<<n->getName()<<std::endl;
//            std::cout<<vEdges[i]->getSource()->getName()<<std::endl;
            if ((vEdges[i]->getSource()->getName() == n->getName()) && !vEdges[i]->getTopVisited())
            {
                vEdges[i]->setTopVisited(true);
                setToS = true;
                for (int j = 0; j < vEdges.size(); j++)
                    if ((vEdges[j]->getDestination()->getName() == vEdges[i]->getDestination()->getName()) && !vEdges[j]->getTopVisited())
                    {
                        setToS = false; break;
                    }
                if (setToS) S.push(vEdges[i]->getDestination());
            }
//        }
    }
    for (int i = 0; i < vEdges.size(); i++) if (!vEdges[i]->getTopVisited()) {std::cout <<"Graph has cycles"<<std::endl; break;}
    
    for(int i = 0; i < L.size(); i++)
        std::cout << i << ". " << L[i]->getName()<<std::endl;
    
    return L;
    
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
void Circuit::calcGatesOutputLoad()
{
    float tempC;
    for(std::map<std::string, gate*>::iterator i = gateMap.begin(); i != gateMap.end(); i++)
    {
        if (i->getType() != "input" && i->getType() != "output")
        {
            tempC = i->getLoadCap();
            tempC+=wireMap[i->getOut()]->getLoadCap(); //add capacitance of output wire
            for (int j = 0; j < wireMap[i->getOut()]->getWDestionations().size(); j++) //loop over the fan out of gate
            {
                if (gateMap[wireMap[i->getOut()]->getWDestionations()[j]].getIn1() == wireMap[i->getOut()].getName)
                    // add capacitance of pin In1 of gate type to tempC
                else if (gateMap[wireMap[i->getOut()]->getWDestionations()[j]].getIn2() == wireMap[i->getOut()].getName)
                    // add capacitance of pin In2 of gate type to tempC
            }
        }
        i->setLoadCap(tempC);
    }
}
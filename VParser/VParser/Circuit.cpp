//
//  Circuit.cpp
//  VParser
//
//  Created by Mahmoud Khodary on 5/10/15.
//  Copyright (c) 2015 Mahmoud Khodary. All rights reserved.
//

#include "Circuit.h"
#include <regex>
#include <fstream>
#include <iostream>

Circuit::Circuit()
{
    
}

void Circuit::parseLine(std::string line)
{
    std::regex sWire(R"(\s*(wire|input|output) +(\w+)\s*)");
    std::regex mWire(R"(\s*(wire|input|output)\s+\[(\d+)\:(\d+)\]\s*(\w+)\s*)");
    std::regex rGate(R"(\s*(\w+)\s+(\w+)\s*)");
    std::regex in1(R"(\s*.[A|CLK]\((\w+))");
    std::regex in2(R"(\s*.[B|D]\((\w+))");
    std::regex rOut(R"(\s*.[Y|G]\((\w+))");
    std::regex assign1(R"(\s*assign\s+(\w+|\w+\[\d+\])\s+\=\s+(\w+\[\d+\]|\w+)\s*)");
    std::smatch results;
    
    if(std::regex_match(line, results, sWire))
    {
        int wType;
        if(results[1] == "wire") wType = 0;
        else if(results[1] == "input") wType = 1;
        else wType = 2; //else it's an output
        
        w = new wire(results[2], wType);
        wireMap.insert(std::pair<std::string, wire*> (results[2], w));
    }
    else if (std::regex_match(line, results, mWire))
    {
        int wType;
        if(results[1] == "wire") wType = 0;
        else if(results[1] == "input") wType = 1;
        else wType = 2; //else it's an output
        
        for(int i = std::atoi(results.str(3).c_str()); i < std::atoi(results.str(2).c_str()); i++){
            std::string x = results.str(4);
            x += '[' + std::to_string(i) + ']';
            w = new wire(x, wType);
            wireMap.insert(std::pair<std::string, wire*>(x, w));
        }
    }
    else if (std::regex_match(line, results, rGate))
    {
        gates.push_back(new gate);
        gates[gates.size()-1]->gateType = results[1];
        gates[gates.size()-1]->gateName = results[2];
        gateMap.insert(std::pair<std::string, int> (results[2], gates.size()-1));
        gates[gates.size()-1]->isFlip = (results[1] == "DFFPOSX1") ? true : false;
    }
    else if (std::regex_match(line, results, in1))
        gates[gates.size()-1]->in1 = wireMap[results[1]];
    
    else if (std::regex_match(line, results, in2))
        gates[gates.size()-1]->in2 = wireMap[results[1]];
    
    else if (std::regex_match(line, results, rOut))
        gates[gates.size()-1]->out1 = wireMap[results[1]];
    else if (std::regex_match(line, results, assign1))
    {
        wire *temp1, *temp2;
        temp1 = wireMap[results[1]];
        temp2 = wireMap[results[2]];
        temp1->setAssign(temp2);
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
//            }
//            catch(std::exception& e){
//                throw std::invalid_argument("file formate invalid");
//            }
        }
    }
}
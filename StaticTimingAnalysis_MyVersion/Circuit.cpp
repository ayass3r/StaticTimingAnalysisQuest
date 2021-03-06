#include "Circuit.h"
#include "LIBPARSER.cpp"
#include <iomanip>
#include <algorithm>

Circuit::Circuit()
{
    w = NULL;
    g = NULL;
    pGate = "";
    pGate2 = "";
}
float Circuit::getClk()
{
    return clkPeriod;
}
void Circuit::openFile(std::string filePath)
{
    std::ifstream ins;
    std::string line;
    int counter = 0;

    ins.open(filePath);
    if(ins.fail()){
        throw std::invalid_argument("File path invaild!\n");
    }
    else{
        while(!ins.eof())
        {
            counter++;
            std::getline(ins, line);
            if(!parseLine(line) && line != ");");
            //    std::cout << '(' << counter << ") NOT PARSED!: " << line << "\n";
        }
    }
    ins.close();
}
void Circuit::openCapFile(std::string filepath)
{
    std::ifstream ins;
    std::string line;

    ins.open(filepath);
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
void Circuit::openConFile(std::string filePath)
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
            if(!parseConFile(line))
                std::cout << "\nNOT PARSED!: " << line << "\n\n";
        }
    }
    ins.close();
}

void Circuit::parselib(const char * f)
{
    ParseLiberty(f);
}
void Circuit::openSkewFile(std::string filepath)
{
    std::ifstream ins;
    std::string line;

    ins.open(filepath);
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
bool Circuit::parseConFile(std::string line)
{
    std::regex clk("\\s*ClockPeriod\\s+(\\d*.\\d*)\\s*");
    std::regex io("\\s*(\\w+)\\s+(\\d*.\\d*)\\s*");
    std::smatch results;

    if(std::regex_match(line, results, clk))
        clkPeriod = atof(results.str(1).c_str());
    else if(std::regex_match(line, results, io))
        gateMap[results.str(1)]->setDelay(atof(results.str(2).c_str()));
    else return false;

    return true;
}
bool Circuit::parseCapLine(std::string line)
{
    std::regex capLine("\\s*(\\S+)\\s+(\\d*.\\d*)");
    std::smatch results;

    if (std::regex_match(line, results, capLine))
    {
        //std::cout << results.str(1) << ' ' << results.str(2)<<std::endl;
        float x = atof(results.str(2).c_str());
        wireMap[results.str(1)]->setNetCap(x);
        return true;
    }
    else return false;
}
bool Circuit::parseSkewLine(std::string line)
{
    std::regex skewLine("\\s*(\\S+)\\s+(\\d*.\\d*)\\s+(\\d*.\\d*)\\s*");
    std::smatch results;
    std::string::size_type sz;

    if (std::regex_match(line, results, skewLine))
    {
        //std::cout<<results.str(1)<<results.str(2)<<std::endl;
        float x = stof(results.str(2), &sz);
        float y = stof(results.str(3), &sz);
        if (gateMap[results.str(1)+"_input"]->getIsFlip()){
            gateMap[results.str(1)+"_input"]->setFlipSkew(x);
            gateMap[results.str(1)+"_input"]->setFInSlew(y);

            gateMap[results.str(1)+"_output"]->setFlipSkew(x);
            gateMap[results.str(1)+"_output"]->setFInSlew(y);

        }
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
        //std::cout << results.str(1) <<" "<< results.str(2) << std::endl;
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
        //std::cout << results.str(1) << " bus " <<results.str(2) << ": "<< results.str(3) << ' '<< results.str(4) << std::endl;
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
        //std::cout << "\nGate " << results.str(1) << ' ' <<results.str(2) << std::endl;
        if(results.str(1) == "DFFPOSX1")
        {
            pGate += "_input";
            pGate2 += "_output";
            g = new gate(pGate, results.str(1));
            g->setIn1("root");
            wireMap["root"]->setWDestination(g);
            gateMap.insert(std::pair<std::string, gate*> (pGate, g));
            gateMap[pGate]->setIsFlip(true);
            gateMap[pGate]->setFlipIn(true);

            g = new gate(pGate2, results.str(1));
            g->setFlipIn(true);
            gateMap.insert(std::pair<std::string, gate*> (pGate2, g));
            gateMap[pGate2]->setIsFlip(true);
            gateMap[pGate2]->setFlipIn(false);
        }
        else
        {
            g = new gate(results.str(2), results.str(1));
            g->setFlipIn(false);
            gateMap.insert(std::pair<std::string, gate*> (results.str(2), g));
        }
    }
    else if (std::regex_match(line, results, in1))
    {
        //std::cout<<"InputPin1: "<< results.str(2) << std::endl;
        gateMap[pGate2]->setIn1(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    else if (std::regex_match(line, results, in2))
    {
        //std::cout<<"InputPin2: "<< results.str(2) << std::endl;
        gateMap[pGate2]->setIn2(results.str(2));
        wireMap[results.str(2)]->setWDestination(gateMap[pGate2]);
    }
    else if (std::regex_match(line, results, rOut))
    {
        //std::cout<<"OutputPin: "<< results.str(2) << std::endl;
        gateMap[pGate]->setOut(results.str(2));
        wireMap[results.str(2)]->setWSource(gateMap[pGate]);
    }
    else if (std::regex_match(line, results, assign1))
    {
        //std::cout<<"Assign: "<< results.str(1) << " to" << results.str(2) <<std::endl;
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
    int pin;
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
    {
        for(int j = 0; j < i->second->getWDestinations().size(); j++)
        {
            if(gateMap[i->second->getWDestinations()[j]->getName()] != NULL){
                if(gateMap[i->second->getWDestinations()[j]->getName()]->getIn2() == i->second->getName())
                    pin = 1;
                else if(gateMap[i->second->getWDestinations()[j]->getName()]->getIn1() == i->second->getName())
                    pin = 0;
            }
            else
                throw std::invalid_argument("Invalid Routing\n");

            e = new Edge(i->second->getWSource(), i->second->getWDestinations()[j], i->second->getName(), pin);
            e->wireName = i->second->getName();
            e->setNCapacitance(i->second->getNetCap());
            vEdges.push_back(e);


        }
    }
    //for (int i = 0; i < vEdges.size(); i++)
      //  std::cout << i << ". " << vEdges[i]->wireName << "\nPin: " <<vEdges[i]->getPin() <<"\nSource: "<< vEdges[i]->getSource()->getName() << "\nDestination: " << vEdges[i]->getDestination()->getName()<< "\n\n";
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
        for (int i = 0; i < vEdges.size(); i++)
        {
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
        }
    }
    for (int i = 0; i < vEdges.size(); i++) if (!vEdges[i]->getTopVisited()) {std::cout <<"Graph has cycles"<<std::endl; break;}


    for(int i = 0; i < L.size(); i++)
      //  std::cout << i << ". " << L[i]->getName()<<std::endl;

    return L;

}
void Circuit::printGates()
{
    for(std::map<std::string, gate*>::iterator i = gateMap.begin(); i != gateMap.end(); i++)
    {
        std::cout << i->second->getType() << ' ' << i->second->getName() << std::endl;
        if(i->second->getIsFlip()){
            std::cout << '\t' <<"FlipFlop ClockSkew: " << i->second->getFlipSkew() << std::endl;
            std::cout << '\t' << "FlipFlop Input Slew: " << i->second->getFInSlew() << std::endl;
        }
        std::cout << '\t' << "iPort1: " << i->second->getIn1() << std::endl;
        if(i->second->getIn2() != "")
            std::cout << '\t' << "iPort2: " << i->second->getIn2() << std::endl;

        std::cout << '\t' << "oPort3: " << i->second->getOut() << std::endl;
    }
}
void Circuit::printWires()
{
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
    {
        std::cout << i->second->getWireType() << ' ' << i->second->getName() << std::endl;

        if(i->second->getWSource() != NULL)
            std::cout << '\t' << "wSource: " << i->second->getWSource()->getName() << std::endl;
        else std::cout << "NO CONNECTION TO SOURCE\n";

        for(int x = 0; x < i->second->getWDestinations().size(); x++)
            std::cout << '\t' << "wDestinations: " << i->second->getWDestinations()[x]->getName() << std::endl;
    }
}
void Circuit::getPaths(gate* g)//gets all paths in the circuit and prints them
{
    Paths.push_back(g); // push this gate into the vector
    if (Paths.size() > 1) PathDelay+=Paths[Paths.size()-2]->getDelay(); //add delay of previous gate
    for(std::map<std::string, wire*>::iterator i = wireMap.begin(); i != wireMap.end(); i++)
        if (i->second->getWSource()->getName() == g->getName())
            for (int j = 0; j < i->second->getWDestinations().size(); j++)
                getPaths(i->second->getWDestinations()[j]);
    if ((g->getIsFlip() && !g->getFlipIn()) || g->getType() == "output") //if the gate is an output flipflop or normal output
        printPaths(calcReqTime(Paths));
    Paths.pop_back();
    PathDelay-= g->getDelay();
}
void Circuit::WrapPrintPaths()
{
    getPaths(gateMap["root"]);
}
void Circuit::printPaths(float c)
{
    float tempDelay = 0.0;
    std::cout <<"------------------------------------------------------------\n";
    std::cout <<std::setw(12)<<"Gate_Name"<<std::setw(12)<< "Type" <<std::setw(17)<< "Inc_Delay" <<std::setw(17)<< "Path_Delay\n";
    std::cout <<"------------------------------------------------------------\n";
    for (int i = 1; i<Paths.size(); i++)
    {std::cout<<std::setw(12)<<Paths[i]->getName()<<std::setw(12)<<Paths[i]->getType()<<std::setw(12)<<Paths[i]->getDelay()<<std::setw(12)<<tempDelay<<"\n";
            tempDelay+=Paths[i]->getDelay();}
    std::cout <<"------------------------------------------------------------\n";
    std::cout <<"Data Arrival Time:\t\t"<<tempDelay<<"\n";
    std::cout <<"Data Required Time:\t\t"<<c<<"\n";
    std::cout <<"Slack Time:\t\t"<<c - tempDelay<<"\n";
    std::cout <<"------------------------------------------------------------\n\n";
}
float Circuit::calcReqTime(vector<gate*> Path)
{
    float c = clkPeriod;
    int x = Path.size();
    if (Path[x-1]->getIsFlip())
    {
        CellClass* temp; CellPin* tempPin; std::string s = Path[x-1]->getType();
        QString qs = QString::fromStdString(s);
        temp = findClass(qs);
        c -= temp->SetupTiming.getMax(Path[x-1]->getFlipSkew(),Path[x-1]->getLoadCap());
        c += Path[x-1]->getFlipSkew();
    }
    else
        c-= Path[x-1]->getDelay();
    if (Path[1]->getIsFlip())
        c-= Path[x-1]->getFlipSkew();
    return c;

}
void Circuit::calcGatesOutputLoad()
{
    float tempC = 0.0;
    for(std::map<std::string, gate*>::iterator i = gateMap.begin(); i != gateMap.end(); i++)
    {
        tempC = i->second->getLoadCap();
        std::string s = i->second->getName();
        int x = s.find("_output");
        if (i->second->getType() != "input" && i->second->getType() != "output" && i->second->getName() != "root" && x==std::string::npos)
        {

            tempC += wireMap[i->second->getOut()]->getNetCap();//add capacitance of output wire
            for (int j = 0; j < wireMap[i->second->getOut()]->getWDestinations().size(); j++)
            {
                CellClass* temp; CellPin* tempPin; std::string s = wireMap[i->second->getOut()]->getWDestinations()[j]->getType();
                if (s!="output")
                {
                    QString qs = QString::fromStdString(s);
                    temp = findClass(qs);
                    if (wireMap[i->second->getOut()]->getWDestinations()[j]->getIn1() == i->second->getOut())
                        tempPin = findPin(0,&temp->InputPins);
                    else if (wireMap[i->second->getOut()]->getWDestinations()[j]->getIn2() == i->second->getOut())
                        tempPin = findPin(1,&temp->InputPins);
                    else std::cout<<"error\n";
                    tempC+= tempPin->maxCap;//add capacitance of pin std::
                 }
            }
        }
        if (x!=std::string::npos) tempC =  prev(i)->second->getLoadCap();
        i->second->setLoadCap(tempC);
        //std::cout << i->second->getName() << "\t" << i->second->getLoadCap() << "\n";

    }
}
void Circuit::trvGate(std::vector<gate *> sVec)
{
    float max_slew = 0, max_delay = 0;
    float temp1, temp2;
    std::vector<Edge*>in, out;
    for(int i = 0; i < sVec.size(); i++)
    {
        if(sVec[i]->getName() == "root")
        {

            sVec[i]->setDelay(0); //std::cout<<"root\n";
            for(int j = 0; j < vEdges.size(); j++)
                if(vEdges[j]->wireName == "root")
                    vEdges[j]->setSlewRate(0);

        }
        else if(sVec[i]->getType() == "input")
        {
            //std::cout<<"input\n";
            for(int j = 0; j < vEdges.size(); j++)
                if(vEdges[j]->wireName == sVec[i]->getName())
                    vEdges[j]->setSlewRate(0.03);
        }
        else if (sVec[i]->getFlipIn())
        {

            //std::cout<<"input FF\n";
            in.clear(); out.clear();
            for(int j = 0; j < vEdges.size(); j++)
            {
                Edge* e = vEdges[j];
                if(sVec[i]->getName() == vEdges[j]->getSource()->getName())
                    out.push_back(e);
            }
            CellClass* temp; CellPin* tempPin; std::string s = sVec[i]->getType();
            //std::cout<<s<<"\n";
            QString qs = QString::fromStdString(s);
            temp = findClass(qs);
            tempPin = findPin(0,&temp->OutputPins);
            for(int h = 0; h<tempPin->DelayTable.size(); h++)max_delay = std::max(max_delay, (float)tempPin->DelayTable[h].getMax(sVec[i]->getFInSlew(), sVec[i]->getLoadCap()));
            for(int h = 0; h<tempPin->SlewTable.size(); h++)max_slew = max(max_slew, (float)tempPin->SlewTable[h].getMax(sVec[i]->getFInSlew(), sVec[i]->getLoadCap()));
            sVec[i]->setDelay(max_delay);
            for(int y = 0; y < out.size(); y++)
                out[y]->setSlewRate(max_slew);
        }
        else if (!sVec[i]->getFlipIn() && sVec[i]->getIsFlip())
        {
            //std::cout<<"Output FF\n";
            sVec[i]->setDelay(0);
        }
        else if (sVec[i]->getType() != "output")
        {
            //std::cout<<"Normal Gate\n";
            in.clear(); out.clear();
            for(int j = 0; j < vEdges.size(); j++)
            {
                Edge* e = vEdges[j];

                if(sVec[i]->getName() == vEdges[j]->getDestination()->getName())
                    in.push_back(e);
                else if(sVec[i]->getName() == vEdges[j]->getSource()->getName())
                    out.push_back(e);
            }
            for(int z = 0; z < in.size(); z++)
            {
                CellClass* temp; CellPin* tempPin; std::string s = in[z]->getDestination()->getType();
                //std::cout<<s<<"\n";
                QString qs = QString::fromStdString(s);
                temp = findClass(qs);
                tempPin = findPin(0,&temp->OutputPins);
                //std::cout<<tempPin->DelayTable.size()<<"\n";
                for(int h = 0; h<tempPin->DelayTable.size(); h++)max_delay = std::max(max_delay, (float)tempPin->DelayTable[h].getMax(in[z]->getSlewRate(), in[z]->getDestination()->getLoadCap()));
                for(int h = 0; h<tempPin->SlewTable.size(); h++) max_slew = max(max_slew, (float)tempPin->SlewTable[h].getMax(in[z]->getSlewRate(), in[z]->getDestination()->getLoadCap()));
            }
            sVec[i]->setDelay(max_delay);
            for(int y = 0; y < out.size(); y++)
                out[y]->setSlewRate(max_slew);
        }
        //std::cout<<i+1<<"\t"<<sVec[i]->getName()<<"\t"<<sVec[i]->getDelay()<<"\n";
    }
}


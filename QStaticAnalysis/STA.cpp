
//#include <iostream>
#include <algorithm>
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QMap>
using namespace std;
enum DirEnum{DirUnknown = 0, DirInput = 1, DirOutput = 2};
enum CellType{CellGate = 0, CellDFF = 1, CellLatch = 2, CellInputPort = 3, CellOutputPort = 4};

class LookUpTable{
public:
    int rows, cols;
    QMap<double, int> rIndex;
    QMap<double, int> cIndex;
    double table[10][10];
    LookUpTable& operator=(const LookUpTable& lut){
        rows = lut.rows; cols = lut.cols;
        rIndex = lut.rIndex; cIndex = lut.cIndex;
        for(int i = 0; i < 10; i++) for(int j = 0; j < 10; j++) table[i][j] = lut.table[i][j];
        return *this;
    }
    void clear(){
        rows = cols = 0;
        rIndex.clear(); cIndex.clear();
        for(int i = 0; i < rows; i++) for(int j = 0; j < 10; j++) table[i][j] = 0;
    }
    double GetValue(double x, double y){
        QMap<double, int>::iterator rit1, rit2;
        rit1 = rIndex.lowerBound(x); rit2 = rIndex.upperBound(x);
        QMap<double, int>::iterator cit1, cit2;
        cit1 = cIndex.lowerBound(y); cit2 = cIndex.upperBound(y);

        if(rit1 == rit2){
            if(rit1 == rIndex.end()) rit1--;
            else if(rit1 == rIndex.begin()) rit2++;
            else rit1--;
        }
        if(rit2 == rIndex.end()) {rit2--; rit2--;}
        if(cit1 == cit2){
            if(cit1 == cIndex.end()) cit1--;
            else if(cit1 == cIndex.begin()) cit2++;
            else cit1--;
        }
        if(cit2 == cIndex.end()) {cit2--; cit2--;}

        double fact = 1.0 / ((rit2.key() - rit1.key()) * (cit2.key() - cit1.key()));
        double f11 = table[rit1.value()][cit1.value()] * (rit2.key() - x) * (cit2.key() - y);
        double f21 = table[rit2.value()][cit1.value()] * (x - rit1.key()) * (cit2.key() - y);
        double f12 = table[rit1.value()][cit2.value()] * (rit2.key() - x) * (y - cit1.key());
        double f22 = table[rit2.value()][cit2.value()] * (x - rit1.key()) * (y - cit1.key());

        return fact * (f11 + f21 + f12 + f22);
    }
};
QDebug& operator<<(QDebug& o, const LookUpTable& lut){
    o << "\nLUT(#Rows = " << lut.rows << " #Cols = " << lut.cols << '\n';
    o << "Row index = "; o << lut.rIndex;
    o << "\nCol index = "; o << lut.cIndex;
    o << "\nData:\n";
    for(int i = 0; i < lut.rows; i++, o << "\n") for(int j = 0; j < lut.cols; j++) o << lut.table[i][j] << ", ";
    o<<")LUT;\n";
    return o;
}
class TimingTable{
public:
    QString RelatedTo;
    QList<LookUpTable> tables;
    void clear(int n){
        RelatedTo = "";
        tables.clear();
        for(int i = 0; i < n; i++){
            tables.push_back(LookUpTable());
            tables.back().clear();
        }
    }
    double getMin(double x, double y){
        double mn = 1e9;
        for(LookUpTable& lut : tables) mn = min(mn, lut.GetValue(x, y));
        return mn;
    }
    double getMax(double x, double y){
        double mx = -1e9;
        for(LookUpTable& lut : tables) mx = max(mx, lut.GetValue(x, y));
        return mx;
    }
};
QDebug& operator<<(QDebug& o, const TimingTable& tt){
    o << "Timing Table Related to: " << tt.RelatedTo << "\t" << tt.tables.size();
    o << "\n";
    o << tt.tables;
    return o;
}
class CellPin{
public:
    QString Name;
    DirEnum Dir;
    double maxCap, minCap;
    QList<TimingTable> DelayTable;
    QList<TimingTable> SlewTable;
    void clear(){
        Name = "";
        Dir = DirUnknown;
        maxCap = minCap = -1;
        DelayTable.clear();
        SlewTable.clear();
    }
};
QDebug& operator<<(QDebug& o, const CellPin& p){
    o << p.Name << ", " << p.Dir << ", MaxC: " << p.maxCap << ", MinC: " << p.minCap << "\n";
    if(p.Dir == DirOutput) o << "Delay:\n" << p.DelayTable;
    if(p.Dir == DirOutput) o << "\nSlew:\n" << p.SlewTable;
    return o;
}
class CellClass{
public:
    CellType Type;
    QString Name;
    QList<CellPin> InputPins;
    QList<CellPin> OutputPins;
    TimingTable SetupTiming;
    TimingTable HoldTiming;
    void clear(){
        Name = "";
        Type = CellGate;
        InputPins.clear();
        OutputPins.clear();
        SetupTiming.clear(2); HoldTiming.clear(2);
    }
};
QDebug& operator <<(QDebug& o, const CellClass& c){
    o << "\n********************\n";
    o << c.Name << ' ' << c.Type << "\nInput Pins: " << c.InputPins << "\nOutput Pins: " << c.OutputPins;
    if(c.Type == CellDFF || c.Type == CellLatch) o << "\nSetup: " << c.SetupTiming << "\nHold: " << c.HoldTiming;
    o << "\n********************\n";
    return o;
}

class Pin{
public:
    QString Name;
    DirEnum Dir;
    QString WireName;
    CellPin* MyCellPin;
    double MinCapLoad, MaxCapLoad;	//For input CapLoad exerted on previous stage, for output Capacitive load exterted on this pin
    double MinDelay, MaxDelay;	//For input Accumlative Delay of previous stages, for output accumlative delay including this pin
    double MinSlew, MaxSlew;	//For input input slew rate, for output output slew rate.
    void SetAttr(QList<QString> lst){
        MinDelay	= lst[2].toDouble();
        MaxDelay	= lst[3].toDouble();
        MinSlew 	= lst[4].toDouble();
        MaxSlew 	= lst[5].toDouble();
        MinCapLoad 	= lst[6].toDouble();
        MaxCapLoad 	= lst[7].toDouble();
    }
};
QDebug& operator<<(QDebug& o, const Pin& p){
    if(p.Name == "Port")  o << p.Name << ";" << p.Dir << ";" << p.WireName << ";";
    else o << p.Name << ";" << p.Dir << ";" << p.WireName << ";" << p.MyCellPin->Name;
    o << "Cap(" << p.MinCapLoad << ", " << p.MaxCapLoad << ")\n";
    o << "Delay(" << p.MinDelay << ", " << p.MaxDelay << ")\n";
    o << "Slew(" << p.MinSlew << ", " << p.MaxSlew << ")\n";
    return o;
}
class Gate{
public:
    QString Name;
    CellType Type;
    CellClass* MyClass;	//Index
    QList<Pin> InputPins;
    QList<Pin> OutputPins;
};
QDebug& operator<<(QDebug& o, const Gate& g){
    if(g.Type == CellInputPort || g.Type == CellOutputPort) o << "Gate " << g.Name << "(" << g.Type << ") ";
    else o << "Gate " << g.Name << "(" << g.Type << " " << g.MyClass->Name << ") ";
    o << "Pins\n";
    o << "InputPins: " << g.InputPins << "\n";
    o << "OutputPins: " << g.OutputPins << "\n";
    return o;
}
class PinIndex{
public:
    int gIDX;
    int pIDX;
    PinIndex(int x = -1, int y = -1){
        gIDX = x;
        pIDX = y;
    }
    bool operator<(const PinIndex& pid) const{
        return gIDX < pid.gIDX || (gIDX == pid.gIDX && pIDX < pid.pIDX);
    }
    bool operator==(const PinIndex& pid) const{
        return gIDX == pid.gIDX && pIDX == pid.pIDX;
    }
};
uint qHash(const PinIndex &p){
    return p.gIDX + p.pIDX;
}
QDebug& operator<<(QDebug& o, const PinIndex& pid){
    o << "PinIndex (" << pid.gIDX << ", " << pid.pIDX << ")";
    return o;
}
class Wire{
public:
    QString Name;
    PinIndex InputGatePin;
    QList<PinIndex> OutputGatePinList;
    double MinWireCapacitance, MaxWireCapacitance;
};
QDebug& operator<<(QDebug& o, const Wire& w){
    o << w.Name << "\t Capacitance: (" << w.MinWireCapacitance << ", " << w.MaxWireCapacitance << ")" << "\n InputGatePin: " << w.InputGatePin << "\n OutputGatePinList: "<< w.OutputGatePinList << "\n";
    return o;
}


double ClockPeriod;
QString ModuleName;
QList<CellClass> Class;
QList<Gate> DAG;
QList<bool> vis;
QMap<QString, Wire> Wires;

LookUpTable ParseLUT(QString str){
    QRegExp regex("", Qt::CaseSensitive);
    QList<QString> tlst;
    LookUpTable ret; ret.clear();
    regex.setMinimal(true);
    regex.setPattern("\\s*index_1\\(\"(.*)\"\\);\\s*index_2\\(\"(.*)\"\\);\\s*values\\(\"(.*)\"\\);\\s*");
    regex.indexIn(str);
    //Rows
    for(QString v : regex.capturedTexts()[1].split(", ")){
        ret.rIndex[v.toDouble()] = ret.rows++;
    }
    //Cols
    for(QString v : regex.capturedTexts()[2].split(", "))
        ret.cIndex[v.toDouble()] = ret.cols++;
    //Data
    tlst = regex.capturedTexts()[3].split(", ");
    for(int i = 0, k = 0; i < ret.rows; i++)
        for(int j = 0; j < ret.cols; j++)
            ret.table[i][j] = tlst[k++].toDouble();
    return ret;
}
void ParseLiberty(const char* fpath){
    //ParseNetList("mux_NetList.v");
    QFile outFile("Out"); outFile.open(QFile::WriteOnly); QTextStream fout(&outFile);

    QFile inFile(fpath); inFile.open(QFile::ReadOnly); QTextStream fin(&inFile);
    QRegExp regex("", Qt::CaseSensitive);
    QList<QString> cap;
    QString data = fin.readAll();

    //Simplification
    {
        //Remove Comments
        regex.setMinimal(true);
        regex.setPattern("/\\*.*\\*/"); data = data.remove(regex);

        //Handeling Escape Sequences
        regex.setMinimal(false);
        regex.setPattern("\",\\s*\\\\\\s*\""); data = data.replace(regex, ", ");
        regex.setPattern("\\s*\\\\\\s*"); data = data.replace(regex, " ");

        //Simplify Information
        regex.setMinimal(true);
        regex.setPattern("ff (.*) "); data = data.replace(regex, "_IsDFF_ ");
        regex.setPattern("latch (.*) "); data = data.replace(regex, "_IsLatch_ ");
        regex.setPattern("timing_type\\s*:\\s*(hold_falling|hold_rising)\\s*;"); data = data.replace(regex, "_HoldConstrains_");
        regex.setPattern("timing_type\\s*:\\s*(setup_falling|setup_rising)\\s*;"); data = data.replace(regex, "_SetupConstrains_");

        //Removing extra info that are hardcoded (For simpler parsing).
        regex.setMinimal(true);
        regex.setPattern("\\s*\\S+_template\\(\\S*_template_\\S*\\) \\{.*\\}"); data = data.remove(regex);
        regex.setPattern("\\(\\S*_template_\\S*\\)"); data = data.remove(regex);
        regex.setPattern("\\s*internal_power\\(\\) \\{\\s*(related_pin : \\S*;)?\\s*rise_power \\{.*;.*;\\s*\\}\\s*fall_power \\{.*;.*;\\s*\\}\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\s*internal_power\\(\\) \\{\\s*(related_pin : \\S*;)?\\s*fall_power \\{.*;.*;\\s*\\}\\s*rise_power \\{.*;.*;\\s*\\}\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\s*internal_power\\(\\) \\{\\s*(related_pin : \\S*;)?\\s*rise_power \\{.*;.*;.*;\\s*\\}\\s*fall_power \\{.*;.*;.*;\\s*\\}\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\s*internal_power\\(\\) \\{\\s*(related_pin : \\S*;)?\\s*fall_power \\{.*;.*;.*;\\s*\\}\\s*rise_power \\{.*;.*;.*;\\s*\\}\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\s*internal_power\\(\\) \\{\\s*(related_pin : \\S*;)?\\s*power \\{.*;.*;.*;\\s*\\}\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\s+pad_cell :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+is_pad :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+drive_current :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+three_state :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+function :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+area.*;"); data = data.remove(regex);
        regex.setPattern("\\s+cell_leakage_power.*;"); data = data.remove(regex);
        regex.setPattern("\\s+timing_sense.*;"); data = data.remove(regex);
        regex.setPattern("\\s+cell_footprint.*;"); data = data.remove(regex);
        regex.setPattern("\\s+min_pulse_width_high :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+min_pulse_width_low :.*;"); data = data.remove(regex);
        regex.setPattern("\\s+capacitance : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+next_state : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+clear : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+preset : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+clear_preset_var1 : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+data_in : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+timing_type : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+when : .*;"); data = data.remove(regex);
        regex.setPattern("\\s+sdf_cond : .*;"); data = data.remove(regex);
        regex.setPattern("\\{\\s*enable :.*;\\s*\\}"); data = data.remove(regex);
        regex.setPattern("\\{\\s*clocked_on :.*;\\s*\\}"); data = data.remove(regex);

        regex.setMinimal(false);
        regex.setPattern("\\s*cell \\(PADFC\\).*$"); data = data.remove(regex);
        regex.setPattern("^.*typical;\\s*"); data = data.remove(regex);
        regex.setPattern("\\s*fall_capacitance : 0;"); data = data.remove(regex);
        regex.setPattern("\\s*rise_capacitance : 0;"); data = data.remove(regex);

        //WhiteSpace Cleanup
        regex.setMinimal(false);
        regex.setPattern("\n\\s*\n"); data = data.replace(regex, "\n");
        regex.setPattern("\\s*\\(\\s*"); data = data.replace(regex, "(");
        regex.setPattern("\\s*\\{"); data = data.replace(regex, "{");
        regex.setPattern("\\s*:\\s*"); data = data.replace(regex, ":");

        fout << data;
    }

    //Extracting data
    regex.setMinimal(true);
    data = data.trimmed(); //data = data.simplified();
    CellClass ct; CellPin pt; TimingTable ttt;
    ttt.clear(2);
    //Cells
    for(QString c : data.split("cell(")) if(c != ""){ ct.clear(); ttt.clear(2);
        //Cell Names
        if(regex.setPattern("^(.*)\\)\\{"), regex.indexIn(c) != -1){
            cap = regex.capturedTexts();
            ct.Name = cap[1];
        }
        else{
            qDebug() << c.left(10).simplified() << "\t Unable to parse!";
            continue;
        }

        if(ct.Name == "PADINOUT") continue;

        //Type
        if(regex.setPattern("\\s+_IsDFF_\\s+"), regex.indexIn(c) != -1) ct.Type = CellDFF;
        else if(regex.setPattern("\\s+_IsLatch_\\s+"), regex.indexIn(c) != -1) ct.Type = CellLatch;
        else ct.Type = CellGate;

        //Setup and hold time
        if(ct.Type == CellDFF || ct.Type == CellLatch){
            regex.setMinimal(true);
            regex.setPattern("\\s+timing\\(\\)\\{.*_HoldConstrains_\\s*rise_constraint\\{(.*)\\}\\s*fall_constraint\\{(.*)\\}\\s*\\}\\s+timing\\(\\)\\{.*_SetupConstrains_\\s*rise_constraint\\{(.*)\\}\\s*fall_constraint\\{(.*)\\}\\s*\\}");
            regex.indexIn(c); cap = regex.capturedTexts(); c.remove(regex);
            ct.HoldTiming.RelatedTo = ct.SetupTiming.RelatedTo = "CLK";
            ct.HoldTiming.tables[0] = ParseLUT(cap[1]);	//Rising
            ct.HoldTiming.tables[1] = ParseLUT(cap[2]);	//Falling
            ct.SetupTiming.tables[0] = ParseLUT(cap[3]);
            ct.SetupTiming.tables[1] = ParseLUT(cap[4]);
        }

        //Pins
        regex.setMinimal(true);
        for(QString p : c.split("pin(")){ pt.clear();
            regex.setPattern("\\s*direction:(input|output);\\s*"); regex.indexIn(p);
            cap = regex.capturedTexts();
            if(cap[0] == "") continue;	//Ignoring first one.

            //Pin Direction
            pt.Dir = DirEnum(DirInput * (cap[1] == "input") + DirOutput * (cap[1] == "output"));

            //Pin Name
            regex.setPattern("^\\s*(.*)\\)\\{\\s*"); regex.indexIn(p);
            pt.Name = regex.capturedTexts()[1];

            //Pin Capacitance
            if(pt.Dir == DirOutput){
                regex.setPattern("\\s*max_capacitance:(.*);"); regex.indexIn(p);
                pt.minCap = 0; pt.maxCap = regex.capturedTexts()[1].toDouble();
            }
            else{
                regex.setPattern("\\s*rise_capacitance:(.*);\\s*fall_capacitance:(.*);\\s*"); regex.indexIn(p);
                pt.minCap = min(regex.capturedTexts()[1].toDouble(), regex.capturedTexts()[2].toDouble());
                pt.maxCap = max(regex.capturedTexts()[1].toDouble(), regex.capturedTexts()[2].toDouble());
            }

            //Pin Delays and Slew models
            if(pt.Dir == DirOutput){
                for(QString tim : p.split("timing()")){
                    regex.setPattern("\\s*related_pin:\"(.*)\";"); regex.indexIn(tim);
                    if(regex.capturedTexts()[0] == "") continue;
                    if(ttt.RelatedTo == "EN"&&ct.Name == "TBUF") continue;	//Hotfix for the 2 timings related to pin "EN" in TBuffs
                    ttt.RelatedTo = regex.capturedTexts()[1];
                    regex.setPattern("cell_rise\\{(.*)\\}"); regex.indexIn(tim);
                    ttt.tables[0] = ParseLUT(regex.capturedTexts()[1]);
                    regex.setPattern("cell_fall\\{(.*)\\}"); regex.indexIn(tim);
                    ttt.tables[1] = ParseLUT(regex.capturedTexts()[1]);
                    pt.DelayTable.push_back(ttt);

                    regex.setPattern("rise_transition\\{(.*)\\}"); regex.indexIn(tim);
                    ttt.tables[0] = ParseLUT(regex.capturedTexts()[1]);
                    regex.setPattern("fall_transition\\{(.*)\\}"); regex.indexIn(tim);
                    ttt.tables[1] = ParseLUT(regex.capturedTexts()[1]);
                    pt.SlewTable.push_back(ttt);
                }
            }

            //Pushing Pin
            if(pt.Dir == DirInput) ct.InputPins.push_back(pt);
            else if(pt.Dir == DirOutput) ct.OutputPins.push_back(pt);
        }
        //qDebug() << ct;

        //Add To List of Cell Classes
        Class.push_back(ct);
    }
}

CellClass* findClass(QString ClassName)
{
    for(int i = 0; i < Class.size(); i++)
        if(Class[i].Name == ClassName)
            return &Class[i];
    return 0;
}
Pin* findPin(QString PinName, QList<Pin>* PinList)
{
    for(int i = 0; i < PinList->size(); i++)
        if((*PinList)[i].Name == PinName)
            return &(*PinList)[i];
    return 0;
}

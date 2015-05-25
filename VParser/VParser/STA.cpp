//#include <iostream>
//#include <string>
//#include <QRegExp>
//#include <File>
//#include <TextStream>
//#include <List>
//#include <Map>
//using namespace std;
//enum DirEnum{DirUnknown = 0, DirInput = 1, DirOutput = 2};
//enum CellType{CellGate = 0, CellDFF = 1, CellLatch = 2, CellInputPort = 3, CellOutputPort = 4};
//
//class Pin{
//public:
//	string Name;
//	DirEnum Dir;
//	string WireList;
//};
//
//class Gate{
//public:
//	string Name;
//	string Type;
//	List<Pin> Pins;
//};
//
//class PinIndex{
//public:
//	int gIDX;
//	int pIDX;
//	PinIndex(int x, int y){
//		gIDX = x;
//		pIDX = y;
//	}
//	bool operator<(const PinIndex& pid) const{
//		return gIDX < pid.gIDX || (gIDX == pid.gIDX && pIDX < pid.pIDX);
//	}
//	bool operator==(const PinIndex& pid) const{
//		return gIDX == pid.gIDX && pIDX == pid.pIDX;
//	}
//};
//uint Hash(const PinIndex &p){
//	return p.gIDX + p.pIDX;
//}
//class Wire{
//public:
//	string Name;
//	List<PinIndex> GateList;
//};
//
//class LookUpTable{
//public:
//	int rows, cols;
//	Map<double, int> rIndex;
//	Map<double, double> cIndex;
//	double table[10][10];
//	LookUpTable& operator=(const LookUpTable& lut){
//		rows = lut.rows; cols = lut.cols;
//		rIndex = lut.rIndex; cIndex = lut.cIndex;
//		for(int i = 0; i < 10; i++) for(int j = 0; j < 10; j++) table[i][j] = lut.table[i][j];
//		return *this;
//	}
//	void clear(){
//		rows = cols = 0;
//		rIndex.clear(); cIndex.clear();
//		for(int i = 0; i < rows; i++) for(int j = 0; j < 10; j++) table[i][j] = 0;
//	}
//};
//
//class CellPin{
//public:
//	string Name;
//};
//
//class CellClass{
//public:
//	CellType Type;
//	string Name;
//	List<int> Sizes;
//	List<List<CellPin> > InputPins;
//	List<List<CellPin> > OutputPins;
//	List<LookUpTable> SetupTiming;
//	List<LookUpTable> HoldTiming;
//	void clear(){
//		Type = CellGate;
//		Sizes.clear(); Sizes.push_back(0);
//		InputPins.clear(); InputPins.push_back(List<CellPin>());
//		OutputPins.clear(); OutputPins.push_back(List<CellPin>());
//		SetupTiming.clear(); SetupTiming.push_back(LookUpTable()); SetupTiming.push_back(LookUpTable());
//		HoldTiming.clear(); HoldTiming.push_back(LookUpTable()); HoldTiming.push_back(LookUpTable());
//		SetupTiming[0].clear(); SetupTiming[1].clear();
//		HoldTiming[0].clear(); HoldTiming[1].clear();	
//	}	
//};
//
//
//string ModuleName;
//List<CellClass> Class;
//List<Gate> DAG;
//Map<string, Wire> Wires;
//
//LookUpTable ParseLUT(string str){
//	RegExp regex("", Qt::CaseSensitive);
//	List<string> tlst;
//	LookUpTable ret; ret.clear();
//	regex.setMinimal(true);
//	regex.setPattern("\\s*index_1\\(\"(.*)\"\\);\\s*index_2\\(\"(.*)\"\\);\\s*values\\(\"(.*)\"\\);\\s*");
//	regex.indexIn(str);
//	//Rows
//	for(string v : regex.capturedTexts()[1].split(", ")){
//		ret.rIndex[v.toDouble()] = ret.rows++;
//	}
//	//Cols
//	for(string v : regex.capturedTexts()[2].split(", "))
//		ret.cIndex[v.toDouble()] = ret.cols++;
//	//Data
//	tlst = regex.capturedTexts()[3].split(", ");
//	for(int i = 0, k = 0; i < ret.rows; i++)
//		for(int j = 0; j < ret.cols; j++)
//			ret.table[i][j] = tlst[k++].toDouble();
//	return ret;
//}
//void ParseLiberty(const char* fpath){
//	//ParseNetList("mux_NetList.v");
//	File outFile("Out"); outFile.open(File::WriteOnly); TextStream fout(&outFile);
//
//	File inFile(fpath); inFile.open(File::ReadOnly); TextStream fin(&inFile);
//	RegExp regex("", Qt::CaseSensitive);
//	List<string> cap;
//	string data = fin.readAll();
//	
//	//Simplification
//	{
//		//Remove Comments
//		regex.setMinimal(true);
//		regex.setPattern("/\\*.*\\*/"); data = data.remove(regex);
//
//		//Handeling Escape Sequences
//		regex.setMinimal(false);
//		regex.setPattern("\",\\s*\\\\\\s*\""); data = data.replace(regex, ", ");
//		regex.setPattern("\\s*\\\\\\s*"); data = data.replace(regex, " ");
//		
//		//Simplify Information
//		regex.setMinimal(true);
//		regex.setPattern("ff (.*) "); data = data.replace(regex, "_IsDFF_ ");
//		regex.setPattern("latch (.*) "); data = data.replace(regex, "_IsLatch_ ");
//		regex.setPattern("timing_type\\s*:\\s*(hold_falling|hold_rising)\\s*;"); data = data.replace(regex, "_HoldConstrains_");
//		regex.setPattern("timing_type\\s*:\\s*(setup_falling|setup_rising)\\s*;"); data = data.replace(regex, "_SetupConstrains_");
//
//		//Removing extra info that are hardcoded (For simpler parsing).
//		regex.setMinimal(true);
//		regex.setPattern("\\s*\\S+_template\\(\\S*_template_\\S*\\) \\{.*\\}"); data = data.remove(regex);
//		regex.setPattern("\\(\\S*_template_\\S*\\)"); data = data.remove(regex);
//		regex.setPattern("internal_power\\(\\)\\s*\\{.*\\{.*\\}.*\\{.*\\}.*\\}"); data = data.remove(regex);
//		regex.setPattern("\\s+pad_cell :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+is_pad :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+drive_current :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+three_state :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+function :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+area.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+cell_leakage_power.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+timing_sense.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+cell_footprint.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+min_pulse_width_high :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+min_pulse_width_low :.*;"); data = data.remove(regex);
//		regex.setPattern("\\s+capacitance : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+next_state : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+clear : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+preset : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+clear_preset_var1 : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+data_in : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+timing_type : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+when : .*;"); data = data.remove(regex);
//		regex.setPattern("\\s+sdf_cond : .*;"); data = data.remove(regex);
//		regex.setPattern("\\{\\s*enable :.*;\\s*\\}"); data = data.remove(regex);
//		regex.setPattern("\\{\\s*clocked_on :.*;\\s*\\}"); data = data.remove(regex);
//
//		regex.setMinimal(false);
//		regex.setPattern("\\s*cell \\(PADFC\\).*$"); data = data.remove(regex);
//		regex.setPattern("^.*typical;\\s*"); data = data.remove(regex);
//		regex.setPattern("\\s*fall_capacitance : 0;"); data = data.remove(regex);
//		regex.setPattern("\\s*rise_capacitance : 0;"); data = data.remove(regex);
//
//		//WhiteSpace Cleanup
//		regex.setMinimal(false);
//		regex.setPattern("\n\\s*\n"); data = data.replace(regex, "\n");
//		regex.setPattern("\\s*\\(\\s*"); data = data.replace(regex, "(");
//		regex.setPattern("\\s*\\{"); data = data.replace(regex, "{");
//		regex.setPattern("\\s*:\\s*"); data = data.replace(regex, ":");
//
//		fout << data;
//	}
//
//	//Extracting data
//	regex.setMinimal(false);
//	data = data.trimmed(); //data = data.simplified();
//	List<string> lst = data.split("cell(");
//	List<string> tlst;
//	CellClass ct; CellPin pt;
//	ct.clear();
//
//	for(string c : lst) if(c != ""){
//		//Cell Names
//		if(regex.setPattern("^([A-WY-Z1-9]+)X?([1-9])\\)\\{"), regex.indexIn(c) != -1){
//			cap = regex.capturedTexts();
//			ct.Name = cap[1];
//			ct.Sizes[0] = cap[2].toInt();
//		}
//		else if(regex.setPattern("^([A-Z1-9]+)X([1-9])\\)\\{"), regex.indexIn(c) != -1){
//			cap = regex.capturedTexts();
//			ct.Name = cap[1];
//			ct.Sizes[0] = cap[2].toInt();
//		}
//		else if(regex.setPattern("^([A-Z1-9]+)\\)\\{"), regex.indexIn(c) != -1){
//			cap = regex.capturedTexts();
//			ct.Name = cap[1];
//			ct.Sizes[0] = 1;
//		}
//		else{
//			continue;
//		}
//
//		//Type
//		if(regex.setPattern("\\s+_IsDFF_\\s+"), regex.indexIn(c) != -1) ct.Type = CellDFF;
//		else if(regex.setPattern("\\s+_IsLatch_\\s+"), regex.indexIn(c) != -1) ct.Type = CellLatch;
//		else ct.Type = CellGate;
//
//		//Setup and hold time
//		if(ct.Type == CellDFF || ct.Type == CellLatch){
//			//Hold Timing
//			regex.setMinimal(true);
//			regex.setPattern("\\s+timing\\(\\)\\{.*_HoldConstrains_\\s*rise_constraint\\{(.*)\\}\\s*fall_constraint\\{(.*)\\}\\s*\\}\\s+timing\\(\\)\\{.*_SetupConstrains_\\s*rise_constraint\\{(.*)\\}\\s*fall_constraint\\{(.*)\\}\\s*\\}");
//			regex.indexIn(c); cap = regex.capturedTexts(); c.remove(regex);
//			ct.HoldTiming[0] = ParseLUT(cap[1]);	//Rising
//			ct.HoldTiming[1] = ParseLUT(cap[2]);	//Falling
//			ct.SetupTiming[0] = ParseLUT(cap[3]);
//			ct.SetupTiming[1] = ParseLUT(cap[4]);
//		}
//
//		//Add To List of Cell Classes
//		bool NewFlag = true;
//		for(CellClass& cc : Class) if(cc.Name == ct.Name){
//			cc.Sizes.push_back(ct.Sizes[0]);
//			cc.InputPins.push_back(ct.InputPins[0]);
//			cc.OutputPins.push_back(ct.OutputPins[0]);
//			NewFlag = false;
//		}
//		if(NewFlag) Class.push_back(ct);
//	}
//	
//}
//void ParseNetList(const char* fpath){
//	File inFile(fpath); inFile.open(File::ReadOnly); TextStream fin(&inFile);
//	RegExp regex("", Qt::CaseSensitive); regex.setMinimal(true);
//	List<string> cap;
//	string data = fin.readAll();
//
//	//Simplification
//	{
//		//Removing Comments and /$
//		regex.setPattern("//.*\n"); data = data.remove(regex);
//		regex.setPattern("\\(\\*.*\\*\\)\\n"); data = data.remove(regex);
//		regex.setPattern("/\\*.*\\*/"); data = data.remove(regex);
//		regex.setPattern("\\\\\\$"); data = data.remove(regex);
//	}
//
//	//Assuming only one module for now
//	data = data.trimmed(); data = data.simplified();
//	List<string> lines = data.split(";");
//
//	int i = 0;
//	Wire wt; Gate gt; Pin pt;
//	regex.setMinimal(false);
//	
//	regex.setPattern("module\\s+(\\S*)\\(.*\\)");	//Parsing Module Name
//	regex.indexIn(lines[i]);
//	cap = regex.capturedTexts();
//	ModuleName = cap[1];
//	i++;
//
//	//Parsing Wires
//	regex.setPattern("(wire|input|output)\\s+(?:\\[(\\d+)\\:(\\d+)\\])?\\s*(\\S+)");
//	while(regex.indexIn(lines[i]) != -1){
//		cap = regex.capturedTexts();
//		for(int s = cap[2].toInt(); s >= cap[3].toInt(); s--){
//			wt.Name = (cap[2] == "") ? cap[4] : (cap[4] + "[" + string::number(s) + "]");
//			Wires[wt.Name] = wt;
//			if(cap[1] == "input" || cap[1] == "output"){
//				pt.Name = "Port";
//				pt.Dir = (cap[1] == "input") ? DirOutput : DirInput;
//				pt.WireList = gt.Name = wt.Name;
//				gt.Type = (cap[1] == "input") ? "InputPort" : "OutputPort";
//				DAG.push_back(gt);
//				DAG.last().Pins.push_back(pt);
//				Wires[wt.Name].GateList.push_back(PinIndex(DAG.size() - 1, 0));
//			}
//		}
//		i++;
//	}
//	//Parsing Gates
//	regex.setPattern("(\\S+)\\s+(\\S+)\\s*\\((.*)\\)");
//	while(regex.indexIn(lines[i]) != -1){
//		cap = regex.capturedTexts();
//		RegExp Param("\\.(\\S+)\\((\\S+)\\)", Qt::CaseSensitive);
//		gt.Name = cap[2];
//		gt.Type = cap[1];
//		DAG.push_back(gt);
//		for (string p : cap[3].trimmed().split(RegExp("\\s*,\\s*"))){
//			Param.indexIn(p);
//			pt.Name = Param.capturedTexts()[1];
//			pt.Dir = DirUnknown;
//			pt.WireList = Param.capturedTexts()[2];
//			DAG.last().Pins.push_back(pt);
//			Wires[pt.WireList].GateList.push_back(PinIndex(DAG.size() - 1, DAG.last().Pins.size() - 1));
//		}
//		i++;	
//	}
//	//Parsing Assigns
//	regex.setPattern("assign(?:\\s+)(\\S+)(?:\\s*)=(?:\\s*)(\\S+)");
//	while(regex.indexIn(lines[i]) != -1){
//		cap = regex.capturedTexts();
//		Wires[cap[1]].GateList.append(Wires[cap[2]].GateList); Wires[cap[1]].GateList = Wires[cap[1]].GateList.toSet().toList();
//		Wires[cap[2]].GateList.append(Wires[cap[1]].GateList); Wires[cap[2]].GateList = Wires[cap[2]].GateList.toSet().toList();
//		i++;
//	}
//}
//int main(){
//	ParseLiberty("Liberty.lib");
//}
/************************************************
File: pass2.cpp
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description: 
This file is the pass 2 cpp file
************************************************/

#include "pass2.h"
#include "pass1.h"

map<string, int> SYMTAB;

ifstream IF;
ofstream LF;

int baseAddress = 0;

string str_line, str_read, objectCode, curr_record, MR, base, label, operand, opcode, comment, operand1, operand2;;
bool nobase;
int TR_len, block, address, startAddress, lineNumber;
bool isComment;

//This function is called by main.cpp and it conducts pass 2. First opens intermidiate file and symbol table to create object code.
void pass2(string filename){
    size_t dot = filename.find_last_of('.');            //remove extention names

    if(dot == string::npos){
        base = filename;
    }else{
        base = filename.substr(0, dot);
    }

    loadSYMTAB(filename);

    IF.open(filename+"_intermediate_file");     //opens an intermediate file for current input file
    if(!IF){ return; }

    LF.open(base+".l");     //opens a listing file for current input file
    if(!LF){ return; }


    objectCode = "";
    TR_len = 0;
    curr_record = "";
    MR = "";
    nobase = true;
    block = 0;

    write(LF, ".  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER", true);

    int listingline = 1;
    while(read_IF(IF, isComment, lineNumber, address, block, label, opcode, operand, comment)){     //reads one line at a time of intermidiate file
                                                                                                    //stops when reaches end of file and no more lines left
        string obj = "";

        if (isComment) { //for comments, only copy comment down, no object code
            writeL(LF, -1, -1, "", "", "", "", comment, true);
            continue;
        }

        if (opcode == "START" || opcode == "END") {
            obj = "";
        } else if (opcode == "BASE") {
            IS_BASE(operand);
            obj = "";
        } else if (opcode == "NOBASE") {
            nobase = true;
            obj = "";
        } else if (opcode == "RESW" || opcode == "RESB") {
            obj = "";
        } else if (opcode == "WORD") {
            obj = IS_WORD(operand);
        } else if (opcode == "BYTE") {
            obj = IS_BYTE(operand);
        } else if (label == "*") {
            obj = IS_Literal(opcode);
        } else {
            obj = IS_Instruction(opcode, operand, address);
        }

        writeL(LF, lineNumber, address, label, opcode, operand, obj, comment, false);   //writes listing file based on opcodes given
        listingline++;
    }

    IF.close();
    LF.close();
};

//int to hex conversion
string toHex(int value, int width) {
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << value; //sets leading zeros
    return ss.str();
}

string read_section(const string& section, int& index){                 //reads a section until parsed by white space
    string str_read = "";
    
    while(index < section.length() && isspace(section[index])){           //increments until it finds non-white space
        index++;
    }

    while(index < section.length() && !isspace(section[index])){        //adds character to str_read until it hits a white space
        str_read +=section[index];
        index++;
    }
    return str_read;
}

//This function writes text to output files. 
void write(ofstream& filename, string text, bool nextLine){             //nextLine determines if next text begins on a new line.     
    if(nextLine){
        filename<<text<<endl;       
    }else{
        filename<<text;
    }
}

//reads intermidiate file
bool read_IF(ifstream& filename, bool& isComment, int& lineNumber, int& address, int& block, string& label, string& opcode, string& operand, string& comment){
    string line = "";
    int index = 0;
    if(!getline(filename, line)){
        return false;
    }
    isComment = false;
    comment = "";
    label = "";
    opcode = "";
    operand = "";

    while (index < line.length() && isspace(line[index])) {
        index++;
    }

    //detectes if line is a comment, 
    if (index < line.length() && line[index] == '.') {
        isComment = true;
        comment = line.substr(index);
        return true;
    }

    string addr = read_section(line, index);    //first section is address
    if (addr.empty()) return false;

    address = stoul(addr, nullptr, 16);

    label = read_section(line, index);      //next section is label
    if (label == "-") label = "";

    opcode = read_section(line, index);     // next sections are opcode and operand
    operand = read_section(line, index);

    
    
    if(index < line.length()){
        comment = line.substr(index);
    }
    return true;

}

//helper function used to read files
void read_non_space(string line, int& index, bool& status, string& data, bool toEnd){
    data = "";
    status = true;
    if(toEnd){
        data = line.substr(index);
        if(data == ""){
            status = false;
        }
        return;
    }
     while (index < line.length() && line[index] != ' ' && line[index] != '\t') {
        data += line[index];
        index++;
    }

    if (data == "") {
        status = false;
    }
}

//helper function used to read files
string read(string data, int& index){
    string x = "";
    while(index<data.length()&&isspace(data[index])){
        index++;
    }
    while (index < data.length() && !isspace(data[index])) {
        x += data[index];
        index++;
    }
    return x;
}

//helper function used to read files
void readOperand(string line, int& index, bool& status, string& data){
    data = "";
    status = true;

    while (index < line.length() && isspace(line[index])) {
        index++;
    }

    while (index < line.length() && !isspace(line[index])) {
        data += line[index];
        index++;
    }

    if (data == "") {
        status = false;
    }
}

void loadSYMTAB(string filename) {
    size_t dot = filename.find_last_of('.');
    if (dot == string::npos) {
        base = filename;
    } else {
        base = filename.substr(0, dot);
    }

    ifstream symFile(base + ".st");
    if (!symFile) {
        cout << "Unable to open SYMTAB file\n";
        return;
    }

    string line;
    bool firstDataLine = true;

    while (getline(symFile, line)) {
        if (line.empty()) continue;
        if (line.find("Csect") != string::npos) continue;
        if (line.find("----") != string::npos) continue;

        istringstream iss(line);
        vector<string> fields;
        string temp;
        while (iss >> temp) {
            fields.push_back(temp);
        }

        if (fields.empty()) continue;

        if (firstDataLine) {
            firstDataLine = false;
            continue;
        }

        if (fields.size() >= 2) {
            string symbol = fields[0];
            string value = fields[1];
            SYMTAB[symbol] = stoi(value, nullptr, 16);
        }
    }
}

//write listing file
void writeL(ofstream& out, int lineNumber, int address, const string& label, const string& opcode, const string& operand, const string& objectCode, const string& comment, bool isCommentLine){
     if (isCommentLine) {
        out << "            " << comment << '\n';
        return;
    }

    out << setw(4) << setfill(' ') << right << lineNumber << "  ";
    out << setw(6) << setfill('0') << uppercase << hex << address << "  ";
    out << left << setfill(' ') << setw(10) << label;
    out << setw(10) << opcode;
    out << setw(18) << operand;
    out << "  " << objectCode;

    if (!comment.empty()) {
        out << "  " << comment;
    }
    out << '\n';

    out << dec;

}

string IS_WORD(const string& operand) {
    int value = stoi(operand);
    return toHex(value & 0xFFFFFF, 6);
}

string IS_BYTE(const string& operand) {
    if (operand.size() < 3) return "";

    if (operand[0] == 'C' && operand[1] == '\'' && operand.back() == '\'') {
        string chars = operand.substr(2, operand.size() - 3);
        string result = "";
        for (char c : chars) {
            result += toHex((unsigned char)c, 2);
        }
        return result;
    }

    if (operand[0] == 'X' && operand[1] == '\'' && operand.back() == '\'') {
        return operand.substr(2, operand.size() - 3);
    }

    return "";
}

string IS_Literal(const string& literal) {
    if (literal.size() < 4 || literal[0] != '=') return "";

    string body = literal.substr(1); // remove '='
    return IS_BYTE(body);
}

void IS_BASE(const string& operand) {
    if (SYMTAB.find(operand) != SYMTAB.end()) {
        baseAddress = SYMTAB[operand];
        nobase = false;
    } else {
        nobase = true;
    }
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c)) && c != '-') return false;
    }
    return true;
}

string IS_Instruction(const string& opcode, const string& operand, int address) {
    string op = opcode;

    if (!op.empty() && op[0] == '+') {
        return Format3AND4(op, operand, address);
    }

    if (isFormat1(op)) {
        if (OPTAB.find(op) == OPTAB.end()) return "";
        return toHex(OPTAB[op], 2);
    }

    if (isFormat2(op)) {
        return Format2(op, operand);
    }

    if (isInstruction(op)) {
        return Format3AND4(op, operand, address);
    }

    return "";
}

string Format2(const string& opcode, const string& operand) {
    if (OPTAB.find(opcode) == OPTAB.end()) return "";

    int opVal = OPTAB[opcode];
    int r1 = 0;
    int r2 = 0;

    string first = operand;
    string second = "";

    size_t comma = operand.find(',');
    if (comma != string::npos) {
        first = trim(operand.substr(0, comma));
        second = trim(operand.substr(comma + 1));
    }

    if (!first.empty()) {
        if (REGTAB.find(first) != REGTAB.end()) r1 = REGTAB[first];
        else if (isNumber(first)) r1 = stoi(first);
        else return "";
    }

    if (!second.empty()) {
        if (REGTAB.find(second) != REGTAB.end()) r2 = REGTAB[second];
        else if (isNumber(second)) r2 = stoi(second);
        else return "";
    }

    int value = (opVal << 8) | (r1 << 4) | r2;
    return toHex(value, 4);
}
string Format3AND4(const string& opcode, const string& operand, int address) {
    string op = opcode;
    bool extended = false;

    if (!op.empty() && op[0] == '+') { // + indicates format 4
        extended = true;
        op = op.substr(1);
    }

    if (OPTAB.find(op) == OPTAB.end()) return "";

    if (op == "RSUB") {
        int opbyte = (OPTAB[op] & 0xFC) | 0x03;
        int value = (opbyte << 16);
        return toHex(value, 6);
    }

    string oper = trim(operand);
    if (!oper.empty() && oper.back() == ',') {
    oper.pop_back();
    }

    bool n = true, i = true, x = false, b = false, p = false, e = extended;
    int target = 0;
    int disp = 0;
    bool immediateConstant = false;

    if (!oper.empty() && oper[0] == '#') { //for immidiate addressing
        n = false;
        i = true;
        oper = oper.substr(1);

        if (!oper.empty() && isdigit(oper[0])) {
            target = stoi(oper);
            immediateConstant = true;
        } else if (SYMTAB.find(oper) != SYMTAB.end()) {
            target = SYMTAB[oper];
        } else {
            return "";
        }
    } else if (!oper.empty() && oper[0] == '@') {   //for indirect addressing
        n = true;
        i = false;
        oper = oper.substr(1);
    }

    size_t comma = oper.find(",X");     //for indexed addressing
    if (comma != string::npos) {
        x = true;
        oper = trim(oper.substr(0, comma));
    }

    

    if (!( !operand.empty() && (operand[0] == '#' || operand[0] == '@') )) {    //simple addressing (aka not @ or #)
        if (isNumber(oper)) {
            target = stoi(oper);
            immediateConstant = true;
        } else if (SYMTAB.find(oper) != SYMTAB.end()) {
            target = SYMTAB[oper];
        } else {
            return "";
        }
    }

    int opbyte = (OPTAB[op] & 0xFC);
    if (n) opbyte |= 0x02;
    if (i) opbyte |= 0x01;

    if (extended) {
        // immediate + format 4
        if (!n && i) {
        b = false;
        p = false;
        }
        int flags = 0;
        if (x) flags |= 0x8;
        if (b) flags |= 0x4;
        if (p) flags |= 0x2;
        if (e) flags |= 0x1;

        int addr20 = target & 0xFFFFF;
        int value = (opbyte << 24) | (flags << 20) | addr20;
        return toHex(value, 8);
    }

    if (immediateConstant && !n && i) {
        if (target < 0 || target > 4095) return "";
        disp = target;
        b = false;
        p = false;
    } else {
        int nextAddress = address + 3;
        int pcDisp = target - nextAddress;

        if (pcDisp >= -2048 && pcDisp <= 2047) {
            disp = pcDisp & 0xFFF;
            p = true;
            b = false;
        } else if (!nobase) {
            int baseDisp = target - baseAddress;
            if (baseDisp >= 0 && baseDisp <= 4095) {
                disp = baseDisp;
                b = true;
                p = false;
            } else {
                return "";
            }
        } else {
            return "";
        }
    }

    int flags = 0;
    if (x) flags |= 0x8;
    if (b) flags |= 0x4;
    if (p) flags |= 0x2;
    if (e) flags |= 0x1;

    int value = (opbyte << 16) | (flags << 12) | (disp & 0xFFF);
    return toHex(value, 6);
}

map<string, int> OPTAB = {
    {"ADD", 0x18}, 
    {"ADDF", 0x58}, 
    {"ADDR", 0x90}, 
    {"AND", 0x40},
    {"CLEAR", 0xB4}, 
    {"COMP", 0x28},
    {"COMPF", 0x88}, 
    {"COMPR", 0xA0}, 
    {"DIV", 0x24},
    {"DIVF", 0x64}, 
    {"DIVR", 0x9C}, 
    {"FIX", 0xC4},
    {"FLOAT", 0xC0}, 
    {"HIO", 0xF4}, 
    {"J", 0x3C},
    {"JEQ", 0x30}, 
    {"JGT", 0x34}, 
    {"JLT", 0x38},
    {"JSUB", 0x48}, 
    {"LDA", 0x00}, 
    {"LDB", 0x68},
    {"LDCH", 0x50}, 
    {"LDF", 0x70}, 
    {"LDL", 0x08},
    {"LDS", 0x6C}, 
    {"LDT", 0x74}, 
    {"LDX", 0x04},
    {"LPS", 0xD0}, 
    {"MUL", 0x20}, 
    {"MULF", 0x60},
    {"MULR", 0x98}, 
    {"NORM", 0xC8}, 
    {"OR", 0x44},
    {"RD", 0xD8}, 
    {"RMO", 0xAC}, 
    {"RSUB", 0x4C},
    {"SHIFTL", 0xA4}, 
    {"SHIFTR", 0xA8}, 
    {"SIO", 0xF0},
    {"SSK", 0xEC}, 
    {"STA", 0x0C}, 
    {"STB", 0x78},
    {"STCH", 0x54}, 
    {"STF", 0x80}, 
    {"STI", 0xD4},
    {"STL", 0x14}, 
    {"STS", 0x7C}, 
    {"STSW", 0xE8},
    {"STT", 0x84}, 
    {"STX", 0x10}, 
    {"SUB", 0x1C},
    {"SUBF", 0x5C}, 
    {"SUBR", 0x94}, 
    {"SVC", 0xB0},
    {"TD", 0xE0}, 
    {"TIO", 0xF8}, 
    {"TIX", 0x2C},
    {"TIXR", 0xB8}, 
    {"WD", 0xDC}
};

map<string, int> REGTAB = {
    {"A", 0}, {"X", 1}, {"L", 2}, {"B", 3},
    {"S", 4}, {"T", 5}, {"F", 6}, {"PC", 8}, {"SW", 9}
};
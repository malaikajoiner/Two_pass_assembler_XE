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

map<string, int> SYMTAB;

ifstream IF;
ofstream LF;

int baseAddress = 0;

string str_line, str_read, objectCode, curr_record, MR, base, label, operand, opcode, comment, operand1, operand2;;
bool nobase;
int TR_len, block, lineNumber, address, startAddress;
bool isComment;

void pass2(string filename){
    size_t dot = filename.find_last_of('.');

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

    while(read_IF(IF, isComment, lineNumber, address, block, label, opcode, operand, comment)){
        string obj = "";

        if (isComment) {
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

        writeL(LF, lineNumber, address, label, opcode, operand, obj, comment, false);
    }

    IF.close();
    LF.close();
};

string toHex(int value, int width) {
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << value;
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

void write(ofstream& filename, string text, bool nextLine){             //nextLine determines if next text begins on a new line.     
    if(nextLine){
        filename<<text<<endl;       
    }else{
        filename<<text;
    }
}

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

    if (index < line.length() && line[index] == '.') {
        isComment = true;
        comment = line.substr(index);
        return true;
    }
    lineNumber = stoi(read_section(line, index));
    address = stoul(read(line, index), nullptr, 16);
    
    string field = read_section(line,index);
    if(field == ""){
        block = -1;
    }else{
        block = stoi(field);
    }

    label = read_section(line, index);
    opcode = read_section(line, index);
    operand = read_section(line, index);
    
    if(index < line.length()){
        comment = line.substr(index);
    }
    return true;

}

void read_non_space(string line, int& index, bool& status, string& data, bool toEnd=false){
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
    if(dot == string::npos){
         base = filename;
    }else{
         base = filename.substr(0, dot);
    }
    ifstream symFile(base + ".st");

    if (!symFile) {
        cout << "Unable to open SYMTAB file\n";
        return;
    }

    string line;
    getline(symFile, line); // skip "SYMTAB:"
    while (getline(symFile, line)) {
        istringstream iss(line);
        string symbol, arrow, value;
        iss >> symbol >> arrow >> value;
        if (!symbol.empty() && !value.empty()) {
            SYMTAB[symbol] = stoi(value, nullptr, 16);
        }
    }
}


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
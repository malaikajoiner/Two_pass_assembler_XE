/************************************************
File: pass1.cpp
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description:
This file is the pass 1 cpp file
************************************************/
#include "pass1.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;


// Format1
bool isFormat1(string opcode) {
    if (opcode == "FIX"  || opcode == "FLOAT" || opcode == "HIO" ||
        opcode == "NORM" || opcode == "SIO"   || opcode == "TIO")
        return true;
    return false;
}

// Format2
bool isFormat2(string opcode) {
    if (opcode == "ADDR"  || opcode == "CLEAR" || opcode == "COMPR" ||
        opcode == "DIVR"  || opcode == "MULR"  || opcode == "RMO"   ||
        opcode == "SHIFTL"|| opcode == "SHIFTR"|| opcode == "SUBR"  ||
        opcode == "SVC"   || opcode == "TIXR")
        return true;
    return false;
}

// Format 3
bool isInstruction(string opcode) {
    if (opcode == "ADD"  || opcode == "ADDF" || opcode == "AND"  ||
        opcode == "COMP" || opcode == "COMPF"|| opcode == "DIV"  ||
        opcode == "DIVF" || opcode == "J"    || opcode == "JEQ"  ||
        opcode == "JGT"  || opcode == "JLT"  || opcode == "JSUB" ||
        opcode == "LDA"  || opcode == "LDB"  || opcode == "LDCH" ||
        opcode == "LDF"  || opcode == "LDL"  || opcode == "LDS"  ||
        opcode == "LDT"  || opcode == "LDX"  || opcode == "MUL"  ||
        opcode == "MULF" || opcode == "OR"   || opcode == "RD"   ||
        opcode == "RSUB" || opcode == "STA"  || opcode == "STB"  ||
        opcode == "STCH" || opcode == "STF"  || opcode == "STI"  ||
        opcode == "STL"  || opcode == "STS"  || opcode == "STSW" ||
        opcode == "STT"  || opcode == "STX"  || opcode == "SUB"  ||
        opcode == "SUBF" || opcode == "TD"   || opcode == "TIX"  ||
        opcode == "WD")
        return true;
    return false;
}

void pass1(string filename) {
    ifstream file; // "file reader" used to read file
    file.open(filename);
    string stFile = filename.substr(0, filename.length() - 4) + ".st"; // -4 => delete .sic from the file name
    ofstream outFile(stFile);

    // intermediate file for teammate
    ofstream intermediate("intermediate.txt");

    if (!outFile) {
        cout << "Unable to open file for SYMTAB" << endl;
        return;
    }

    if (!file) {
        cout << "can't open the file" << endl;
        return;
    }

    string line;
    int locctr = 0;
    int startAddress = 0;

    map<string, int> SYMTAB;

    vector<LineInfo> lines; // save all line info => so we can access each line later in pass 2

    while (getline(file, line)) { // reading each line in the file
        if (line.empty()) continue;
        if (line[0] == '.') continue;

        if (line[0] == '*') {
            istringstream iss(line);
            string star, literal;
            iss >> star >> literal;

            int currentAddress = locctr;

            if (literal.size() > 3 && literal[1] == 'C') {
                locctr += literal.length() - 3;
            } else if (literal.size() > 3 && literal[1] == 'X') {
                locctr += (literal.length() - 3) / 2;
            }

            LineInfo info;
            info.address = currentAddress;
            info.label = "*";
            info.opcode = literal;
            info.operand = "";
            lines.push_back(info);

            intermediate << hex << uppercase << info.address << " "
                         << info.label << " "
                         << info.opcode << " "
                         << info.operand << endl;
            intermediate << dec;

            continue;
        }

        // separate to label, opcode, and operand
        istringstream iss(line); // making parser( reads a line and splits it into meaningful pieces)
        string label, opcode, operand;

        if (line[0] == ' ' || line[0] == '\t') { // no LABEL
            iss >> opcode >> operand;
            label = "";
        } else {
            iss >> label >> opcode >> operand;
        }

        // address
        int currentAddress;

        // Originally, operand is a string, such as "0" or "1000", so we need to convert it to an integer. stoi is doing that!!
        if (opcode == "START") {
            startAddress = stoi(operand, nullptr, 16);
            locctr = startAddress;
            currentAddress = locctr;
        } else {
            currentAddress = locctr;

            if (opcode == "WORD") {
                locctr += 3;
            } else if (opcode == "RESW") {
                locctr += 3 * stoi(operand);
            } else if (opcode == "RESB") {
                locctr += stoi(operand);
            } else if (opcode == "BYTE") {
                if (!operand.empty() && operand[0] == 'C') {
                    locctr += operand.length() - 3;
                } else if (!operand.empty() && operand[0] == 'X') {
                    locctr += (operand.length() - 3) / 2;
                }
            } else if (opcode == "BASE" || opcode == "END") {
                locctr += 0;
            } else if (isFormat1(opcode)) {
                locctr += 1;
            } else if (isFormat2(opcode)) {
                locctr += 2;
            } else if (!opcode.empty() && opcode[0] == '=') {
                if (opcode.size() > 3 && opcode[1] == 'C') {
                    locctr += opcode.length() - 4;
                } else if (opcode.size() > 3 && opcode[1] == 'X') {
                    locctr += (opcode.length() - 4) / 2;
                } else {
                    cout << "Invalid literal: " << opcode << endl;
                }
            } else if (!opcode.empty() && opcode[0] == '+') { // if we don't put the first "empty" part, it will crash when opcode is empty.
                locctr += 4;
            } else if (isInstruction(opcode)) {
                locctr += 3;
            } else if (!opcode.empty() && opcode[0] == '=') {
                // skip
            } else {
                cout << "Invalid opcode: " << opcode << endl;
            }
        }

        // SYMTAB
        if (!label.empty() && label != "*" && label[0] != '=') {
            if (SYMTAB.find(label) != SYMTAB.end()) { // find => iterator( the location of the label) or SYMTAB.end() (not found)
                cout << "Duplicate label: " << label << endl;
            } else {
                SYMTAB[label] = currentAddress;
            }
        }

        // save the information for each line
        LineInfo info;
        info.address = currentAddress;
        info.label = label;
        info.opcode = opcode;
        info.operand = operand;
        lines.push_back(info);

        // write intermediate line
        if (intermediate.is_open()) {
            string labelOut = info.label.empty() ? "-" : info.label;

            intermediate << hex << uppercase << info.address << " "
                         << labelOut << " "
                         << info.opcode << " "
                         << info.operand << endl;
            intermediate << dec;
        }

        /*
        test(address, label, opcode, operand)
        cout << "address: " << currentAddress
             << ", label: " << label
             << ", opcode: " << opcode
             << ", operand: " << operand << endl;
        */
    }

    intermediate.close();

    // length
    int programLength = locctr - startAddress;
    cout << "Program length: " << hex << programLength << endl;
    cout << dec;

    // open outputfile
    outFile << "Csect   Symbol   Value   LENGTH   Flags:\n";
    outFile << "----------------------------------------\n";

    // control section line (you can just use PROG)
    outFile << left << setw(8) << "PROG"
            << setw(8) << ""
            << uppercase << hex << setw(6) << setfill('0') << startAddress
            << " "
            << setw(6) << setfill('0') << programLength
            << setfill(' ')
            << "\n";

    // symbol rows
    for (auto &pair : SYMTAB) {
        outFile << left << setw(8) << ""
                << setw(8) << pair.first
                << uppercase << hex << setw(6) << setfill('0') << pair.second
                << setfill(' ')
                << "      \n";
    }

    /*    cout << "SYMTAB:" << endl;
        for (auto &pair : SYMTAB) {
            cout << pair.first << " -> " << pair.second << endl; }*/
}
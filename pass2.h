/************************************************
File: pass2.h
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description: 
This file declares the functions and constants used in pass2.cpp
************************************************/

#ifndef PASS2_H
#define PASS2_H


#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

extern map<string, int> SYMTAB, OPTAB, REGTAB;

extern ifstream IF;
extern ofstream LF;

extern string str_line, objectCode, curr_record, MR, base, label, operand, opcode, comment, operand1, operand2;;
extern bool nobase;
extern int TR_len, lineNumber, block, address, startAddress, baseAddress;
extern bool isComment;

void pass2(string filename);   //main function for calling pass 2 from main.cpp

void write(ofstream& filename, string text, bool nextLine); //writes text to output files

string read_section(const string& section, int& index); //reads a field starting at given index until it hits a white space

//This function reads a line from intermidiate file
bool read_IF(ifstream& filename, bool& isComment, int& lineNumber, int& address, int& block, string& label, string& opcode, string& operand, string& comment);

//used to read 
void read_non_space(string line, int& index, bool& status, string& data, bool toEnd=false);

string read(string data, int& index); //simple version to read sections

//used to identify operands
void readOperand(string line, int& index, bool& status, string& data);

//converts integers to hex
string toHex(int value, int width);

//writes a line to listing file
void writeL(ofstream& out, int lineNumber, int address, const string& label, const string& opcode, const string& operand, const string& objectCode, const string& comment, bool isCommentLine);




string IS_WORD(const string& operand);              //generates object codes for word, byte, and literals

string IS_BYTE(const string& operand);

string IS_Literal(const string& literal);

void IS_BASE(const string& operand);    //used to process BASE directive

string IS_Instruction(const string& opcode, const string& operand, int address);     //chooses instruction format

string Format2(const string& opcode, const string& operand);                          //helper functions based on each instruction format

string Format3AND4(const string& opcode, const string& operand, int address);

bool isNumber(const string& s);

string trim(const string& s);   //removes whitespaces from strings

void loadSYMTAB(string filename);   //load symbol table into memory

#endif




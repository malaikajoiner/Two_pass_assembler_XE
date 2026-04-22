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

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <map>
using namespace std;

extern map<string, int> SYMTAB;

extern ifstream IF;
extern ofstream LF;

extern string str_line, objectCode, curr_record, MR, base, label, operand, opcode, comment, operand1, operand2;;
extern bool nobase;
extern int TR_len, block, lineNumber, address, startAddress;
extern bool isComment;

void pass2(string filename);   //main function for calling pass 2 from main.cpp

void write(ofstream& filename, string text, bool nextLine); //writes text to given file

string read_section(const string& section, int& index);

bool read_IF(ifstream& filename, bool& isComment, int& lineNumber, int& address, int& block, string& label, string& opcode, string& operand, string& comment);

void read_non_space(string line, int& index, bool& status, string& data, bool toEnd=false);

string read(string data, int& index);

void readOperand(string line, int& index, bool& status, string& data);

#endif




/************************************************
File: pass2.h
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description: 
This file declares the functions and constants used in pass2.cpp
************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

ifstream IF;                    //reads intermediate file 
ofstream LF, SYMTAB;            //creates output files (Listing file and Symbol Table)

string str_line;                //temp string for reading whole line
string str_read;                //temp string for reading a section

string objectCode;                 
int TR_len;                      //text record length
string curr_record;              //current text record
string MR;                      //Modification record
bool nobase;
int block = 0;

void pass2(string filename){}   //main function for calling pass 2 from main.cpp

void write(ofstream& filename, string text, bool nextLine){} //writes text to given file

string read_section(const string& section, int& index){}

bool read_IF(ifstream& filename, bool& isComment, int& lineNumber, int& address, int& block, string& label, string& opcode, string& operand, string& comment){
    string file = "";
    string buffer = ""
}








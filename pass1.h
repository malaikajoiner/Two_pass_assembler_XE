/************************************************
File: pass1.h
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description:
This file declares the functions and constants used in pass1.cpp
************************************************/


#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <map>
#include <ios>
#include <iomanip>

using namespace std;

struct LineInfo {
    int address;
    string label;
    string opcode;
    string operand;
};

bool isFormat1(string opcode);
bool isFormat2(string opcode);
bool isInstruction(string opcode);

void pass1(string filename);

#endif
/************************************************
File: main.cpp
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc2530

File Description:
This file is the main, it handles going through pass 1 and pass 2.
************************************************/

#include <iostream>
#include "pass2.h"
#include "pass1.h"
using namespace std;

int main(int argc, char* argv[]){
    if (argc <= 1){cout << "sorry, there are no source files. needs inputs." << endl; return 0;}
    for (int i = 1; i < argc; i++){
        string filename = argv[i];
        pass1(filename);
        pass2(filename);
    }
    return 0;
}
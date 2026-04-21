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

void pass2(string filename){
    IF.open(filename+"_intermediate_file");     //opens an intermediate file for current input file
    LF.open(filename+"_intermediate_file");     //opens a listing file for current input file
    getline(IF, str_line); 
    write(LF, ".  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER", true);
    objectCode = "";
    TR_len = 0;
    curr_record = "";
    MR = "";
    nobase = true;
    block = 0;

    read_IF(IF, )

}

string read_section(const string& section, int& index){                 //reads a section until parsed by white space
    str_read = "";
    
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
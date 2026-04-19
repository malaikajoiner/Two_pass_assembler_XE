/************************************************
File: README.md
Assignment: Assignment 2, SIC/XE 2 pass assembler
Class: CS530, Spring 2026
Malaika Joiner cssc2527
Ryoka Kobayashi cssc####????

File Description: 
This file contains the documentation and overall description of the program
************************************************/

Program description: 
This is a 2 pass assembler designed to take SIC/XE source files and output corresponding listing (.l) files and symbol table (.st) files.

Files Included:
- README.md
- Makefile
- main.cpp
- pass1.h
- pass1.cpp
- pass2.h
- pass2.cpp
- main.sic
- calc.sic
- io.sic

Compile Instructions:
Run 'make' to compile the program and generate the lxe executable file.
Run 'make clean' to remove object files and other unnecessary files.

Operating Instructions:
Provide an input in the command line using the following format,
./lxe file_name1 file_name2...
The input files must be SIC/XE assembler source files ending with .sic and are located within this directory.
You may use multiple source files, but the program will terminate and return an error message if there is not at least 1 input file.

Supported Assembler Directives:
- START
- END
- BYTE
- WORD
- RESB
- RESW
- BASE
- NOBASE
- LTORG(?)
- USE(?)

Known Deficiencies:
This project does not yet support the use of EQU or ORG assembler directives.

Additional Features:
- Instruction Formats 1-4
- Addressing modes Immediate(#), Indirect(@), Simple/Direct, and Indexed
- PC Relative and Base Relative addressing
- Use of literals

Pass 1:
- Initializes LOCCTR with START directive
- Assigns addresses and increments LOCCTR (based on each instruction format)
- Builds SYMTAB and detect duplicate labels
- Ignores . comments

Pass 2:
- Resolve labels using SYMTAB
- Generate machine code
- Supports addressing modes when calculating addresses (supports Base relative addressing)
- Sets nixbpe flags
- Assign addresses to literals
- Processes pass 2 directives

Lessons Learned:
This project gave us a deeper understanding of the SIC/XE two pass assembler design.

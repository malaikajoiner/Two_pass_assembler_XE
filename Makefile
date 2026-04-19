#************************************************
# File: Makefile
# Assignment: Assignment 2, SIC/XE 2 pass assembler
# Class: CS530, Spring 2026
# Malaika Joiner cssc2527
# Ryoka Kobayashi cssc####????

# File Description: 
# This is the Makefile used to compile the program
#************************************************

EXEC = lxe
FILES = main.cpp pass1.cpp pass2.cpp
OBJECTS = $(FILES:.cpp=.o)

CXX = g++
CXXFLAGS = -Wall -g

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS)

%.o: %.cpp pass1.h pass2.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(EXEC)

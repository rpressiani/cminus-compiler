# Makefile for cminus

CC=gcc
LEX=flex
YACC=bison
MV=/bin/mv
RM=/bin/rm
CP=/bin/cp

LIBRARIES=  

CFLAGS=-g -Wall
YFLAGS=-d

PROGRAM=bin/symboltable

OBJECTS=main.o symbolTable.o

SOURCES=src/main.c src/symbolTable.c


$(PROGRAM): $(OBJECTS)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBRARIES)

clean:
	$(RM) $(PROGRAM) $(OBJECTS) 

submit:
	mkdir submit
	cp symbolTable.c submit
	turnin -ccs473 -psymtab submit

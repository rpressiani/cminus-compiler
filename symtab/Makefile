# Makefile for cminus

CC=gcc
LEX=flex
YACC=bison
MV=/bin/mv
RM=/bin/rm
CP=/bin/cp
MKDIR=/bin/mkdir

LIBRARIES=  

CFLAGS=-g -Wall
YFLAGS=-d

SRCDIR=src
OBJDIR=obj
BINDIR=bin

_PROGRAM=symboltable
PROGRAM=$(patsubst %,$(BINDIR)/%,$(_PROGRAM))

_SOURCES=main.c symbolTable.c
SOURCES=$(patsubst %,$(SRCDIR)/%,$(_SOURCES))

_OBJECTS=main.o symbolTable.o
OBJECTS=$(patsubst %,$(OBJDIR)/%,$(_OBJECTS))


$(PROGRAM): $(OBJECTS)
	$(MKDIR) -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBRARIES)

obj/symbolTable.o: src/symboltable.c
	$(MKDIR) -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o obj/symbolTable.o src/symbolTable.c

obj/main.o: src/main.c
	$(MKDIR) -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o obj/main.o src/main.c

clean:
	$(RM) -rf $(OBJDIR) $(BINDIR)

submit:
	$(MKDIR) submit
	cp symbolTable.c submit
	turnin -ccs473 -psymtab submit

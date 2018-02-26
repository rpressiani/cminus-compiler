# Makefile for cminus

CC=gcc
LEX=flex
YACC=bison
MV=/bin/mv
RM=/bin/rm
CP=/bin/cp
MKDIR=/bin/mkdir

#COMMON_DIR = /mnt_nfs/home4/instruct/i473/scan
#INCL_DIR = /mnt_nfs/home4/instruct/i473/scan

LIBRARIES=-ll 

CFLAGS=-g

BINDIR=bin

_PROGRAM=cmlexer
PROGRAM=$(patsubst %,$(BINDIR)/%,$(_PROGRAM))

OBJECTS=src/cmlexer.o 

SOURCES=src/cmlexer.l 


$(PROGRAM): $(OBJECTS)
	$(MKDIR) -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBRARIES)

cmlexer.o: src/cmlexer.c src/cmparser.tab.h src/globals.h
	$(CC) $(CFLAGS) -c src/cmlexer.c

cmlexer.c: src/cmlexer.l 
	$(LEX)  -osrc/cmlexer.c src/cmlexer.l 

clean:
	$(RM) $(PROGRAM) $(OBJECTS) 

submit:
	mkdir submit
	cp $(SOURCES) submit
	turnin -ccs473 -plexer submit

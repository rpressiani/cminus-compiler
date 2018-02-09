
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"

// Top should point to the top of the scope stack,
// which is the most recent scope pushed

SymbolTableStackEntryPtr symbolStackTop;

int scopeDepth;


/* global function prototypes */

// Allocate the global scope entry and symbol table --and set scopeDepth to 0
// The global scope remains till the end of the program
// return 1 on success, 0 on failure (such as a failed malloc)
int initSymbolTable() {
    SymbolTableStackEntryPtr stackEntry = malloc(sizeof(SymbolTableStackEntry));
    if (stackEntry == NULL) return 0;

    stackEntry -> symbolTablePtr = malloc(sizeof(SymbolTable));

    for (int i = 0; i < MAXHASHSIZE; i++) {
        stackEntry->symbolTablePtr->hashTable[i] = NULL;
    }

    stackEntry -> prevScope = NULL;

    symbolStackTop = stackEntry;
    scopeDepth = 0;

    return 1;
}


// Look up a given entry 
// ElementPtr symLookup(char *name) {

// }


// Insert an element with a specified type in a particular line number
// initialize the scope depth of the entry from the global var scopeDepth
ElementPtr symInsert(char *name, struct type *type, int line) {
    HashTableEntry newEntry = malloc(sizeof(Element));

    newEntry->key = hash(name, MAXHASHSIZE);
    newEntry->id = name;
    newEntry->linenumber = line;
    newEntry->scope = scopeDepth;
    newEntry->stype = type;
    newEntry->ast = NULL;
    newEntry->next = NULL;

    if (!symbolStackTop->symbolTablePtr->hashTable[newEntry->key]) {
        symbolStackTop->symbolTablePtr->hashTable[newEntry->key] = newEntry;
    } else {
        HashTableEntry last = symbolStackTop->symbolTablePtr->hashTable[newEntry->key];
        while(last->next) {
            last = last->next;
        }
        last->next = newEntry;
    }

    return newEntry;
}


// Push a new entry to the symbol stack
// This should modify the variable top and change the scope depth
// return 1 on success, 0 on failure (such as a failed malloc)
// int enterScope() {

// }


// Pop an entry off the symbol stack
// This should modify top and change scope depth
// void leaveScope() {

// }




// Do not modify this function
void printElement(ElementPtr symelement) {
    if (symelement != NULL) {
        printf("Line %d: %s\n", symelement->linenumber,symelement->id);
    }
    else printf("Wrong call! symbol table entry NULL");
}

// Should traverse through the entire symbol table and print it
// must use the printElement function given above
void printSymbolTable() {
    // TODO Implement for multiple symbolTables
    SymbolTablePtr p = symbolStackTop->symbolTablePtr;

    for (int i = 0; i < MAXHASHSIZE; i++) {
        HashTableEntry element = p->hashTable[i];
        while (element) {
            printElement(element);
            element = element->next;
        }
    }
}


int hash(char* str, int maxHashSize) {
    int sum = 0;
    for (int i = 0; i < strlen(str); ++i) {
        sum += str[i];
    }

    return sum % maxHashSize;
}

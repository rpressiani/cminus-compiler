
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"

// Top should point to the top of the scope stack,
// which is the most recent scope pushed

SymbolTableStackEntryPtr symbolStackTop;

int scopeDepth;

int hash(char *, int );
void printSymbolTable();
void printElement(ElementPtr);
SymbolTableStackEntryPtr createSymbolTable();

/* global function prototypes */

// Allocate the global scope entry and symbol table --and set scopeDepth to 0
// The global scope remains till the end of the program
// return 1 on success, 0 on failure (such as a failed malloc)
int initSymbolTable() {
    SymbolTableStackEntryPtr stackEntry = createSymbolTable();
    if (stackEntry == NULL) return 0;

    stackEntry -> prevScope = NULL;

    symbolStackTop = stackEntry;
    scopeDepth = 0;

    return 1;
}


// Look up a given entry 
ElementPtr symLookup(char *name) {
    int h = hash(name, MAXHASHSIZE);
    SymbolTableStackEntryPtr st = symbolStackTop;

    while (st) {
        HashTableEntry entry = st->symbolTablePtr->hashTable[h];

        while (entry) {
            if (strcmp(entry->id, name) == 0) {
                return entry;
            } else {
                entry = entry->next;
            }
        }

        st = st->prevScope;
    }

    return NULL;
}


// Insert an element with a specified type in a particular line number
// initialize the scope depth of the entry from the global var scopeDepth
ElementPtr symInsert(char *name, struct type *type, int line) {
    HashTableEntry newEntry = malloc(sizeof(Element));

    newEntry->key = hash(name, MAXHASHSIZE);
    // printf("[INSERT - %s] hash: %d\n", name, newEntry->key);
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
int enterScope() {
    SymbolTableStackEntryPtr stackEntry = createSymbolTable();
    if (stackEntry == NULL) return 0;

    stackEntry -> prevScope = symbolStackTop;

    symbolStackTop = stackEntry;
    scopeDepth++;

    return 1;
}


// Pop an entry off the symbol stack
// This should modify top and change scope depth
void leaveScope() {
    if (symbolStackTop->prevScope != NULL) {
        symbolStackTop = symbolStackTop->prevScope;
        scopeDepth--;
    }
}


// Do not modify this function
void printElement(ElementPtr symelement) {
    if (symelement != NULL) {
        printf("Line %d: %s", symelement->linenumber, symelement->id);
        printf("\n");
    }
    else printf("Wrong call! symbol table entry NULL");
}


// Should traverse through the entire symbol table and print it
// must use the printElement function given above
void printSymbolTable() {

    SymbolTableStackEntryPtr current = symbolStackTop;

    while(current != NULL) {
        SymbolTablePtr p = current->symbolTablePtr;

        int i;
        for (i = 0; i < MAXHASHSIZE; i++) {
            HashTableEntry element = p->hashTable[i];
            while (element) {
                printElement(element);
                // printf("[LOOKUP] hash: %d\n", element->key);
                element = element->next;
            }
        }

        current = current->prevScope;
    }
}


int hash(char* str, int maxHashSize) {
    int sum = 0;
    int i;
    
    for (i = 0; i < strlen(str); ++i) {
        sum += str[i];
    }

    return sum % maxHashSize;
}


SymbolTableStackEntryPtr createSymbolTable() {
    SymbolTableStackEntryPtr stackEntry = malloc(sizeof(SymbolTableStackEntry));
    if (stackEntry == NULL) return NULL;

    stackEntry -> symbolTablePtr = malloc(sizeof(struct symbolTable));
    if (stackEntry -> symbolTablePtr == NULL) {
        free(stackEntry);
        return NULL;
    }

    int i;
    for (i = 0; i < MAXHASHSIZE; i++) {
        stackEntry->symbolTablePtr->hashTable[i] = NULL;
    }

    return stackEntry;
}

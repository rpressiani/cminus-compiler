

#include "symbolTable.h"

// Top should point to the top of the scope stack,
// which is the most recent scope pushed

SymbolTableStackEntryPtr symbolStackTop;

int scopeDepth;


/* global function prototypes */

//allocate the global scope entry and symbol table --and set scopeDepth to 0
// The global scope remains till the end of the program
// return 1 on success, 0 on failure (such as a failed malloc)
int		initSymbolTable() 
{
}


// Look up a given entry 
ElementPtr		symLookup(char *name)
{
}


// Insert an element with a specified type in a particular line number
// initialize the scope depth of the entry from the global var scopeDepth
ElementPtr		symInsert(char *name, struct type *type, int line)
{
}


//push a new entry to the symbol stack
// This should modify the variable top and change the scope depth
// return 1 on success, 0 on failure (such as a failed malloc)
int			enterScope()
{
}


//pop an entry off the symbol stack
// This should modify top and change scope depth
void			leaveScope()
{
}




// Do not modify this function
void printElement(ElementPtr symelement) {
    if (symelement != NULL) {
        printf("Line %d: %s", symelement->linenumber,symelement->id);
    }
    else printf("Wrong call! symbol table entry NULL");
}

//should traverse through the entire symbol table and print it
// must use the printElement function given above
void printSymbolTable()
{
}





            


    






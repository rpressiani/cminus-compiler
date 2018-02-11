
#include "../src/symbolTable.h"
#include "stdio.h"
extern SymbolTableStackEntryPtr top;

int main()
{
    initSymbolTable();
    // create a new type structure for a integer variable
    Type *typ_int = (Type *)malloc(sizeof(Type));
    typ_int->kind = INT;
    
    // create a new type structure for a integer variable
    Type *typ_void = (Type *)malloc(sizeof(Type));
    typ_void->kind = VOID;

    // create a new type structure for a integer variable
    Type *typ_func = (Type *)malloc(sizeof(Type));
    typ_func->kind = FUNCTION;
    
    // create a new type structure for a integer variable
    Type *typ_array = (Type *)malloc(sizeof(Type));
    typ_array->kind = ARRAY;
    typ_array->dimension = 10;
    
    ElementPtr s;
        
    symInsert("main", typ_func, 2);
    enterScope();
    printf("----- SYMBOLTABLE -----\n");
    printSymbolTable();
    printf("\n");
    symInsert("x", typ_int, 4);
    symInsert("d", typ_int, 4);
    symInsert("D", typ_int, 13);
    printf("----- SYMBOLTABLE -----\n");
    printSymbolTable();
    printf("\n");
    s = symLookup("D");
    printElement(s);
    printf("\n");
    s = symLookup("d");
    printElement(s);
    printf("\n");
    enterScope();
    symInsert("?", typ_int, 9);
    printf("----- SYMBOLTABLE -----\n");
    printSymbolTable();
    printf("\n");
    s = symLookup("x");
    printElement(s);
    printf("\n");
    
    symInsert("y", typ_array, 5);

    s = symLookup("main");
    printElement(s);
    printf("\n");

       s = symLookup("x");
    printElement(s);
    printf("\n");
     s = symLookup("y");
    printElement(s);
    printf("\n");
       leaveScope();
    //This must return symbol table entry for counter on line  10
       s = symLookup("x");
    printElement(s);
    printf("\n");
    leaveScope();
    s = symLookup("x");
    printElement(s);
    printf("\n");
    leaveScope();
    s = symLookup("x");
    printElement(s);
    printf("\n");
    leaveScope();
    s = symLookup("x");
    printElement(s);
    printf("\n");

    
}


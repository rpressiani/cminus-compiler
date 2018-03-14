#include "symbolTable.h"
#include "ast.h"


extern SymbolTableStackEntryPtr symbolStackTop;

extern int scopeDepth;

//creates a new expression node
AstNodePtr  new_ExprNode(ExpKind kind) 
{
    return NULL;
}

//creates a new statement node
AstNodePtr new_StmtNode(StmtKind kind)
{
    return NULL;
}

//creates a new type node for entry into symbol table
Type* new_type(TypeKind kind)
{
    return NULL;
}

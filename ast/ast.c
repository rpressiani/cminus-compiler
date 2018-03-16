#include "symbolTable.h"
#include "ast.h"


extern SymbolTableStackEntryPtr symbolStackTop;

extern int scopeDepth;

//creates a new expression node
AstNodePtr new_ExprNode(ExpKind kind, int nLinenumber)
{
    AstNodePtr node = (AstNode *)malloc(sizeof(AstNode));
    node->nKind = EXPRESSION;
    node->eKind = kind;
    node->children[0] = NULL;
    node->children[1] = NULL;
    node->children[2] = NULL;
    node->sibling = NULL;
    node->nSymbolTabPtr = symbolStackTop->symbolTablePtr;
    node->nLinenumber = nLinenumber;

    return node;
}

//creates a new statement node
AstNodePtr new_StmtNode(StmtKind kind, int nLinenumber)
{
    AstNodePtr node = (AstNode *)malloc(sizeof(AstNode));
    node->nKind = STMT;
    node->sKind = kind;
    node->children[0] = NULL;
    node->children[1] = NULL;
    node->children[2] = NULL;
    node->sibling = NULL;
    node->nSymbolTabPtr = symbolStackTop->symbolTablePtr;
    node->nLinenumber = nLinenumber;

    return node;
}

//creates a new node
AstNodePtr new_Node(NodeKind kind, char* name, int nLinenumber)
{
    AstNodePtr node = (AstNode *)malloc(sizeof(AstNode));
    node->nKind = kind;
    node->children[0] = NULL;
    node->children[1] = NULL;
    node->children[2] = NULL;
    node->sibling = NULL;
    node->nSymbolPtr = symLookup(name);
    node->nSymbolTabPtr = symbolStackTop->symbolTablePtr;
    node->nLinenumber = nLinenumber;

    return node;
}

//creates a new type node for entry into symbol table
Type* new_type(TypeKind kind)
{
    Type *typ = (Type *)malloc(sizeof(Type));
    typ->kind = kind;
    return typ;
}

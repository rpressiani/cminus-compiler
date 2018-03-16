%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "symbolTable.h"
#include "util.h"


/* other external function prototypes */
extern int yylex();
extern int initLex(int ,  char **);
 
    
/* external global variables */

extern int      yydebug;
extern int      yylineno;
extern SymbolTableStackEntryPtr symbolStackTop;
extern int scopeDepth;

/* function prototypes */ 
void    yyerror(const char *);
void    resetScope();

/* global variables */

int error = FALSE;
AstNodePtr program;

%}

/* YYSTYPE */
%union
{
    AstNodePtr nodePtr;
    int        iVal;
    char      *cVal;
    Type      *type;
}

    

/* terminals */

%token TOK_ELSE TOK_IF TOK_RETURN TOK_VOID TOK_INT TOK_WHILE 
%token TOK_PLUS TOK_MINUS TOK_MULT TOK_DIV TOK_LT TOK_LE TOK_GT TOK_GE TOK_EQ TOK_NE TOK_ASSIGN TOK_SEMI TOK_COMMA
%token TOK_LPAREN TOK_RPAREN TOK_LSQ TOK_RSQ TOK_LBRACE TOK_RBRACE TOK_ERROR
%token <cVal> TOK_ID 
%token <iVal> TOK_NUM

%type <nodePtr> Declarations Functions Fun_Declaration Params Param_List Param
%type <type> Type_Specifier 
%type <nodePtr> Compound_Stmt Statements Statement
%type <nodePtr> Expr_Statement If_Else_Statement Selection_Stmt Iteration_Stmt Return_Stmt
%type <nodePtr> Expression Simple_Expression Additive_Expression Term Factor Var Call
%type <nodePtr> Args Args_List
/* associativity and precedence */

%nonassoc TOK_IF
%nonassoc TOK_ELSE
%right TOK_ASSIGN
%left TOK_EQ TOK_NE
%nonassoc TOK_LT TOK_GT TOK_LE TOK_GE
%left TOK_PLUS TOK_SUB
%left TOK_MULT TOK_DIV 
%nonassoc error

%%


Start
    :   Declarations
    {
        /* Action to accept */
    }
    ; /* note that the rule ends with a semicolon */

Declarations
    :   Functions { 
        program = $1;
    }
    |   Var_Declaration Declarations {}
    ;

Functions
    :   Fun_Declaration {
        $$ = $1;
    }
    |   Fun_Declaration Functions {
        $1->sibling = $2;
        $$ = $1;
    }
    ;

Var_Declaration
    :   Type_Specifier TOK_ID TOK_SEMI {
        if (!symLookup($2) || symLookup($2)->scope < scopeDepth) {
            symInsert($2, $1, yylineno);
        } else {
            yyerror("var redefinition");
        }
    }
    |   Type_Specifier TOK_ID TOK_LSQ TOK_NUM TOK_RSQ TOK_SEMI {
        if (!symLookup($2) || symLookup($2)->scope < scopeDepth) {
            $1->dimension = $4;
            symInsert($2, $1, yylineno);
        } else {
            yyerror("var redefinition");
        }
    }
    ;

Fun_Declaration
    :   Type_Specifier TOK_ID TOK_LPAREN Params TOK_RPAREN Compound_Stmt {
        resetScope();

        if (!symLookup($2)) {
            symInsert($2, $1, yylineno);

            AstNodePtr node = new_Node(METHOD, $2, yylineno);

            if ($4) node->children[0] = $4;
            if ($6) node->children[1] = $6;

            $$ = node;
        } else {
            yyerror("var redefinition");
        }
    }
    ;

Params
    :   Param_List  { enterScope(); $$ = $1; }
    |   TOK_VOID    { enterScope(); $$ = NULL; }
    ;

Param_List
    :   Param TOK_COMMA Param_List {
        $1->sibling = $3;
        $$ = $1;
    }
    |   Param { $$ = $1; }
    ;

Param
    :   Type_Specifier TOK_ID {
        if (scopeDepth == 0) enterScope();

        if (!symLookup($2) || symLookup($2)->scope < scopeDepth) {
            symInsert($2, $1, yylineno);

            AstNodePtr node = new_Node(FORMALVAR, $2, yylineno);

            $$ = node;
        } else {
            yyerror("var redefinition");
        }
    }
    |   Type_Specifier TOK_ID TOK_LSQ TOK_RSQ {
        if (scopeDepth == 0) enterScope();

        if (!symLookup($2) || symLookup($2)->scope < scopeDepth) {
            symInsert($2, $1, yylineno);

            AstNodePtr node = new_Node(FORMALVAR, $2, yylineno);

            $$ = node;
        } else {
            yyerror("var redefinition");
        }
    }
    ;

Type_Specifier
    :   TOK_INT     { $$ = new_type(INT); }
    |   TOK_VOID    { $$ = new_type(VOID); }
    ;

Compound_Stmt
    :   TOK_LBRACE Statements TOK_RBRACE {

        AstNodePtr node = new_StmtNode(COMPOUND_STMT, yylineno);

        if ($2) node->children[0] = $2;

        $$ = node;
    }
    |   TOK_LBRACE Local_Declarations Statements TOK_RBRACE {

        AstNodePtr node = new_StmtNode(COMPOUND_STMT, yylineno);

        if ($3) node->children[0] = $3;

        $$ = node;
    }
    ;

Local_Declarations
    :   Var_Declaration Local_Declarations {}
    |   Var_Declaration {}
    ;

Statements
    :   Statement Statements {
        if ($2) $1->sibling = $2;

        $$ = $1;
    }
    | { $$ = NULL; }
    ;

Statement
    :   Expr_Statement  { $$ = $1; }
    |   Compound_Stmt   {}
    |   Selection_Stmt  {}
    |   Iteration_Stmt  {}
    |   Return_Stmt     { $$ = $1; }
    ;

Expr_Statement
    :   Expression TOK_SEMI {
        AstNodePtr node = new_StmtNode(EXPRESSION_STMT, yylineno);

        node->children[0] = $1;

        $$ = node;
    }
    |   TOK_SEMI {}
    ;

Selection_Stmt
    :   If_Else_Statement %prec TOK_IF {}
    |   If_Else_Statement TOK_ELSE Statement {}
    ;

If_Else_Statement
    :   TOK_IF TOK_LPAREN Expression TOK_RPAREN Statement {}
    ;

Iteration_Stmt
    :   TOK_WHILE TOK_LPAREN Expression TOK_RPAREN Statement {}
    ;

Return_Stmt
    :   TOK_RETURN Expression TOK_SEMI {
        AstNodePtr node = new_StmtNode(RETURN_STMT, yylineno);

        node->children[0] = $2;

        $$ = node;
    }
    |   TOK_RETURN TOK_SEMI {
        $$ = new_StmtNode(RETURN_STMT, yylineno);
    }
    ;

Expression
    :   Var TOK_ASSIGN Expression {
        AstNodePtr node = new_ExprNode(ASSI_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Simple_Expression { $$ = $1; }
    ;

Var
    :   TOK_ID {
        if (symLookup($1)) {
            AstNodePtr node = new_ExprNode(VAR_EXP, yylineno);

            node->nSymbolPtr = symLookup($1);

            $$ = node;
        } else {
            yyerror("var never declared");
        }
    }
    |   TOK_ID TOK_LSQ Expression TOK_RSQ {
        if (symLookup($1)) {
            AstNodePtr node = new_ExprNode(ARRAY_EXP, yylineno);

            node->nSymbolPtr = symLookup($1);
            node->children[0] = $3;

            $$ = node;
        } else {
            yyerror("var never declared");
        }
    }
    ;

Simple_Expression
    :   Additive_Expression TOK_GT Additive_Expression {
        AstNodePtr node = new_ExprNode(GT_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_LT Additive_Expression {
        AstNodePtr node = new_ExprNode(LT_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_GE Additive_Expression {
        AstNodePtr node = new_ExprNode(GE_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_LE Additive_Expression {
        AstNodePtr node = new_ExprNode(LE_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_EQ Additive_Expression {
        AstNodePtr node = new_ExprNode(EQ_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_NE Additive_Expression {
        AstNodePtr node = new_ExprNode(NE_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression { $$ = $1; }
    ;

Additive_Expression
    :   Additive_Expression TOK_PLUS Term {
        AstNodePtr node = new_ExprNode(ADD_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Additive_Expression TOK_MINUS Term {
        AstNodePtr node = new_ExprNode(SUB_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Term { $$ = $1; }
    ;

Term
    :   Term TOK_MULT Factor {
        AstNodePtr node = new_ExprNode(MULT_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Term TOK_DIV Factor {
        AstNodePtr node = new_ExprNode(DIV_EXP, yylineno);

        node->children[0] = $1;
        node->children[1] = $3;

        $$ = node;
    }
    |   Factor { $$ = $1; }
    ;

Factor
    :   TOK_LPAREN Expression TOK_RPAREN { $$ = $2; }
    |   Var     { $$ = $1; }
    |   Call    { $$ = $1; }
    |   TOK_NUM {
        AstNodePtr node = new_ExprNode(CONST_EXP, yylineno);

        node->nValue = $1;

        $$ = node;
    }
    ;

Call
    :   TOK_ID TOK_LPAREN Args TOK_RPAREN {
        AstNodePtr node = new_ExprNode(CALL_EXP, yylineno);

        node->children[0] = $3;
        node->fname = $1;

        $$ = node;
    }
    ;

Args
    :   Args_List { $$ = $1; }
    |   { $$ = NULL; }
    ;

Args_List
    :   Expression TOK_COMMA Args_List {
        $1->sibling = $3;
        $$ = $1;
    }
    |   Expression { $$ = $1; }
    ;

%%
void yyerror (char const *s) {
    error = TRUE;
    fprintf (stderr, "Line %d: %s\n", yylineno, s);
}

void resetScope() {
    while(scopeDepth > 0) {
        leaveScope();
    }
}

int main(int argc, char **argv){

    initLex(argc,argv);
    initSymbolTable();

#ifdef YYLLEXER
   while (gettok() !=0) ; //gettok returns 0 on EOF
    return;
#else
    yyparse();

if (!error) {
    print_Ast();
}

#endif

}

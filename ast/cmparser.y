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
%type <nodePtr> Expression Simple_Expression Additive_Expression Factor Var Call
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
        if (!symLookup($2)) {
            symInsert($2, $1, yylineno);
            // printSymbolTable();
        } else {
            yyerror("var redefinition");
        }
    }
    |   Type_Specifier TOK_ID TOK_LSQ TOK_NUM TOK_RSQ TOK_SEMI {
        if (!symLookup($2)) {
            $1->kind = ARRAY;
            $1->dimension = $4;
            symInsert($2, $1, yylineno);
            // printSymbolTable();
        } else {
            yyerror("var redefinition");
        }
    }
    ;

Fun_Declaration
    :   Type_Specifier TOK_ID TOK_LPAREN Params TOK_RPAREN Compound_Stmt {
        resetScope();
        if (!symLookup($2)) {
            symInsert($2, new_type(FUNCTION), yylineno);

            AstNodePtr node;

            if ($4) {
                node = new_Node(METHOD, $4, $2, yylineno);
            } else {
                node = new_Node(METHOD, NULL, $2, yylineno);
            }

            printSymbolTable();
            $$ = node;
        } else {
            yyerror("var redefinition");
        }
    }
    ;

Params
    :   Param_List  { $$ = $1; }
    |   TOK_VOID    { $$ = NULL; }
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

            AstNodePtr node = new_Node(FORMALVAR, NULL, $2, yylineno);

            printSymbolTable();

            $$ = node;
        } else {
            yyerror("var redefinition");
        }
    }
    |   Type_Specifier TOK_ID TOK_LSQ TOK_RSQ {
        if (scopeDepth == 0) enterScope();

        if (!symLookup($2) || symLookup($2)->scope < scopeDepth) {

            $1->kind = ARRAY;
            symInsert($2, $1, yylineno);

            AstNodePtr node = new_Node(FORMALVAR, NULL, $2, yylineno);

            printSymbolTable();

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

Compound_Stmt : TOK_LBRACE Statements TOK_RBRACE {

            }
              | TOK_LBRACE Local_Declarations Statements TOK_RBRACE {
          
                }
;

Local_Declarations : Var_Declaration Local_Declarations {

            }
           | Var_Declaration {
           
            }
;

Statements : Statement Statements {

            }
       | {
            
            }
;

Statement : Expr_Statement  {
            
            }
      | Compound_Stmt {
            
            }
      | Selection_Stmt {
      
            }
      | Iteration_Stmt {
      
            }
      | Return_Stmt {
        
            }
;

Expr_Statement : Expression TOK_SEMI {

            }
           | TOK_SEMI {
           
                }
;

Selection_Stmt : If_Else_Statement %prec TOK_IF {

            }
           | If_Else_Statement TOK_ELSE Statement {
           
                }
;

If_Else_Statement : TOK_IF TOK_LPAREN Expression TOK_RPAREN Statement {

            }
;

Iteration_Stmt : TOK_WHILE TOK_LPAREN Expression TOK_RPAREN Statement {

            }
;

Return_Stmt : TOK_RETURN Expression TOK_SEMI {

            }
        | TOK_RETURN TOK_SEMI {
        
                }
;

Expression : Var TOK_ASSIGN Expression  {

            }
            | Simple_Expression {
        
                }
;

Var : TOK_ID {
    
            }
    | TOK_ID TOK_LSQ Expression TOK_RSQ {
    
                }
;

Simple_Expression : Additive_Expression TOK_GT Additive_Expression {

            }
                  | Additive_Expression TOK_LT Additive_Expression {
          
            }
                  | Additive_Expression TOK_GE Additive_Expression {
          
            }
                  | Additive_Expression TOK_LE Additive_Expression {
            
            }
                  | Additive_Expression TOK_EQ Additive_Expression {
          
            }
                  | Additive_Expression TOK_NE Additive_Expression {
          
            }
          | Additive_Expression {
          
            }
;

Additive_Expression : Additive_Expression TOK_PLUS Term {

            }
                    | Additive_Expression TOK_MINUS Term {
            
                }
            | Term {
            
                }
;

Term : Term TOK_MULT Factor  {

            }
     |  Term TOK_DIV Factor {
     
                }
     | Factor {
                
            }
;

Factor : TOK_LPAREN Expression TOK_RPAREN {

            }
       | Var {
                
            }
       | Call {
       
                }
       | TOK_NUM {
       
                }
;

Call : TOK_ID TOK_LPAREN Args TOK_RPAREN {

            }
;

Args : Args_List {
            
            }
     | {
     
                }
;

Args_List : Args_List TOK_COMMA Expression {

            }
      | Expression {
      
            }
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

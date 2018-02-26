%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* external function prototypes */
extern int yylex();
extern int initLex(int ,  char **);


/* external global variables */

extern int      yydebug;
extern int      yylineno;


/* function prototypes */ 
void    yyerror(const char *);

/* global variables */

%}

/* YYSTYPE */

/* terminals */
/* Start adding token names here */
/* Your token names must match Project 1 */
/* The file cmparser.tab.h was gets generated here */

%token TOK_ELSE
%token TOK_IF
%token TOK_RETURN
%token TOK_VOID
%token TOK_INT
%token TOK_WHILE
%token TOK_ID
%token TOK_NUM
%token TOK_PLUS
%token TOK_MINUS
%token TOK_MULT
%token TOK_DIV
%token TOK_LT
%token TOK_LE
%token TOK_GT
%token TOK_GE
%token TOK_EQ
%token TOK_NE
%token TOK_ASSIGN
%token TOK_SEMI
%token TOK_COMMA
%token TOK_LPAREN
%token TOK_RPAREN
%token TOK_LSQ
%token TOK_RSQ
%token TOK_LBRACE
%token TOK_RBRACE
%token TOK_ERROR

/* associativity and precedence */
/* specify operator precedence (taken care of by grammar) and associatity here --uncomment */

//%left 
//%right 
//%nonassoc

%left TOK_MULT
%left TOK_DIV
%left TOK_PLUS
%left TOK_MINUS
%nonassoc TOK_LT
%nonassoc TOK_GT
%nonassoc TOK_LE
%nonassoc TOK_GE
%left TOK_EQ
%left TOK_NE
%right TOK_ASSIGN

%nonassoc error

/* Begin your grammar specification here */
%%


start
    :   declarations
    {
        /* Action to accept */
    }
    ; /* note that the rule ends with a semicolon */

declarations
    :   var_declarations fun_declarations
    ;

var_declarations
    :   var_declarations var_declaration;
    |   /* empty */
    ;

fun_declarations
    :   fun_declarations fun_declaration
    |   fun_declaration
    ;

var_declaration
    :   type_specifier TOK_ID TOK_SEMI
    |   type_specifier TOK_ID TOK_LSQ TOK_NUM TOK_RSQ TOK_SEMI
    ;

type_specifier
    :   TOK_INT
    |   TOK_VOID
    ;

fun_declaration
    :   type_specifier TOK_ID TOK_LPAREN params TOK_RPAREN compound_stmt
    ;

params
    :   param_list
    |   TOK_VOID
    ;

param_list
    :   param_list TOK_COMMA param
    |   param
    ;

param
    :   type_specifier TOK_ID
    |   type_specifier TOK_ID TOK_LSQ TOK_RSQ
    ;

compound_stmt
    :   TOK_LBRACE local_declarations statements TOK_RBRACE
    ;

local_declarations
    :   local_declarations local_declaration
    |   /* empty */
    ;

local_declaration
    :   var_declaration
    ;

statements
    :   statements statement
    |   /* empty */
    ;

statement
    :   expression_stmt
    |   compound_stmt
    |   iteration_stmt
    ;

iteration_stmt
    :   TOK_WHILE TOK_LPAREN expression TOK_RPAREN statement
    ;

expression_stmt
    :   expression TOK_SEMI
    |   TOK_SEMI
    ;

expression
    :   var TOK_ASSIGN expression
    |   simple_expression
    ;

var
    :   TOK_ID
    |   TOK_ID TOK_LSQ expression TOK_RSQ
    ;

simple_expression
    :   additive_expression rel_op additive_expression
    |   additive_expression
    ;

rel_op
    :   TOK_LT
    |   TOK_LE
    |   TOK_GT
    |   TOK_GE
    |   TOK_EQ
    |   TOK_NE
    ;

additive_expression
    :   additive_expression add_op term
    |   term
    ;

add_op
    :   TOK_PLUS
    |   TOK_MINUS
    ;

term
    :   term mult_op factor
    |   factor
    ;

mult_op
    :   TOK_MULT
    |   TOK_DIV
    ;

factor
    :   TOK_LPAREN expression TOK_RPAREN
    |   var
    |   call 
    |   TOK_NUM
    ;

call
    :   TOK_ID TOK_LPAREN args TOK_RPAREN
    ;

args
    :   args_list
    |   /* empty */
    ;

args_list
    :   args_list TOK_COMMA expression
    |   expression
    ;


%%
void yyerror (char const *s) {
       fprintf (stderr, "Line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv){

    initLex(argc,argv);

#ifdef YYLLEXER
   while (gettok() !=0) ; //gettok returns 0 on EOF
    return;
#else
    yyparse();
#endif

}

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* external function prototypes */
extern int yylex();
extern int initLex(int ,  char **);
 
    

/* external global variables */

extern int		yydebug;
extern int		yylineno;


/* function prototypes */ 
void	yyerror(const char *);

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

%nonassoc error

/* Begin your grammar specification here */
%%


Start	: /* put your RHS for this rule here */
        {
        }
	
  ; /* note that the rule ends with a semicolon */
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

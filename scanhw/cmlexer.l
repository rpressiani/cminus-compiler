
/**********************/
/* C header files */
/**********************/

%{
#include "globals.h"
#include "cmparser.tab.h"
char tokenString[TOKENMAX];
int printoutScan = 1;

%}

/**********************/
/* start your regular definitions  here */
/**********************/

%option yylineno

/* Keywords */
ELSE 		"else"
IF 			"if"
INT 		"int"
RETURN 		"return"
VOID 		"void"
WHILE 		"while"
AUTO 		"auto"
BREAK 		"break"
CASE 		"case"
CHAR 		"char"
CONST 		"const"
CONTINUE 	"continue"
DEFAULT 	"default"
DO 			"do"
DOUBLE 		"double"
FOR 		"for"
GOTO 		"goto"
LONG 		"long"
SHORT 		"short"
REGISTER 	"register"
SWITCH 		"switch"

NUMBER		[0-9]+
SEMI		";"

/* start your token specifications here */
/* Token names must come from cmparser.tab.h */

%%

\n {}

{ELSE}		{ return TOK_ELSE; }
{IF}		{ return TOK_IF; }
{INT}		{ return TOK_INT; }
{RETURN}	{ return TOK_RETURN; }
{VOID}		{ return TOK_VOID; }
{WHILE}		{ return TOK_WHILE; }
{AUTO}		{ return TOK_ERROR; }
{BREAK}		{ return TOK_ERROR; }
{CASE}		{ return TOK_ERROR; }
{CHAR}		{ return TOK_ERROR; }
{CONST}		{ return TOK_ERROR; }
{CONTINUE}	{ return TOK_ERROR; }
{DEFAULT}	{ return TOK_ERROR; }
{DO}		{ return TOK_ERROR; }
{DOUBLE}	{ return TOK_ERROR; }
{FOR}		{ return TOK_ERROR; }
{GOTO}		{ return TOK_ERROR; }
{LONG}		{ return TOK_ERROR; }
{SHORT}		{ return TOK_ERROR; }
{REGISTER}	{ return TOK_ERROR; }
{SWITCH}	{ return TOK_ERROR; }

{NUMBER} { return TOK_NUM; }
{SEMI} { return TOK_SEMI; }


%%
/**********************/
/* C support functions */
/**********************/

void printToken(int token, char *str)
{
/* Print the line number, token name and matched lexeme
   -- one per line without any additional characters exactly as below */ 
/* Example  13:TOK_INT: 37*/

	switch(token)
		{
			// Keywords
			case TOK_ELSE:
				fprintf(yyout,"%d:TOK_ELSE: %s\n",yylineno,str);
				break;
			case TOK_IF:
				fprintf(yyout,"%d:TOK_IF: %s\n",yylineno,str);
				break;
			case TOK_INT:
				fprintf(yyout,"%d:TOK_INT: %s\n",yylineno,str);
				break;
			case TOK_RETURN:
				fprintf(yyout,"%d:TOK_RETURN: %s\n",yylineno,str);
				break;
			case TOK_VOID:
				fprintf(yyout,"%d:TOK_VOID: %s\n",yylineno,str);
				break;
			case TOK_WHILE:
				fprintf(yyout,"%d:TOK_WHILE: %s\n",yylineno,str);
				break;

			// Other
			case TOK_NUM:
				fprintf(yyout,"%d:TOK_NUM: %s\n",yylineno,str);
				break;
			case TOK_ERROR:
				fprintf(yyout,"%d:TOK_ERROR: %s\n",yylineno,str);
				break;
			case TOK_SEMI:
				fprintf(yyout,"%d:TOK_SEMI: %s\n",yylineno,str);
				break;
		}
}


int gettok(void){
	int currentToken;
	
	currentToken=yylex();
	if (currentToken == 0) { // means EOF}
			return 0;
	}
	strncpy(tokenString, yytext, TOKENMAX);
	if (printoutScan) {
		printToken(currentToken,tokenString);
	}
	return currentToken;
}

int main(int argc, char **argv){

	
   if ( argc > 1 )
	   yyin = fopen( argv[1], "r" );
   else
	yyin = stdin;

   while (gettok() !=0) ; //gettok returns 0 on EOF
	return 1;
} 


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
RETURN 		"return"
WHILE 		"while"
AUTO 		"auto"
BREAK 		"break"
CASE 		"case"
CONST 		"const"
CONTINUE 	"continue"
DEFAULT 	"default"
DO 			"do"
FOR 		"for"
GOTO 		"goto"
REGISTER 	"register"
SWITCH 		"switch"

/* Identifier */
ID 			[A-Z|a-z|_|$][A-Z|a-z|_|$|0-9]*

/* Primitive Types */
INT 		"int"
VOID 		"void"
BYTE 		"byte"
DOUBLE 		"double"
LONG 		"long"
SHORT 		"short"
FLOAT 		"float"
CHAR 		"char"

/* Integer Literals*/
NUMBER		[0-9]+

/* Punctuation */
LPAREN 		"("
RPAREN 		")"
LBRACE 		"{"
RBRACE 		"}"
LSQ 		"["
RSQ 		"]"
SEMI 		";"
COMMA 		","

/* start your token specifications here */
/* Token names must come from cmparser.tab.h */

%%

\n 			{}

{ELSE}		{ return TOK_ELSE; }
{IF}		{ return TOK_IF; }
{RETURN}	{ return TOK_RETURN; }
{WHILE}		{ return TOK_WHILE; }
{AUTO}		{ return TOK_ERROR; }
{BREAK}		{ return TOK_ERROR; }
{CASE}		{ return TOK_ERROR; }
{CONST}		{ return TOK_ERROR; }
{CONTINUE}	{ return TOK_ERROR; }
{DEFAULT}	{ return TOK_ERROR; }
{DO}		{ return TOK_ERROR; }
{FOR}		{ return TOK_ERROR; }
{GOTO}		{ return TOK_ERROR; }
{REGISTER}	{ return TOK_ERROR; }
{SWITCH}	{ return TOK_ERROR; }

{INT}		{ return TOK_INT; }
{VOID}		{ return TOK_VOID; }
{BYTE}		{ return TOK_ERROR; }
{DOUBLE}	{ return TOK_ERROR; }
{LONG}		{ return TOK_ERROR; }
{SHORT}		{ return TOK_ERROR; }
{FLOAT}		{ return TOK_ERROR; }
{CHAR}		{ return TOK_ERROR; }

{ID}		{ return TOK_ID; }

{NUMBER}	{ return TOK_NUM; }

{LPAREN}	{ return TOK_LPAREN; }
{RPAREN}	{ return TOK_RPAREN; }
{LBRACE}	{ return TOK_LBRACE; }
{RBRACE}	{ return TOK_RBRACE; }
{LSQ}		{ return TOK_LSQ; }
{RSQ}		{ return TOK_RSQ; }
{SEMI}		{ return TOK_SEMI; }
{COMMA}		{ return TOK_COMMA; }

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
			case TOK_RETURN:
				fprintf(yyout,"%d:TOK_RETURN: %s\n",yylineno,str);
				break;
			
			case TOK_WHILE:
				fprintf(yyout,"%d:TOK_WHILE: %s\n",yylineno,str);
				break;

			// Identifier
			case TOK_ID:
				fprintf(yyout,"%d:TOK_ID: %s\n",yylineno,str);
				break;

			// Primitive Types
			case TOK_INT:
				fprintf(yyout,"%d:TOK_INT: %s\n",yylineno,str);
				break;
			case TOK_VOID:
				fprintf(yyout,"%d:TOK_VOID: %s\n",yylineno,str);
				break;

			// Integer Literals
			case TOK_NUM:
				fprintf(yyout,"%d:TOK_NUM: %s\n",yylineno,str);
				break;

			// Punctuation
			case TOK_LPAREN:
				fprintf(yyout,"%d:TOK_LPAREN: %s\n",yylineno,str);
				break;
			case TOK_RPAREN:
				fprintf(yyout,"%d:TOK_RPAREN: %s\n",yylineno,str);
				break;
			case TOK_LBRACE:
				fprintf(yyout,"%d:TOK_LBRACE: %s\n",yylineno,str);
				break;
			case TOK_RBRACE:
				fprintf(yyout,"%d:TOK_RBRACE: %s\n",yylineno,str);
				break;
			case TOK_LSQ:
				fprintf(yyout,"%d:TOK_LSQ: %s\n",yylineno,str);
				break;
			case TOK_RSQ:
				fprintf(yyout,"%d:TOK_RSQ: %s\n",yylineno,str);
				break;
			case TOK_SEMI:
				fprintf(yyout,"%d:TOK_SEMI: %s\n",yylineno,str);
				break;
			case TOK_COMMA:
				fprintf(yyout,"%d:TOK_COMMA: %s\n",yylineno,str);
				break;

			// Other
			case TOK_ERROR:
				fprintf(yyout,"%d:TOK_ERROR: %s\n",yylineno,str);
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

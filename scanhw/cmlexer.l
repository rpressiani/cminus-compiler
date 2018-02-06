
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

/* start your token specifications here */
/* Token names must come from cmparser.tab.h */

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

/* Operators */

ASSIGN 		"="
GT 			">"
LT 			"<"
EQ 			"=="
GE 			">="
LE 			"<="
NE 			"!="
PLUS 		"+"
MINUS 		"-"
MULT 		"*"
DIV 		"/"
BNOT 		"~"
TCOND1 		"?"
TCOND2 		":"
INC 		"++"
DEC 		"--"
NOT 		"!"
BAND 		"&"
BOR 		"\|"
BXOR 		"^"
BLS 		"<<"
BRS 		">>"
ADDASS 		"+="
SUBASS 		"-="
MULASS 		"*="
DIVASS 		"/="
BXORASS 	"^="
MODASS 		"%="
BLSASS 		"<<="
BRSASS 		">>="
BANDASS		"&="
BORASS 		"|="
UBRS 		">>>"
UBRSASS 	">>>="

/* COMMENTS */

SINGLELINECOMMENT	\/\/.*
MULTILINECOMMENT	\/\*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*\/

%%

[ \t]+		{}

\n 			{}

{ELSE}		{ return TOK_ELSE; }
{IF}		{ return TOK_IF; }
{RETURN}	{ return TOK_RETURN; }
{WHILE}		{ return TOK_WHILE; }

{INT}		{ return TOK_INT; }
{VOID}		{ return TOK_VOID; }

{NUMBER}	{ return TOK_NUM; }

{LPAREN}	{ return TOK_LPAREN; }
{RPAREN}	{ return TOK_RPAREN; }
{LBRACE}	{ return TOK_LBRACE; }
{RBRACE}	{ return TOK_RBRACE; }
{LSQ}		{ return TOK_LSQ; }
{RSQ}		{ return TOK_RSQ; }
{SEMI}		{ return TOK_SEMI; }
{COMMA}		{ return TOK_COMMA; }

{ASSIGN}	{ return TOK_ASSIGN; }
{GT}		{ return TOK_GT; }
{LT}		{ return TOK_LT; }
{EQ}		{ return TOK_EQ; }
{GE}		{ return TOK_GE; }
{LE}		{ return TOK_LE; }
{NE}		{ return TOK_NE; }
{PLUS}		{ return TOK_PLUS; }
{MINUS}		{ return TOK_MINUS; }
{MULT}		{ return TOK_MULT; }
{DIV}		{ return TOK_DIV; }

{ID}		{ return TOK_ID; }

{MULTILINECOMMENT}		{}
{SINGLELINECOMMENT}		{}

.			{ return TOK_ERROR; }

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

			// Operators
			case TOK_ASSIGN:
				fprintf(yyout,"%d:TOK_ASSIGN: %s\n",yylineno,str);
				break;
			case TOK_GT:
				fprintf(yyout,"%d:TOK_GT: %s\n",yylineno,str);
				break;
			case TOK_LT:
				fprintf(yyout,"%d:TOK_LT: %s\n",yylineno,str);
				break;
			case TOK_EQ:
				fprintf(yyout,"%d:TOK_EQ: %s\n",yylineno,str);
				break;
			case TOK_GE:
				fprintf(yyout,"%d:TOK_GE: %s\n",yylineno,str);
				break;
			case TOK_LE:
				fprintf(yyout,"%d:TOK_LE: %s\n",yylineno,str);
				break;
			case TOK_NE:
				fprintf(yyout,"%d:TOK_NE: %s\n",yylineno,str);
				break;
			case TOK_PLUS:
				fprintf(yyout,"%d:TOK_PLUS: %s\n",yylineno,str);
				break;
			case TOK_MINUS:
				fprintf(yyout,"%d:TOK_MINUS: %s\n",yylineno,str);
				break;
			case TOK_MULT:
				fprintf(yyout,"%d:TOK_MULT: %s\n",yylineno,str);
				break;
			case TOK_DIV:
				fprintf(yyout,"%d:TOK_DIV: %s\n",yylineno,str);
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

	while (gettok() != 0) ; //gettok returns 0 on EOF

	return 1;
} 

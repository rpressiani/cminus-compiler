
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

NUMBER		[0-9]+

/* start your token specifications here */
/* Token names must come from cmparser.tab.h */

%%

{NUMBER} { return TOK_NUM; }


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
            case TOK_INT:
                fprintf(yyout,"%d:TOK_INT: %s\n",yylineno,str);
                break;
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
    return;
} 

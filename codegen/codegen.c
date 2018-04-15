#include "ast.h"
#include "codegen.h"

#define VERBOSE 1

extern FILE         *outfile;
extern AstNodePtr   program;

void emit_label(const char *label){
    fprintf(outfile, "%s:\n", label); 
}
void emit(const char *instr){
    fprintf(outfile, "\t%s\n", instr);
}

char *gen_new_label(){
    return NULL;
}

void code_gen_expr(AstNode *node){
;
}

void code_gen_stmt(AstNode *node){
;
}

void code_gen_method(AstNode *node){
;
}

void printFile(char* filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        return;
    }
 
    // Read contents from file
    char c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
 
    fclose(fptr);
}

void codegen_helper(AstNode *root) {
    switch(root->nKind) {
        case METHOD:
            emit_label(root->nSymbolPtr->id);
            break;
        case FORMALVAR:
            break;
        case STMT:
            break;
        case EXPRESSION:
            break;
        }
}

void codegen(){

    if (program) {
        outfile = fopen( "test.s", "w+" );

        emit(".text");
        emit(".align 2");
        emit("\n");

        codegen_helper(program);

        fclose(outfile);
    } else {
        printf("ERROR\n");
    }

    if (VERBOSE) printFile("test.s");

    
    // emit("subu $sp, $sp, 8");
    // emit("sw $fp, 4($sp)");
    // emit("sw $ra, 0($sp)");
    // emit("addiu $fp, $sp, 4");
 /* from now on call code gen on the main stmt in main */

}   

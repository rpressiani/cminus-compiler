#include "ast.h"
#include "codegen.h"

#define VERBOSE 1

extern FILE         *outfile;
extern AstNodePtr   program;

char *instr;

void emit_label(const char *label){
    fprintf(outfile, "%s:\n", label); 
}
void emit(const char *instr){
    fprintf(outfile, "\t%s\n", instr);
}

char *gen_new_label(){
    return NULL;
}

void code_gen_expr(AstNode *expr){
    if(expr == NULL) return;

    switch(expr->eKind) {
        case VAR_EXP:
            asprintf(&instr, "li $v0, -%d($fp)", expr->nSymbolPtr->offset + 4);
            emit(instr);
            break;
        case ARRAY_EXP:
            break;
        case ASSI_EXP:
            code_gen_expr(expr->children[1]);
            asprintf(&instr, "sw $v0, -%d($fp)", expr->children[0]->nSymbolPtr->offset + 4);
            emit(instr);
            break;
        case ADD_EXP:
            break;
        case SUB_EXP:
            break;
        case MULT_EXP:
            break;
        case DIV_EXP:
            break;
        case GT_EXP:
            break;
        case LT_EXP:
            break;
        case GE_EXP:
            break;
        case LE_EXP:
            break;
        case EQ_EXP:
            break;
        case NE_EXP:
            break;
        case CALL_EXP:
            break;
        case CONST_EXP:
            asprintf(&instr, "li $v0, %d", expr->nValue);
            emit(instr);
            break;
    }
}

void code_gen_localVarDecl(SymbolTablePtr scope) {
    int nVar = 0;

    ElementPtr symelement = scope->queue;
    while(symelement) {
        switch(symelement->stype->kind) {
            case INT:
                nVar++;
                symelement->offset = nVar*4;
                break;
            case ARRAY:
                // TODO
                break;
            // case VOID:
            //     break;
            // case FUNCTION:
            //     break;
            default:
            // TODO
                printf("ERROR\n");
                break;
        }
        symelement = symelement->queue_next; 
    }

    asprintf(&instr, "subu $sp, $sp, %d", nVar*4);
    emit(instr);
}

void code_gen_stmt(AstNode *stmt){
    if(stmt == NULL) return;

    switch(stmt->sKind) {
        case IF_THEN_ELSE_STMT:
            break;
        case WHILE_STMT:
            break;
        case RETURN_STMT:
            break;
        case COMPOUND_STMT:
            if(stmt->nSymbolTabPtr != NULL) code_gen_localVarDecl(stmt->nSymbolTabPtr);
            // codegen first statement
            if(stmt->children[0] != NULL) code_gen_stmt(stmt->children[0]);
            break;
        case EXPRESSION_STMT:
            code_gen_expr(stmt->children[0]);
            break;
    }
    code_gen_stmt(stmt->sibling); // codegen next statement
}

// void code_gen_method(AstNode *node){
// ;
// }

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
    if(root == NULL) return;
    
    switch(root->nKind) {
        case METHOD:
            emit_label(root->nSymbolPtr->id);
            emit("subu $sp, $sp, 8");
            emit("sw $fp, 4($sp)");
            emit("sw $ra, 0($sp)");
            emit("addiu $fp, $sp, 4");

            codegen_helper(root->children[1]); // body of the method
            codegen_helper(root->sibling); // codegen next method
            break;
        case FORMALVAR:
            break;
        case STMT:
            code_gen_stmt(root);
            break;
        case EXPRESSION:
            break;
        }
}

void codegen(){

    if (program) {
        outfile = fopen("temp.s","w+");

        emit(".text");
        emit(".align 2");
        emit("\n");

        codegen_helper(program);

        fclose(outfile);
    } else {
        printf("ERROR\n");
    }
    
    if (VERBOSE) printFile("temp.s");
}

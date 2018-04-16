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

void code_gen_binary_expr(AstNode *expr) {
    code_gen_expr(expr->children[0]);
    emit("subu $sp, $sp, 4");
    emit("sw $v0, 0($sp)");
    code_gen_expr(expr->children[1]);
    emit("lw $v1, 0($sp)");
    emit("addu $sp, $sp, 4");
}

void code_gen_expr(AstNode *expr){
    if(expr == NULL) return;

    switch(expr->eKind) {
        case VAR_EXP:
            if (expr->nSymbolPtr->offset <= 0) {                // LOCAL VAR
                asprintf(&instr, "lw $v0, -%d($fp)", -(expr->nSymbolPtr->offset) + 4);
            } else {                                            // ARGUMENT
                asprintf(&instr, "lw $v0, %d($fp)", expr->nSymbolPtr->offset);
            }
            emit(instr);
            break;
        case ARRAY_EXP:
            break;
        case ASSI_EXP:
            code_gen_expr(expr->children[1]);
            if (expr->children[0]->nSymbolPtr->offset <= 0) {   // LOCAL VAR
                asprintf(&instr, "sw $v0, -%d($fp)", -(expr->children[0]->nSymbolPtr->offset) + 4);
            } else {                                            // ARGUMENT
                asprintf(&instr, "sw $v0, %d($fp)", expr->children[0]->nSymbolPtr->offset);
            }
            emit(instr);
            break;
        case ADD_EXP:
            code_gen_binary_expr(expr);
            emit("add $v0, $v0, $v1");
            break;
        case SUB_EXP:
            code_gen_binary_expr(expr);
            emit("sub $v0, $v0, $v1");
            break;
        case MULT_EXP:
            code_gen_binary_expr(expr);
            emit("mul $v0, $v0, $v1");
            break;
        case DIV_EXP:
            code_gen_binary_expr(expr);
            emit("divu $v0, $v0, $v1");
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
        case CALL_EXP: {
            int nArgs = 0;
            if(expr->children[0] != NULL) {
                AstNodePtr ptr = expr->children[0];
                while(ptr) {
                    nArgs++;
                    ptr = ptr->sibling;
                }

                asprintf(&instr, "subu $sp, $sp, %d", nArgs*4);
                emit(instr);

                int offset = 0;
                ptr = expr->children[0];
                while(ptr) {
                    code_gen_expr(ptr);
                    asprintf(&instr, "sw $v0, %d($sp)", offset*4);
                    emit(instr);
                    offset++;
                    ptr = ptr->sibling;
                }
            }

            if (expr->nSymbolPtr) {
                asprintf(&instr, "jal %s", expr->nSymbolPtr->id);
            } else {
                asprintf(&instr, "jal %s", expr->fname);
            }

            emit(instr);
            asprintf(&instr, "addu  $sp, $sp, %d", nArgs*4);
            emit(instr);
            break;
        }
        case CONST_EXP:
            asprintf(&instr, "li $v0, %d", expr->nValue);
            emit(instr);
            break;
    }
}

int code_gen_localVarDecl(SymbolTablePtr scope) {
    int nVar = 0;

    ElementPtr symelement = scope->queue;
    while(symelement) {
        switch(symelement->stype->kind) {
            case INT:
                nVar++;
                symelement->offset = -(nVar*4);
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

    return nVar;
}

void code_gen_stmt(AstNode *stmt){
    if(stmt == NULL) return;

    switch(stmt->sKind) {
        case IF_THEN_ELSE_STMT:
            break;
        case WHILE_STMT:
            break;
        case RETURN_STMT:
            if (stmt->children[0]) {
                // Assign the return value to $v0
                code_gen_expr(stmt->children[0]);
            }
            //restore values of $ra and $fp
            emit("lw $ra, -4($fp)");
            emit("lw $fp, 0($fp)");
            //adjust the stack once more for $fp and $ra
            emit("addu $sp, $sp, 8");
            //back to caller
            emit("jr $ra");
            break;
        case COMPOUND_STMT: {
            int nVar = 0;
            if(stmt->nSymbolTabPtr != NULL) nVar = code_gen_localVarDecl(stmt->nSymbolTabPtr);
            // codegen first statement
            if(stmt->children[0] != NULL) code_gen_stmt(stmt->children[0]);
            // adjust the stack back
            if (nVar > 0) {
                asprintf(&instr, "addu  $sp, $sp, %d", nVar*4);
                emit(instr);
            }
            break;
        }
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

            if (strcmp(root->nSymbolPtr->id, "output") && strcmp(root->nSymbolPtr->id, "input")) {
                if(root->children[0]) {
                    root->children[0]->nSymbolPtr->offset = 4;
                    codegen_helper(root->children[0]); // codegen parameters of the method
                }
                codegen_helper(root->children[1]); // body of the method
            } else if (strcmp(root->nSymbolPtr->id, "output") == 0) {
                emit("li $v0, 1");
                emit("lw $a0, 4($fp)");
                emit("syscall");
            }

            if (root->nType->function->kind == VOID) {
                // TODO check if return; is present in VOID function
                //restore values of $ra and $fp
                emit("lw $ra, -4($fp)");
                emit("lw $fp, 0($fp)");
                //adjust the stack once more for $fp and $ra
                emit("addu $sp, $sp, 8");
                //back to caller
                emit("jr $ra");
            }

            codegen_helper(root->sibling); // codegen next method
            break;
        case FORMALVAR:
            if (root->sibling) {
                root->sibling->nSymbolPtr->offset = root->nSymbolPtr->offset + 4;
                codegen_helper(root->sibling);
            }
            break;
        case STMT:
            code_gen_stmt(root);
            break;
        case EXPRESSION:
            // TODO
            break;
        }
}

void codegen(){

    if (program) {
        // TODO filname same as source code file
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

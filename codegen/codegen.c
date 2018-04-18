#include "ast.h"
#include "codegen.h"

#define VERBOSE 1

extern FILE         *outfile;
extern AstNodePtr   program;
extern SymbolTableStackEntryPtr symbolStackTop;

char *instr;
int methodNVar = 0;

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
            if (expr->nSymbolPtr->scope == 0) {                 // GLOBAL VAR
                asprintf(&instr, "lw $v0, %s", expr->nSymbolPtr->id);
            } else if (expr->nSymbolPtr->offset <= 0) {         // LOCAL VAR
                asprintf(&instr, "lw $v0, -%d($fp)", -(expr->nSymbolPtr->offset) + 4);
            } else {                                            // ARGUMENT
                asprintf(&instr, "lw $v0, %d($fp)", expr->nSymbolPtr->offset);
            }
            emit(instr);
            break;
        case ARRAY_EXP:
            // store index in v0
            code_gen_expr(expr->children[0]);
            // index * 4
            emit("add $v0, $v0, $v0");
            emit("add $v0, $v0, $v0");
            if (expr->nSymbolPtr->scope == 0) {                 // GLOBAL VAR
                // store array offset in v1
                asprintf(&instr, "la $v1, %s", expr->nSymbolPtr->id);  // get address
                emit(instr);
                // calculate offset of selected cell
                emit("addu $v0, $v1, $v0");
            } else {
                // store array offset in v1
                asprintf(&instr, "li $v1, %d", -(expr->nSymbolPtr->offset) + 4);
                emit(instr);
                // calculate offset of selected cell
                // if only array (c[4]) on the stack and selected c[1]
                // c[0] is $fp-16-4 (due to RA register), so
                // c[1] is $fp-16-4+4, or 16+4 -> 20, 20-4 = 16 -> fp - 16
                emit("subu $v1, $v1, $v0");
                // fp-16 (see above)
                emit("sub $v0, $fp, $v1");
            }
            // store content of cell in v0
            emit("lw $v0, 0($v0)");
            break;
        case ASSI_EXP:
            if (expr->children[0]->eKind == ARRAY_EXP) {
                // store index in v0
                code_gen_expr(expr->children[0]->children[0]);
                // index * 4
                emit("add $v0, $v0, $v0");
                emit("add $v0, $v0, $v0");

                if (expr->children[0]->nSymbolPtr->scope == 0) {                 // GLOBAL VAR
                    // store array offset in v1
                    asprintf(&instr, "la $v1, %s", expr->children[0]->nSymbolPtr->id);  // get address
                    emit(instr);
                    // calculate offset of selected cell
                    emit("addu $v0, $v1, $v0");
                } else {
                    // store array offset in v1
                    asprintf(&instr, "li $v1, %d", -(expr->nSymbolPtr->offset) + 4);
                    emit(instr);
                    // calculate offset of selected cell
                    // if only array (c[4]) on the stack and selected c[1]
                    // c[0] is $fp-16-4 (due to RA register), so
                    // c[1] is $fp-16-4+4, or 16+4 -> 20, 20-4 = 16 -> fp - 16
                    emit("subu $v1, $v1, $v0");
                    // fp-16 (see above)
                    emit("sub $v0, $fp, $v1");
                }
                // store selected cell address on the stack
                emit("subu $sp, $sp, 4");
                emit("sw $v0, 0($sp)");
                // store rhs value in v0
                code_gen_expr(expr->children[1]);
                // get selected cell address back from the stack in v1
                emit("lw $v1, 0($sp)");
                emit("addu $sp, $sp, 4");
                // store rhs value in selected cell address
                emit("sw $v0, 0($v1)");
            } else {
                code_gen_expr(expr->children[1]);
                if (expr->children[0]->nSymbolPtr->scope == 0) {                 // GLOBAL VAR
                    asprintf(&instr, "la $v1, %s", expr->children[0]->nSymbolPtr->id);  // get address
                    emit(instr);
                    asprintf(&instr, "sw $v0, 0($v1)");
                } else if (expr->children[0]->nSymbolPtr->offset <= 0) {   // LOCAL VAR
                    asprintf(&instr, "sw $v0, -%d($fp)", -(expr->children[0]->nSymbolPtr->offset) + 4);
                } else {                                            // ARGUMENT
                    asprintf(&instr, "sw $v0, %d($fp)", expr->children[0]->nSymbolPtr->offset);
                }
                emit(instr);
            }
            break;
        case ADD_EXP:
            code_gen_binary_expr(expr);
            emit("add $v0, $v0, $v1");
            break;
        case SUB_EXP:
            code_gen_binary_expr(expr);
            emit("sub $v0, $v1, $v0");
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
            code_gen_binary_expr(expr);
            emit("sgt $v0, $v1, $v0");
            break;
        case LT_EXP:
            code_gen_binary_expr(expr);
            emit("slt $v0, $v1, $v0");
            break;
        case GE_EXP:
            code_gen_binary_expr(expr);
            emit("sge $v0, $v1, $v0");
            break;
        case LE_EXP:
            code_gen_binary_expr(expr);
            emit("sle $v0, $v1, $v0");
            break;
        case EQ_EXP:
            code_gen_binary_expr(expr);
            emit("seq $v0, $v0, $v1");
            break;
        case NE_EXP:
            code_gen_binary_expr(expr);
            emit("sne $v0, $v0, $v1");
            break;
        case CALL_EXP: {
            emit("#CALL_EXP");
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

    if (!scope->queue) return nVar;

    ElementPtr symelement = scope->queue;
    while(symelement) {
        switch(symelement->stype->kind) {
            case INT:
                nVar++;
                methodNVar++;
                symelement->offset = -(methodNVar*4);
                break;
            case ARRAY:
                nVar = nVar + symelement->stype->dimension;
                methodNVar = methodNVar + symelement->stype->dimension;
                symelement->offset = -(methodNVar*4);
                break;
            // TODO
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

    if(nVar > 0) {
        asprintf(&instr, "subu $sp, $sp, %d", nVar*4);
        emit(instr);
    }

    return nVar;
}

void code_gen_stmt(AstNode *stmt){
    if(stmt == NULL) return;

    switch(stmt->sKind) {
        case IF_THEN_ELSE_STMT:
            codegen_helper(stmt->children[0]);
            if (stmt->children[2]) {
                asprintf(&instr, "beq $v0, $zero, else%d", stmt->nLinenumber);
            } else {
                asprintf(&instr, "beq $v0, $zero, endif%d", stmt->nLinenumber);
            }
            emit(instr);
            code_gen_stmt(stmt->children[1]);
            if (stmt->children[2]) {
                asprintf(&instr, "j endif%d", stmt->nLinenumber);
                emit(instr);
                asprintf(&instr, "else%d", stmt->nLinenumber);
                emit_label(instr);
                code_gen_stmt(stmt->children[2]);
            }
            asprintf(&instr, "endif%d", stmt->nLinenumber);
            emit_label(instr);

            break;
        case WHILE_STMT:
            asprintf(&instr, "beginwhile%d", stmt->nLinenumber);
            emit_label(instr);
            codegen_helper(stmt->children[0]);
            asprintf(&instr, "beq $v0, $zero, endwhile%d", stmt->nLinenumber);
            emit(instr);
            code_gen_stmt(stmt->children[1]);
            asprintf(&instr, "j beginwhile%d", stmt->nLinenumber);
            emit(instr);
            asprintf(&instr, "endwhile%d", stmt->nLinenumber);
            emit_label(instr);
            break;
        case RETURN_STMT:
            if (stmt->children[0]) {
                // Assign the return value to $v0
                codegen_helper(stmt->children[0]);
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
            codegen_helper(stmt->children[0]);
            break;
    }
    code_gen_stmt(stmt->sibling); // codegen next statement
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
            } else if (strcmp(root->nSymbolPtr->id, "input") == 0) {
                emit("li $v0, 5");
                emit("syscall");
                emit("lw $ra, -4($fp)");
                emit("lw $fp, 0($fp)");
                //adjust the stack once more for $fp and $ra
                emit("addu $sp, $sp, 8");
                //back to caller
                emit("jr $ra");
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
            methodNVar = 0;
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
            code_gen_expr(root);
            break;
        }
}

void codegen(){

    if (program) {
        // TODO filname same as source code file
        outfile = fopen("temp.s","w+");

        if (symbolStackTop->symbolTablePtr->queue) {
            emit(".data");
            int nVar = 0;
            ElementPtr symelement = symbolStackTop->symbolTablePtr->queue;
            while(symelement) {
                switch(symelement->stype->kind) {
                    case INT:
                        if (!symelement->stype->function) break;
                        emit_label(symelement->id);
                        asprintf(&instr, ".space %d", 4);
                        emit(instr);
                        break;
                    case ARRAY:
                        emit_label(symelement->id);
                        asprintf(&instr, ".space %d", symelement->stype->dimension*4);
                        emit(instr);
                        break;
                    // TODO
                    case VOID:
                        break;
                    case FUNCTION:
                        break;
                    default:
                        // TODO
                        printf("ERROR\n");
                        break;
                }
                symelement = symelement->queue_next; 
            }
        }

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

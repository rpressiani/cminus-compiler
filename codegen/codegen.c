#include "ast.h"
#include "codegen.h"

extern FILE *outfile;

void emit_label(const char *label){
	fprintf(outfile, "%s\n", label); 
}
void emit(const char *instr){
	fprintf(outfile, "\t%s\n", instr);
}

char *gen_new_label(){
;
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

void codegen(){
	emit(".text");
	emit(".align 2");
	emit("\n");
	emit_label("main:");
	emit("subu $sp, $sp, 8");
	emit("sw $fp, 4($sp)");
	emit("sw $ra, 0($sp)");
	emit("addiu $fp, $sp, 4");
 /* from now on call code gen on the main stmt in main */
}	

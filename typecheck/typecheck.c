
#include "typecheck.h"
#include "symbolTable.h"
#include "ast.h"

void addInputFunc() {
	Type* input_t = (Type*) malloc(sizeof(Type));
	input_t->kind = FUNCTION;

	Type* in_return = (Type*) malloc(sizeof(Type));
	in_return->kind = INT;
	in_return->function = NULL;

	input_t->function = in_return;

	symInsert("input", input_t, 0);
}

void addOutputFunc() {
	Type* output_t = (Type*) malloc(sizeof(Type));
	output_t->kind = FUNCTION;

	Type* out_return = (Type*) malloc(sizeof(Type));
	out_return->kind = VOID;

	output_t->function = out_return;

	Type* out_param1 = (Type*) malloc(sizeof(Type));
	out_param1->kind = INT;
	out_param1->function = NULL;

	output_t->function->function = out_param1;

	symInsert("output", output_t, 0);
}

// Starts typechecking the AST  returns 1 on success
//use the global variable program
// 
int typecheck(){
	addInputFunc();
	addOutputFunc();

	return 1;
}

// compares two types and returns the resulting type
// from the comparison

Type* type_equiv(Type *t1, Type *t2){
}

// Typechecks a method and returns 1 on success
int typecheck_method(AstNode *node_){
}

// Typechecks a statement and returns 1 on success
int typecheck_stmt( AstNode *node_){
}

// Type checks a given expression and returns its type
// 

Type *typecheck_expr (AstNode *node_){
}

void printMethodType(Type* t) {

	printf("RET: %u\n", t->kind);
	printf("-----\n");
	t = t->function;
	while(t) {
		printf("%u\n", t->kind);
		t = t->function;
		printf("-----\n");
	}
	printf("\n");
}

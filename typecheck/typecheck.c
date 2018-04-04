
#include "typecheck.h"
#include "symbolTable.h"
#include "ast.h"

extern AstNode *program;

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

	if(!program) return 0;

	return typecheck_method(program);
}

// compares two types and returns the resulting type
// from the comparison

Type* type_equiv(Type *t1, Type *t2){
	if (t1->kind == t2->kind) {
		return t1;
	} else {
		return NULL;
	}
}

// Typechecks a method and returns 1 on success
int typecheck_method(AstNode *node_){
	if (!node_) return 1;

	int pass = 1;
	if (!typecheck_stmt(node_->children[1]->children[0], node_)) pass = 0;

	return typecheck_method(node_->sibling) && pass;

}

// Typechecks a statement and returns 1 on success
int typecheck_stmt( AstNode *node_, AstNode* method){
	if (!node_) return 1;

	int pass = 1;

	switch(node_->sKind) {
		case RETURN_STMT:
			if (!node_->children[0]) {	// return ;
				if (method->nType->function->kind == VOID) {
					break;
				} else {
					pass = 0;
					printf("[ERROR] Line %d\n", node_->nLinenumber);
				}
			} else {							// return e;
				if (type_equiv(typecheck_expr(node_->children[0]), method->nType->function)) {
					break;
				} else {
					pass = 0;
					printf("[ERROR] Line %d\n", node_->nLinenumber);
				}
			}
			break;
		default:
			break;
	}

	return typecheck_stmt(node_->sibling, method) && pass;
}

// Type checks a given expression and returns its type
Type *typecheck_expr (AstNode *node_){
	Type* type = (Type*) malloc(sizeof(Type));

	switch(node_->eKind) {
		case VAR_EXP:
			return node_->nSymbolPtr->stype;
		case ADD_EXP:
		case SUB_EXP:
		case MULT_EXP:
		case DIV_EXP:
		case EQ_EXP:
		case NE_EXP:
			return type_equiv(typecheck_expr(node_->children[0]), typecheck_expr(node_->children[1]));
		case GT_EXP:
		case LT_EXP:
		case GE_EXP:
		case LE_EXP:
			type->kind = INT;
			if (type_equiv(typecheck_expr(node_->children[0]), type) &&
				type_equiv(typecheck_expr(node_->children[1]), type)) {
				return type;
			} else {
				return NULL;
			}
		case CONST_EXP:
			type->kind = INT;
			return type;
		default:
			return NULL;
	}
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


#include "typecheck.h"
#include "symbolTable.h"
#include "ast.h"

extern AstNode *program;

void addInputFunc() {
	if (!symLookup("input")) {
		Type* input_t = (Type*) malloc(sizeof(Type));
		input_t->kind = FUNCTION;

		Type* in_return = (Type*) malloc(sizeof(Type));
		in_return->kind = INT;
		in_return->function = NULL;

		input_t->function = in_return;

		symInsert("input", input_t, 0);
	} else {
		printf("input function already declared\n");
	}
	
}

void addOutputFunc() {
	if (!symLookup("output")) {
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
	} else {
		printf("output function already declared\n");
	}
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
	if (t1 && t2 && t1->kind == t2->kind) {
		return t1;
	} else {
		return NULL;
	}
}

// Typechecks a method and returns 1 on success
int typecheck_method(AstNode *node_){
	if (!node_) return 1;

	int pass = 1;
	if (!typecheck_stmt(node_->children[1], node_)) pass = 0;

	return typecheck_method(node_->sibling) && pass;
}

// Typechecks a statement and returns 1 on success
int typecheck_stmt( AstNode *node_, AstNode* method){
	if (!node_) return 1;
	
	// If non-void function, last statement must be a RETURN_STMT
	// If sibling == NULL, but statement is COMPOUNT_STMT, proceed only if COMPOUND_STMT is not empty
	if (method->nType->function->kind != VOID && !node_->sibling && (node_->sKind != COMPOUND_STMT || !node_->children[0]) && node_->sKind != RETURN_STMT) {
		printf("[ERROR] Line %d: control reaches end of non-void function\n", node_->nLinenumber);
		return 0;
	}

	int pass = 1;

	switch(node_->sKind) {
		case IF_THEN_ELSE_STMT:
			if (typecheck_expr(node_->children[0])->kind != INT) {
				pass = 0;
				printf("[ERROR] Line %d\n", node_->nLinenumber);
			}
			if (typecheck_stmt(node_->children[1], method) == 0 || typecheck_stmt(node_->children[2], method) == 0) {
				pass = 0;
			}
			break;
		case WHILE_STMT:
			if (typecheck_expr(node_->children[0])->kind != INT) {
				pass = 0;
				printf("[ERROR] Line %d\n", node_->nLinenumber);
			}
			if (typecheck_stmt(node_->children[1], method) == 0) {
				pass = 0;
			}
			break;
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
		case COMPOUND_STMT:
			if (node_->children[0] && !typecheck_stmt(node_->children[0], method)) pass = 0;
			break;
		case EXPRESSION_STMT:
			if (!node_->children[0] || (node_->children[0] && typecheck_expr(node_->children[0]))) { 		// ;
				break;
			} else {
				pass = 0;
				printf("[ERROR] Line %d\n", node_->nLinenumber);
			}
	}

	return typecheck_stmt(node_->sibling, method) && pass;
}

// Type checks a given expression and returns its type
Type *typecheck_expr (AstNode *node_){

	switch(node_->eKind) {
		case VAR_EXP:
			return node_->nSymbolPtr->stype;
		case ARRAY_EXP: {
			if (node_->nSymbolPtr->stype->kind == ARRAY) {
				Type* expr = typecheck_expr(node_->children[0]);
				if (expr && expr->kind == INT) {
					Type* type = (Type*) malloc(sizeof(Type));
					type->kind = INT;
					return type;
				}
			}
			return NULL;
		}
		case ASSI_EXP:
			if (node_->children[0]->eKind == VAR_EXP || node_->children[0]->eKind == ARRAY_EXP) {
				return type_equiv(typecheck_expr(node_->children[0]), typecheck_expr(node_->children[1]));
			}
			return NULL;
		case ADD_EXP:
		case SUB_EXP:
		case MULT_EXP:
		case DIV_EXP:
		case EQ_EXP:
		case NE_EXP:
		case GT_EXP:
		case LT_EXP:
		case GE_EXP:
		case LE_EXP: {
			Type* type = (Type*) malloc(sizeof(Type));
			type->kind = INT;
			if (type_equiv(typecheck_expr(node_->children[0]), type) &&
				type_equiv(typecheck_expr(node_->children[1]), type)) {
				return type;
			} else {
				return NULL;
			}
		}
		case CALL_EXP: {
			
			ElementPtr func = node_->nSymbolPtr;
			
			if (!func) return NULL;

			if (func->stype->kind != FUNCTION) return NULL;

			if (func->stype->function->function != NULL) {
				AstNode* arg = node_->children[0];
				Type* formalvar = func->stype->function->function;

				while(formalvar) {
					if (!arg) return NULL;
					if (type_equiv(typecheck_expr(arg), formalvar)) {
						arg = arg->sibling;
						formalvar = formalvar->function;
						continue;
					} else {
						return NULL;
					}
				}
				if (arg) {
					return NULL;
				}
			}
			return func->stype->function;
		}
		case CONST_EXP: {
			Type* type = (Type*) malloc(sizeof(Type));
			type->kind = INT;
			return type;
		}
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

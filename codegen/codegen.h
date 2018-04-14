void emit(const char *);
char *gen_new_label();
int gen_offsets(SymbolTablePtr s);
void code_gen_expr(AstNode *);
void code_gen_stmt(AstNode *);
void code_gen_method(AstNode *);
void code_gen_binary_expr(AstNode *);
void codegen();

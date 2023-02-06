// TODO: make a token for each keyword for better error messages
// TODO: reach all branches before implement them.
// TODO: make better compile error messages
// TODO: StructField* parse_struct_field(){}
// NOTE: maybe in the future change the initial token kinds for ascii to actual token kind

#ifndef __parser
#define __parser
#include "./lexer.cpp"
#include "./ast.cpp"
#include "./token.cpp"
#include "./grammar.cpp"
#include "./ordering.cpp"
#include "./pre_process.cpp"
AST_ROOT parser_run_code(const char* src);
Decl** parse_file(FILE* f);
Expr* parse_binary();
Expr* parse_e0();
Expr* parse_unary();
Expr* parse_mult();
Expr* parse_expr();
TypeField* parse_typefield();
Type* parse_type();
Type* unsolved_type();
Decl* parse_global_var();
Var*  parse_var_def();
Decl* parse_decl();
Stmt* parse_stmt();
Stmt* parse_while();
Stmt* parse_switch();
switchList* parse_switch_block();
StmtList* parse_block();
ProcArgs* parse_proc_args();
TypeField*  parse_proc_return();

Decl* parse_proc_def();
Decl* parse_typedef();
Decl* parse_namespace();

Elif* parse_elif_nodes();
Stmt* parse_if();
Proc* current_proc = NULL;
void current_proc_reserve_memory(Var* var){
  assert(current_proc);    
  current_proc->stack_allocation += sizeof_type(var->type_field->type);
  
};

size_t calc_offset(size_t sz, bool reset = false){
  static size_t offset = 0;
  if(reset) offset = 0;
  offset += sz;
  return offset;
}
extern void fatal(const char *fmt, ...);
// TODO: expand the exp logic
/* parse_expr logic:
     e0		= INT
     | '(' e_head   ')' 
     | '(' ':' NAME ')' :: Cast

     e1		=    [ - ] e0
     e2		= e1 [/ *] e1
     e_head	= e2 [+ -] e1
     
 */
Expr *parse_e0(){
  Expr *e = new Expr;
  e->type = new Type;
  if(is_token(TokenKind::TOKEN_INT)){
    e->kind   = EXPRKIND_INT;
    e->as.INT = consume().INT;
    return e;
  }
  else if(is_token(TokenKind::TOKEN_STRING)){
    e->kind      = EXPRKIND_STRING_LITERAL;
    e->as.STRING = consume().name;
    return e;
  } 
  else if(expect_token(TOKEN_OPEN_R_PAREN)){
    if(expect_token(TOKEN_DOUBLE_DOT)){
      // TODO: this cast parsing does allocate good memory, fix this
      // Syntax: (:int)(expr)
      TypeField* casting_to = new TypeField;
      casting_to->type = parse_type();
      e->kind = EXPRKIND_CAST;
      e->as.cast.type = casting_to;
      MustExpect(TOKEN_CLOSE_R_PAREN);
      e->as.cast.expr = parse_expr();
      return e;
    }
    else{
      e = parse_expr();
      expect_token(TOKEN_CLOSE_R_PAREN);
      return e;
    }
    assert(false);        
  }
  else if(is_token(TOKEN_OPEN_C_PAREN)){
    fatal("compound expression are not implemented yet.\n");
    exit(1);
  }
  else if(is_token(TOKEN_NAME)){
    if(token_is_name("new")){
      printf("New operator is not avaliable yet.\n");
      exit(1);
    }
    const char* NAME = consume().name;
    if(is_token(TOKEN_OPEN_R_PAREN)){
      e->kind = EXPRKIND_PROC_CALL;
      e->as.call.p_name = NAME;
      next_token();
      Expr **args = NULL;
      e->as.call.args_size = 0; 
      while(!is_token(TOKEN_CLOSE_R_PAREN)){
	if(e->as.call.args_size > 0){
	  MustExpect(TOKEN_COMMA);
	}
	buf__push(args, (parse_expr()));
        e->as.call.args_size++;
      }      
      e->as.call.args = args;
      MustExpect(TOKEN_CLOSE_R_PAREN);
      return e;
    }
    // TODO: implement field_access :: source.field
    else if(expect_token(TOKEN_OPEN_S_PAREN)){
      e->kind = EXPRKIND_ARRAY_ACCESS;
      e->as.ArrayAccess.name  = NAME;
      e->as.ArrayAccess.expr  = parse_expr();
      MustExpect(TOKEN_CLOSE_S_PAREN);
      return e;
    }
    // TODO: implement Namespaces
    
    e->kind       = EXPRKIND_NAME;
    e->name       = NAME;
    return e;
  }

  fatal("Unexpected token: `%s` when trying to parse a expression.\n",
	human_readable_token(token));
  exit(1);
}
Expr* parse_unary(){
  Expr *e = new Expr;

  if(expect_token(TOKEN_TAKEAWAY)){
    e = parse_e0();
    e->as.INT   *= -1;
    e->as.FLOAT *= -1;
  }
  else if(expect_token(TOKEN_BANG)){
    e->kind = EXPRKIND_NEG;
    e->as.expr = parse_unary();    
  }
  else if(expect_token(TOKEN_STAR)){
    e->kind		= EXPRKIND_DERREF_NAME;
    e->as.derref	= parse_unary();
  }
  else if(expect_token(TOKEN_AMPERSAND)){
    e->kind =  EXPRKIND_ADDROF_NAME;
    e->as.addr_of = parse_unary();    
  }
  else{  
    e = parse_e0();
  }
  return e;
}
Expr* parse_mult(){
  Expr* elhs = new Expr;
  elhs = parse_unary();
  while(is_token(TOKEN_STAR) || is_token(TOKEN_DIV)){
    EXPR_BINARY_OP_KIND op = make_expr_binary_op_kind();
    next_token();
    Expr *erhs = new Expr;
    erhs = parse_unary();
    elhs = expr_make_binary(elhs, erhs, op);
  }
  return elhs;
}
inline bool is_token_logic(){
  return token_is_name(OR_KEYWORD)
    || token_is_name(AND_KEYWORD)
    ;
    
}
Expr* parse_logic(){
  Expr* lhs = new Expr;
  lhs = parse_mult();
  while(is_token_logic()){
    Expr* rhs = new Expr;
    EXPR_BINARY_OP_KIND kind = STR_CMP(consume().name, AND_KEYWORD)
      ? OP_KIND_AND
      : OP_KIND_OR;
    rhs = parse_mult();
    lhs = make_expr_logic(kind, lhs, rhs);
  }
  return lhs;
}
inline bool is_token_asign(){
  return is_token(TOKEN_EQ)
    || is_token(TOKEN_EQLESS)
    || is_token(TOKEN_EQPLUS)
    ;
}
Expr* parse_reasign(){
  Expr* e = new Expr;
  e = parse_logic();
  if(is_token_asign()){
    Token tk = consume();
    e = expr_reasign(tk, e, parse_expr());
  }
  return e;
}
inline bool is_token_binary(){
  return is_token(TOKEN_PLUS)
    || is_token(TOKEN_TAKEAWAY)
    || is_token(TOKEN_SHL)
    || is_token(TOKEN_SHR)
    ;
}
Expr* parse_binary(){
  Expr* elhs = new Expr;
  elhs = parse_reasign();
  while(is_token_binary()){
    EXPR_BINARY_OP_KIND op = make_expr_binary_op_kind();
    next_token();
    Expr* erhs = new Expr;
    erhs = parse_reasign();
    elhs = expr_make_binary(elhs, erhs, op);
  }
  return elhs;
}

inline bool is_token_cmp(){
  return is_token(TOKEN_CMP_EQ)
    || is_token(TOKEN_CMP_NEQ)
    || is_token(TOKEN_LESS)
    || is_token(TOKEN_LESSEQ)
    || is_token(TOKEN_GREATER)
    || is_token(TOKEN_GREATEREQ)
    
    ;
}
Expr* parse_cmp(){
  Expr* elhs = new Expr;
  elhs = parse_binary();
  while(is_token_cmp()){
    EXPR_CMP_KIND op = make_cmp_kind();
    next_token();    
    
    Expr* erhs = new Expr;
    erhs = parse_binary();
    elhs = expr_make_cmp(elhs, erhs, op);
  }
  return elhs;  
}
Expr* parse_if_expr(){
  Expr* expr = new Expr;
  expr = parse_cmp();
  if(expect_name(IF_KEYWORD)){
    Expr* if_expr			= new Expr;
    if_expr->kind			= EXPRKIND_LOCAL_IF;
    if_expr->as.local_if.cond		= parse_expr();
    if_expr->as.local_if.if_body	= expr;
    if(!expect_name(ELSE_KEYWORD)){
      fprintf(stderr,
	      "ERROR: expected 'else' after local if.\n");
      exit(1);
    }
    if_expr->as.local_if.else_body      = parse_expr();
    return if_expr;
  }
  return expr;
}
Expr* parse_expr(){
  Expr* lhs = parse_if_expr();
  if(expect_name(AS_KEYWORD)){
    Expr* cast = new Expr;
    TypeField* casting_to = new TypeField;
    casting_to->type = parse_type();
    cast->kind = EXPRKIND_CAST;
    cast->as.cast.type = casting_to;
    cast->as.cast.expr = lhs;
    return cast;
  }
  return lhs;
}
Type* parse_builtin_type(){
  // kind name and size
  assert(*token.name == '_');
  token.name++;
  Type* type = new Type;
  if(STR_CMP(token.name, "char")){
    type->kind = TYPE_CHAR;
    type->size = 4;
    type->name = "char";
    return type;
  }
  if(*token.name == 'i'){
    type->kind = TYPE_I64;    
    type->name = "int";
    token.name++;
    type->size = (size_t)atoi(token.name);
    if(type->size != 8  and
       type->size != 64       
       ){
      fprintf(stderr,
	      "ERROR: built-in type I64 can only work with 8, 16, 32 and 64, but got: %zu.\n",
	      type->size);
      exit(1);
    }
    type->size /= 8;        
  }
  else if (*token.name ==  'f'){
    printf("ERROR: floats are not implemented yet.\n");
    exit(1);
  }
  return type;
}
Type* parse_type(){
  Type* type = new Type;
  type->is_const = false;
  if(expect_name("const")){
    type->is_const = true;
  }
  if(expect_token(TOKEN_STAR)){
    type->kind     = TYPE_PTR;
    type->ptr.base = new Type;
    type->ptr.base = parse_type();
    type->name     = "*";
    type->size     = sizeof_type(type);
    return type;
  }
  assert(token.kind == TOKEN_NAME);

  type->is_unsigned = false;
  
  if(STR_CMP(token.name, UNSIGNED_KEYWORD)){
    type->is_unsigned = true;
    next_token();
  }
  type->size = 0;
  // Built in types start with '_'  
  if(*token.name == '_'){
    Type* btype = parse_builtin_type();
    type->kind = btype->kind;
    type->name = btype->name;
    type->size = btype->size;      
  }
  else {
    type->kind = TypeFieldKind_by_cstr(token.name);
    if(type->kind != TYPE_NONE and type->kind != TYPE_UNSOLVED)
      type->size = sizeof_type(type);
  }
  type->name = token.name;  
  next_token();
  if(expect_token(TOKEN_OPEN_S_PAREN)){
    Type* t = new Type;
    t->kind = TYPE_PTR;
    t->ptr.base = type;
    t->size = 0;
    t->name       = type->name;
    t->size = 0;
    if(!is_token(TOKEN_CLOSE_S_PAREN)){
      Expr* arrsz = parse_expr();
      assert(arrsz->kind == EXPRKIND_INT);
      t->size = arrsz->as.INT;     
    }
    if(STR_CMP(type->name, "mem")){
      t->size *= 8;
    }
    MustExpect(TOKEN_CLOSE_S_PAREN);    
    return t;
  }

  return type;
}
TypeField* parse_typefield(){
  assert(token.kind == TOKEN_NAME);
  TypeField* tf = new TypeField;
  tf->name = consume().name;
  MustExpect(TOKEN_DOUBLE_DOT);  
  tf->type = parse_type();
  return tf;
}

inline Type* unsolved_type(){
  Type* t = new Type;
  t->kind  = TYPE_UNSOLVED;
  return t;
}
inline Type* type_from_typespec_kind(TypeFieldKind kd){
  Type* t = new Type;
  t->kind = kd;
  return t;
}
Var*  parse_var_def(){
  assert(token.kind == TOKEN_NAME);
  Var* v = new Var;
  v->type_field = parse_typefield();
  v->expr = NULL; 
  v->offset = calc_offset(v->type_field->type->size);
  if(expect_token(TOKEN_EQ)){
    v->expr = parse_expr();
  }  
  MustExpect(TOKEN_DOT_AND_COMMA);
  return v;
}
Decl* parse_global_var(){
  Decl* gvar = new Decl;
  gvar->kind = DeclKind::DECL_VAR;
  gvar->as.varDecl = *parse_var_def();
  gvar->name = gvar->as.varDecl.type_field->name;
  return gvar;
}
Elif* parse_elif_nodes(){
  Elif*      elif = new Elif;
  Expr**     nodes_expr = NULL;
  StmtList** nodes_block = NULL;
  
  elif->nodes_size = 0;  
  do{
    buf__push(nodes_expr, (parse_expr()));
    buf__push(nodes_block, (parse_block()));
    elif->nodes_size++;
  } while(expect_name(ELIF_KEYWORD)); 
  elif->node_expr  = nodes_expr;
  elif->node_block = nodes_block;
  return elif;
}
Stmt* parse_if(){
  Stmt* _if = new Stmt;
  _if->kind = STMTKIND_IF;
  _if->as.__if.expr   = parse_expr();
  _if->as.__if.block = parse_block();
  if(expect_name(ELIF_KEYWORD)){
    _if->as.__if.elif_nodes = parse_elif_nodes();
  }

  if(expect_name(ELSE_KEYWORD)){
    _if->as.__if.else_block = parse_block();
  }
  return _if;
}
Stmt* parse_while(){  
  assert(token.kind == TOKEN_NAME);
  next_token();
  Stmt* whl = new Stmt;
  whl->kind = STMTKIND_WHILE;
  whl->as.__while.expr  = parse_expr();
  whl->as.__while.block = parse_block();
  assert(whl->as.__while.block != NULL);
  return whl;  
}
switchList* parse_switch_block(){
  MustExpect(TOKEN_OPEN_C_PAREN);
  switchList* swtch = new switchList;
  swtch->nodes_size = 0;
  Expr**     cond   = NULL;
  StmtList** nodes  = NULL;
  while(!is_token(TOKEN_CLOSE_C_PAREN)){
    if(expect_name("case")){
      buf__push(cond, (parse_expr()));
      buf__push(nodes, (parse_block()));
      swtch->nodes_size++;
    }
    else {
      fatal("unexpected statement inside switch block.\n");
    }
  }
  MustExpect(TOKEN_CLOSE_C_PAREN);
  swtch->cond  = cond;
  swtch->nodes = nodes;
  return swtch;
}
Stmt* parse_switch(){
  assert(token.kind == TOKEN_NAME);
  next_token();
  Stmt* _switch = new Stmt;
  _switch->kind = STMTKIND_SWITCH;
  //switch 'a'
  _switch->as.__switch.cond = parse_expr();
  _switch->as.__switch.node = parse_switch_block();
  return _switch;
}
Stmt* parse_stmt(){
  Stmt* s = new Stmt;
  if(expect_name(RETURN_KEYWORD)){
    s->kind = STMTKIND_RETURN;
    s->as.expr = NULL;
    if(!is_token(TOKEN_DOT_AND_COMMA)){
      s->as.expr = parse_expr();
    }
    MustExpect(TOKEN_DOT_AND_COMMA);
  }
  else if(expect_name(BREAK_KEYWORD)){
    s->kind = STMTKIND_BREAK;
    MustExpect(TOKEN_DOT_AND_COMMA);
  }
  else if(expect_name(CONTINUE_KEYWORD)){
    s->kind = STMTKIND_CONTINUE;
    MustExpect(TOKEN_DOT_AND_COMMA);
  }
  else if(is_token(TOKEN_OPEN_C_PAREN)){
    s->kind     = STMTKIND_BLOCK;
    s->as.stmts = parse_block();
  }
  else if(expect_name(IF_KEYWORD)){
    s = parse_if();
  }
  else if(expect_name(VAR_KEYWORD)){
    s->kind = STMTKIND_LOCAL_VAR;
    s->as.var = parse_var_def();
    current_proc_reserve_memory(s->as.var);    
  }
  else if(token_is_name(WHILE_KEYWORD)){
    s = parse_while();
  }
  else if(expect_name(DO_KEYWORD)){
    printf("UNIMPLEMENTED: do\n");
    exit(1);
  }
  else if(token_is_name(SWITCH_KEYWORD)){
    s = parse_switch();		  
  }  
  else{
    s->kind    = STMTKIND_EXPR;
    s->as.expr = parse_expr();
    MustExpect(TOKEN_DOT_AND_COMMA);
  }
  return s;
}
StmtList* parse_block(){

  StmtList* sl   = new StmtList;
  Stmt** block   = NULL;
  sl->stmts_size = 0;
  if(expect_token(TOKEN_OPEN_C_PAREN)){
    while(!is_token(TOKEN_CLOSE_C_PAREN)){
      buf__push(block, (parse_stmt()));
      sl->stmts_size++;
    }
    sl->stmts = block;
    MustExpect(TOKEN_CLOSE_C_PAREN);
    return sl;
  }
  buf__push(block, (parse_stmt()));
  sl->stmts_size = 1;
  return sl;
}
TypeField*  parse_proc_return(){
  MustExpect(TOKEN_DOUBLE_DOT);
  TypeField* t = new TypeField;
  t->type = parse_type();
  return t;
}
ProcArgs* parse_proc_args(){
  MustExpect(TOKEN_OPEN_R_PAREN);
  ProcArgs* t = new ProcArgs;
  t->vars_size = 0;
  Var** vars = NULL;
 
  while(!is_token(TOKEN_CLOSE_R_PAREN)){
    if(buf__len(vars) > 0){
      MustExpect(TOKEN_COMMA);
    }
    TypeField* ts = parse_typefield();    
    Var* var = new Var{
      .type_field = new TypeField{
	.name = ts->name,
	.type = ts->type
      },
      .offset          = calc_offset(ts->type->size),
      .expr            = NULL
    };
    if(expect_token(TOKEN_EQ)){
      var->expr = parse_expr();
    }
    buf__push(vars,var);  
    current_proc_reserve_memory(var);
  }
  MustExpect(TOKEN_CLOSE_R_PAREN);
  t->vars = vars;
  t->vars_size = buf__len(vars);
  return t;
}
Decl* parse_proc_def(){
  Decl* proc = new Decl;
  current_proc = &proc->as.procDecl;
  proc->kind = DeclKind::DECL_PROC;  
  proc->name = consume().name;
  proc->as.procDecl.stack_allocation = 0;
  proc->as.procDecl.name = proc->name;
  proc->as.procDecl.args = parse_proc_args();  
  // TODO: make a uncomplete type
  proc->as.procDecl.ret_type = Type_none();
  if(expect_token(TOKEN_DOUBLE_DOT)){    
    proc->as.procDecl.ret_type = parse_type();
  }
  if(is_token(TOKEN_EQ)){
    next_token();
    Stmt* stmt = parse_stmt();
    proc->as.procDecl.block = new StmtList;
    proc->as.procDecl.block->stmts = NULL;
    buf__push(proc->as.procDecl.block->stmts, stmt);
    proc->as.procDecl.block->stmts_size = buf__len(proc->as.procDecl.block->stmts);
  }
  else if (is_token(TOKEN_OPEN_C_PAREN)){
    proc->as.procDecl.block = parse_block();
  }
  else if (expect_token(TOKEN_DOT_AND_COMMA)){
    proc->as.procDecl.block = NULL;
  }
  else {
    fprintf(stderr, "ERROR: could not parse the procedure.\n");
    exit(1);
  }
  current_proc = NULL;
  
  return proc;
}

Decl* parse_typedef(){
  // Syntax: type <name> :: <type>
  const char* name = consume().name;
  MustExpect(TOKEN_ACCESS_FIELD);
  Type* equivalent = parse_type();
  Decl* Typedef = new Decl;
  Typedef->kind = DeclKind::DECL_TYPEDEF;
  Typedef->name = name;
  Typedef->as.type_def.name	= name;
  
  Typedef->as.type_def.type	= equivalent;
  return Typedef;
}
EnumFields* parse_enum_fields(){
  MustExpect(TOKEN_OPEN_C_PAREN);
  EnumFields* efs = new EnumFields;
  size_t offset     = 0;
  while(!is_token(TOKEN_CLOSE_C_PAREN)){
    if(offset > 0){
      MustExpect(TOKEN_COMMA);
    }
    EnumField* ef = new EnumField;
    ef->name      = consume().name;
    ef->offset    = offset++;
    if(expect_token(TOKEN_EQ)){
      ef->expr = parse_expr();
    }
    buf__push(efs->fields, ef);
  }
  MustExpect(TOKEN_CLOSE_C_PAREN);
  return efs;
}
Decl* parse_enum(){
  assert(token.kind == TOKEN_NAME);
  Decl* Enum = new Decl;
  Enum->kind = DECL_ENUM;
  Enum->name = consume().name;
  Enum->as.Enum.fields = parse_enum_fields();
  return Enum;
}
Decl* parse_namespace(){
  MustExpectName(NAMESPACE_KEYWORD);
  Decl* ns = new Decl;
  ns->as.Namespace.decls = NULL;
  ns->kind = DECL_NAMESPACE;
  ns->name = consume().name;
  ns->as.Namespace.name = ns->name;
  MustExpect(TOKEN_OPEN_C_PAREN);
  while(!is_token(TOKEN_CLOSE_C_PAREN)){
    buf__push(ns->as.Namespace.decls, parse_decl());
  }
  MustExpect(TOKEN_CLOSE_C_PAREN);  
  return ns;
}
Decl* parse_import(){
  MustExpectName(INCLUDE_KEYWORD);

  const char* fp = consume().name;
  file = fp;
  if( Included_fp_find(fp) ) return NULL;
  Included_fp_push(fp);
  FILE* f = fopen(fp, "r");
  if(!f){
    fprintf(stderr,
	    "ERROR: Não foi possivel encontrar o caminho para incluir o arquivo: '%s'\n",
	    file);
    exit(1);
  }
  Decl** ast = parse_file(f);
  Decl* node = new Decl;

  node->kind = DECL_IMPORT;
  node->as.Import.path = fp;
  node->name = node->as.Import.path;
  node->as.Import.ast = ast;
  return node;
  
}
Decl* parse_decl(){
  if(expect_token(TOKEN_AT_SIGN)){
    fatal("Notes for procedures are not implemented yet.\n");
    exit(1);
  }
  
  if(expect_name(VAR_KEYWORD)){
    return parse_global_var();
  }
  else if(expect_name(PROC_KEYWORD)){
    return parse_proc_def();
  }
  
  else if(expect_name(TYPEDEF_KEYWORD)){    
    return parse_typedef();
  }
  else if(expect_name(ENUM_KEYWORD)){    
    return parse_enum();
  }
  else if(expect_name(UNION_KEYWORD)){
    fatal("parsing unions are not implemented yet\n");
    exit(1);
  }
  else if (token_is_name(NAMESPACE_KEYWORD)){
    return parse_namespace();
  }
  else if(is_token(TOKEN_NAME)){
    const char* tk_name = consume().name;

    if(expect_token(TokenKind::TOKEN_ACCESS_FIELD)){
      Expr* macro_body = parse_expr();
      Macro macro = {
	.name = tk_name,
	.expr = macro_body
      };
      GFL_Macros_push(macro);
      return NULL;
    }
    else {
      syntax_error("unexpected global declaration `%s`\n", tk_name);
      exit(1);
    }
  }
  else {
    fatal("unexpected token in the global scope: '%s: %s'\n", human_readable_token(token));
    exit(1);
  }
}

 
AST_ROOT parser_run_code(const char* src){
  init_stream(src);
  AST_ROOT ast = NULL;
  while(*stream){
    if(expect_token(TOKEN_HASHTAG)){
      if(token_is_name(INCLUDE_KEYWORD)){
	const char* last_str = stream;
	const char* old_file = file;
	Decl* node = parse_import();
	file = old_file;
	if(node){
	  init_stream(last_str);
	  next_token();
	  buf__push(ast, node);
	}
	continue;
      }
      else {
	printf("UNEXPECTED PRE-PROCESSING.\n");
	exit(1);
      }
    }
    else {
      calc_offset(0, true); // reset local memory offset
      Decl* node = parse_decl();
      if(node){
	if (node->kind == DECL_NAMESPACE) namespaces_push(&namespaces, node);	
	else buf__push(ast, node);	
      }            
    }
  
  }
  return ast;
}
extern const char*  program;
Decl** parse_file(FILE* f){
  char* str = NULL;
  char c;
  do{
    c = (char)fgetc(f);
    buf__push(str, c);
  } while (c != EOF);
  return parser_run_code(str);
}
Decl** parse_from_file(const char* file_path){
  FILE* f = fopen(file_path, "r");
  assert(f);
  return parse_file(f);
}

#endif /* __parser */

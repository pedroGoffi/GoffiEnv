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
Expr* parse_plus_minus();
Expr* parse_e0();
Expr* parse_e1();
Expr* parse_e2();
Expr* parse_expr();
TypeSpec* parse_typespec();
Type* parse_type();
TypeField* parse_typefield();
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
TypeSpec*  parse_proc_return();

Decl* parse_proc_def();
Decl* parse_struct_def();
Decl* parse_typedef();
Decl* parse_cimport();
Elif* parse_elif_nodes();
Stmt* parse_if();
void parser_run();
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
  if(is_token(TokenKind::TOKEN_INT)){
    e->kind = EXPRKIND_INT;
    e->as.INT = consume().INT;
    return e;
  }
  else if(is_token(TokenKind::TOKEN_STRING)){
    e->kind = EXPRKIND_STRING_LITERAL;
    e->as.STRING = consume().name;
    return e;
  } 
  else if(expect_token(TOKEN_OPEN_R_PAREN)){
    if(expect_token(TOKEN_DOUBLE_DOT)){
      // TODO: this cast parsing does allocate good memory, fix this
      // Syntax: (:int)(expr)
      TypeSpec* casting_to = new TypeSpec;
      casting_to->type = parse_type();
      e->kind = EXPRKIND_CAST;
      e->as.Cast.type = casting_to;
      MustExpect(TOKEN_CLOSE_R_PAREN);
      e->as.Cast.expr = parse_expr();
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
      e->as.Call.p_name = NAME;
      next_token();
      Expr **args = NULL;
      e->as.Call.args_size = 0; 
      while(!is_token(TOKEN_CLOSE_R_PAREN)){
	if(e->as.Call.args_size > 0){
	  MustExpect(TOKEN_COMMA);
	}
	buf__push(args, (parse_expr()));
        e->as.Call.args_size++;
      }      
      e->as.Call.args = args;
      MustExpect(TOKEN_CLOSE_R_PAREN);
      return e;
    }
    else if(expect_token(TOKEN_DOT)){      
      e->kind = EXPRKIND_FIELD_ACCESS;
      e->as.FieldAccess.fields_size = 0;
      e->as.FieldAccess.source = NAME;
      const char** fields = NULL;
      buf__push(fields, (consume().name));      
      while(expect_token(TOKEN_DOT)){
	printf("%s.", token.name);
	buf__push(fields, (consume().name));
	e->as.FieldAccess.fields_size++;	
      }
      e->as.FieldAccess.fields = fields;
      return e;
    }
    else if(expect_token(TOKEN_OPEN_S_PAREN)){
      e->kind = EXPRKIND_ARRAY_ACCESS;
      e->as.ArrayAccess.name  = NAME;
      e->as.ArrayAccess.expr  = parse_expr();
      MustExpect(TOKEN_CLOSE_S_PAREN);
      return e;
    }

    e->kind = EXPRKIND_NAME;
    e->name = NAME;
    return e;
  }
  fatal("Unexpected token: `%s` when trying to parse a number.\n",
	human_readable_token(token.kind));
  exit(1);
}
Expr* parse_e1(){
  Expr *e = new Expr;
  if(expect_token(TOKEN_TAKEAWAY)){
    e = parse_e0();
    e->as.INT   *= -1;
    e->as.FLOAT *= -1;    
  }
  else if(expect_token(TOKEN_BANG)){
    e = parse_e0();
    printf("parsing NOT operator is not imeplemented yet\n");
    exit(1);
  }
  else if(expect_token(TOKEN_STAR)){
    e->kind      = EXPRKIND_DERREF_NAME;    
    e->as.derref = parse_expr();
  }
  else if(expect_token(TOKEN_AMPERSAND)){
    e->kind =  EXPRKIND_ADDROF_NAME;
    e->as.addr_of = parse_expr();
  }
  else{  
    e = parse_e0();
  }
  return e;
}
Expr* parse_e2(){
  Expr* elhs = new Expr;
  elhs = parse_e1();
  while(is_token(TOKEN_STAR) || is_token(TOKEN_DIV)){
    EXPR_BINARY_OP_KIND op = make_expr_binary_op_kind();
    next_token();
    Expr *erhs = new Expr;
    erhs = parse_e1();
    elhs = expr_make_binary(elhs, erhs, op);
  }
  return elhs;
}

Expr* parse_logic(){
  Expr* e = new Expr;
  e = parse_e2();
  if(expect_token(TOKEN_EQ)){
    e->as.Reasign.from = expr_make_name(e);
    e->kind = EXPRKIND_REASIGN;
    e->as.Reasign.to = parse_expr();          
  }
  return e;
}
Expr* parse_plus_minus(){
  Expr* elhs = new Expr;
  elhs = parse_logic();
  while(is_token(TOKEN_PLUS) || is_token(TOKEN_TAKEAWAY)){
    EXPR_BINARY_OP_KIND op = make_expr_binary_op_kind();
    next_token();
    Expr* erhs = new Expr;
    erhs = parse_logic();
    elhs = expr_make_binary(elhs, erhs, op);
  }
  return elhs;
}

Expr* parse_expr(){
  Expr* elhs = new Expr;
  elhs = parse_plus_minus();
  while(is_token(TOKEN_CMP_EQ)
	|| is_token(TOKEN_CMP_NEQ)
	|| is_token(TOKEN_LESS)
	|| is_token(TOKEN_GREATER)
	){
    EXPR_CMP_KIND op = make_cmp_kind();
    next_token();    
    
    Expr* erhs = new Expr;
    erhs = parse_plus_minus();
    elhs = expr_make_cmp(elhs, erhs, op);
  }
  return elhs;  
}

Type* parse_type(){
  Type* type = new Type;

  if(expect_token(TOKEN_STAR)){
    type->kind     = TYPE_PTR;
    type->ptr.base = parse_type();
    return type;
  }
  assert(token.kind == TOKEN_NAME);
  type->kind = TypeSpecKind_by_cstr(token.name);
  type->name = token.name;
  next_token();
  if(expect_token(TOKEN_OPEN_S_PAREN)){
    Type* t = new Type;
    t->kind = TYPE_ARRAY;
    t->array.base = type;
    t->array.size = nullptr;
    if(!is_token(TOKEN_CLOSE_S_PAREN)){
      t->array.size = parse_expr();
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
TypeSpec* parse_typespec(){
  assert(token.kind == TOKEN_NAME);
  TypeSpec* ts = new TypeSpec; 
  ts->name = consume().name;
  MustExpect(TOKEN_DOUBLE_DOT);
  ts->type = parse_type();  
  return ts;
}
inline Type* unsolved_type(){
  Type* t = new Type;
  t->kind  = TYPE_UNSOLVED;
  return t;
}
inline Type* type_from_typespec_kind(TypeSpecKind kd){
  Type* t = new Type;
  t->kind = kd;
  return t;
}
Var*  parse_var_def(){
  assert(token.kind == TOKEN_NAME);
  Var* v = new Var;
  v->type_field = parse_typefield();
  v->expr = NULL;
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
  } while(expect_name("elif")); 
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
    s->as.expr = parse_expr();
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
    printf("UNIMPLEMENTED: nested scopes\n");
    exit(1);
  }
  else if(expect_name(IF_KEYWORD)){
    s = parse_if();
  }
  else if(expect_name(VAR_KEYWORD)){
    s->kind = STMTKIND_LOCAL_VAR;
    s->as.var = parse_var_def();
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
  MustExpect(TOKEN_OPEN_C_PAREN);
  StmtList* sl   = new StmtList;
  Stmt** block   = NULL;
  sl->stmts_size = 0;

  while(!is_token(TOKEN_CLOSE_C_PAREN)){
    buf__push(block, (parse_stmt()));
    sl->stmts_size++;
  }
  sl->stmts = block;
  MustExpect(TOKEN_CLOSE_C_PAREN);
  return sl;  
}
TypeSpec*  parse_proc_return(){
  MustExpect(TOKEN_DOUBLE_DOT);
  TypeSpec* t = new TypeSpec;
  t->type = parse_type();
  return t;
}
ProcArgs* parse_proc_args(){
  MustExpect(TOKEN_OPEN_R_PAREN);
  ProcArgs* t = new ProcArgs;
  t->argsList_size = 0;
  TypeSpec** al = NULL; 
  while(!is_token(TOKEN_CLOSE_R_PAREN)){
    if(t->argsList_size > 0){
      MustExpect(TOKEN_COMMA);
    }
    buf__push(al, (parse_typespec()));
    t->argsList_size++;
  }
  MustExpect(TOKEN_CLOSE_R_PAREN);
  t->argsList = al;
  return t;
}
Decl* parse_proc_def(){
  Decl* proc = new Decl;
  proc->kind = DeclKind::DECL_PROC;
  proc->name = consume().name;
  proc->as.procDecl.args = parse_proc_args();
  proc->as.procDecl.ret_type = NULL;
  // TODO: make a uncomplete type
  if(is_token(TOKEN_DOUBLE_DOT)){
    next_token();    
    proc->as.procDecl.ret_type = parse_type();
  }
  if(is_token(TOKEN_EQ)){
    syntax_error("`=` notation in procedures is not implemented yet\n");
    exit(1);    
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
  return proc;
}

Decl* parse_struct_def(){
  Decl          *dec = new Decl;
  dec->kind = DeclKind::DECL_STRUCT;
  dec->name = consume().name;
  dec->as.structDecl.fields_size = 0;
  MustExpect(TOKEN_OPEN_C_PAREN);
  TypeSpec **fields = NULL;
  while(!is_token(TOKEN_CLOSE_C_PAREN)){
    buf__push(fields, (parse_typespec()));
    MustExpect(TOKEN_DOT_AND_COMMA);
    dec->as.structDecl.fields_size++;    
  }
  MustExpect(TOKEN_CLOSE_C_PAREN);
  dec->as.structDecl.fields = fields;
  return dec;
}
Decl* parse_cimport(){
  assert(token.kind == TOKEN_NAME);
  next_token();
  Decl* cimport = new Decl;
  cimport->kind = DeclKind::DECL_CIMPORT;
  if(expect_token(TOKEN_LESS)){
    assert(token.kind == TOKEN_NAME);
    cimport->as.cimportDecl.FILENAME = consume().name;
    cimport->as.cimportDecl.isStd = true;
    MustExpect(TOKEN_GREATER);
  } else if (expect_token(TOKEN_STRING)){
    printf("peeking local path is not implemented\n");
    exit(1);
  }
  else {
    syntax_error("#cimport expects \"FILENAME\" or <FILENAME>\n");
    exit(1);
  }
  return cimport;
}
Decl* parse_typedef(){
  Type* type       = parse_type();
  Type* equivalent = parse_type();
  MustExpect(TOKEN_DOT_AND_COMMA);
  Decl* Typedef = new Decl;
  Typedef->kind = DeclKind::DECL_TYPEDEF;
  Typedef->as.Typedef.type       = type;
  Typedef->as.Typedef.type_equivalent = equivalent;
  return Typedef;
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

  else if(expect_name(STRUCT_KEYWORD)){    
    return parse_struct_def();
  }
  else if(expect_name(TYPEDEF_KEYWORD)){    
    return parse_typedef();
  }
  else if(expect_name(ENUM_KEYWORD)){
    fatal("parsing enums are not implemented yet\n");
    exit(1);    
  }
  else if(expect_name(UNION_KEYWORD)){
    fatal("parsing unions are not implemented yet\n");
    exit(1);
  }
  else if(expect_token(TOKEN_HASHTAG)){
    if(token_is_name("cimport")){      
      return parse_cimport();
    }
    else if(token_is_name("import")){
      printf("includes are not implemented yet\n");
      exit(1);
    }
    syntax_error("unexpected compiler time procedure\n");
    exit(1);
  }  
  else if(is_token(TOKEN_NAME)){
    const char* tk_name = consume().name;
    if(expect_token(TOKEN_DOUBLE_DOT)){
      syntax_error("unimplementedcompiler time constants\n");
      exit(1);      
    }
    else {
      syntax_error("unexpected global declaration `%s`\n", tk_name);
      exit(1);
    }
  }
  else {
    fatal("unexpected token in the global scope starts witch: '%c': '%s'\n", *stream, stream);
    exit(1);
  }
}
void parser_run(){
  // TODO: compiler time constants
  // NOTE: SYNTAX IS
  //    CTE :: 10
  //    CTE :: "name"
  // NOTE: if the syntax if too noisy make sure it will be easy to change in the code
  // TODO: lambda expressions
  // NOTE: SYNTAX IS
  //    (args) -> return_type => {} would be a cool like javascript
  
  const char*  src = {
    "var k: bufHdr* = 420\n"
    // TODO: add support for more top global names
    //"const k: bufHdr* = 420",
    //"union fooUNION{"
    //"  coolFoo,"
    //"  badFoo"
    //"}",
    "struct foo{\n"
    "  name: int\n"
    "  name: int\n"
    "  name: int\n"
    "}\n"
    "struct bar{\n"
    "  age: bufHdr*\n"
    "}\n"
    "proc main(argc:int, argv: char**) -> void {\n"
    "  if 1+1+1 {\n"
    "    return 420\n"
    "    return 420\n"
    "  }\n"
    "  elif (:int)(420+10) {\n"
    "    return 420+10+1\n"
    "  }\n"
    "  else {\n"
    "    return 1+(1+1*2)\n"
    "  }\n"
    "}\n"
  };
  // NOTE: make structs typedef by default, because i like this way.
  //for(auto& it: src){
  init_stream(src);
  while(*stream){

    Decl* d = parse_decl(); 
    print_decl(d);
    printf("\n");   
  }
}
AST_ROOT parser_run_code(const char* src){
  init_stream(src);
  AST_ROOT ast = NULL;
  while(*stream){
    Decl* node = parse_decl();
    buf__push(ast, node);
  }
  return ast;
}
#endif /* __parser */

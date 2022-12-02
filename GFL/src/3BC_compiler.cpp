#ifndef __3bc_assembler
#define __3bc_assembler
#include "./ast.cpp"
#include "./compiler.cpp"
#define __REG(n) "___pseudo_register" #n "___"
#define REG1 __REG(1)
#define REG2 __REG(2)

#define STRING_MODE println("\tMODE NILL 0d2 # String mode")
#define CALL_MODE   println("\tMODE NILL d42 # Call mode")

#define LABEL_3bc(ln) println("NILL NILL %s:", ln)
#define RET_3bc       \
  println("\tMODE NILL d41");\
  println("\tBACK NILL NILL # return");

static void expr_3bc(Expr* e);
static bool try_make_builtin_3bc(Call* c){
  const char* pname = c->p_name;
#define DOIF(str, ...)				\
  if(STR_CMP(c->p_name, (str))) {		\
    {__VA_ARGS__}				\
    println("\t# Built-in gfl -> 3bc %s", str); \
    return true;				\
  }
  //

  DOIF("__set_acc", {    
      Expr* arg = c->args[0];
      assert(arg);
      assert(arg->kind == EXPRKIND_INT);
      println("\tMODE NILL 8");
      println("\tALLOC 0   %i", arg->as.INT);
    });
  DOIF("__writestr__", {
      Expr** args = c->args;
      for(size_t i=0; i < buf__len(args); ++i){
	Expr* arg = args[i];
	assert(arg->kind == EXPRKIND_STRING_LITERAL);
	const char* str = arg->as.STRING;
	STRING_MODE;    
	for(size_t i=0; i + 1< buf__len(str); ++i){
	  println("\tSTRC 5 %zu", str[i]);
	}
      }
    });
#undef DOIF
  return false;
}
static void expr_3bc(Expr* e){
  switch(e->kind){
  case EXPRKIND_INT:
    println("\tMODE  NILL 8");
    println("\tALLOC NILL %i", e->as.INT);
    break;
  case EXPRKIND_STRING_LITERAL: {
    assert(false && "UNIMPLEMENYED STRINGS");    
  } break;
  case EXPRKIND_NAME: {
    if(Var* global_var = Var_get(&global_vars, e->name)){
      assert(global_var);
      println("\tMODE NILL 6");;
      println("\tPULL :%s NILL", global_var->type_field->name);
    } else {
      fprintf(stderr,
	      "ERROR: 3bc the name '%s' was not declared in this scope.\n",
	      e->name);
      exit(1);
    }
  } break;
  case EXPRKIND_REASIGN: {    
    expr_3bc(e->as.Reasign.to);
    expr_3bc(e->as.Reasign.from);
  } break;
  case EXPRKIND_PROC_CALL: {
    if(try_make_builtin_3bc(&e->as.call)) return;
    fprintf(stderr,
	    "ERROR: 3bc procedure call unreachable.\n");
    exit(1);
  } break;
  default:
    print_expr(e);
    fprintf(stderr,
	    "ERROR: 3bc unreachable expr.\n");
    exit(1);
  }
}
void stmt_3bc(Stmt* st){  
  switch(st->kind){
  case STMTKIND_EXPR:
    expr_3bc(st->as.expr);
    break;
  case STMTKIND_RETURN:
    RET_3bc;
    if(st->as.expr){
      expr_3bc(st->as.expr);
    }
    break;
  case STMTKIND_LOCAL_VAR:
    fprintf(stderr,
	    "ERROR: 3bc does not support local variables.\n");
    exit(1);
  default:
    print_stmt(st);
    fprintf(stderr,
	    "ERROR: 3bc unreachable stmt.\n");
    exit(1);
  }
}
void block_3bc(StmtList* block){
  for(size_t i=0; i< buf__len(block->stmts); ++i){
    stmt_3bc(block->stmts[i]);
  }
}
void decl_3bc(Decl* d){
  switch(d->kind){
  case DECL_PROC:
    LABEL_3bc(d->as.procDecl.name);
    block_3bc(d->as.procDecl.block);
    RET_3bc;
    break;
  case DECL_VAR:
    Var_push(&global_vars, &d->as.varDecl);
    break;
  default:
    print_decl(d);
    fprintf(stderr,
	    "ERROR: 3bc unreachable decl.\n");
    exit(1);
  }
}
void init_pseudo_regs(){
}
void entry_3bc(){
  LABEL_3bc("_start");
  for(size_t i=0; i < buf__len(global_vars); ++i){
    Var* gvar = global_vars[i];;
    if(gvar->expr){      
      fprintf(stderr,
	      "ERROR: 3bc global var asign not implemented yet.\n");
      exit(1);
    }
  }
  init_pseudo_regs();
  CALL_MODE;  
  println("\tCALL NILL :main");
}
void assembly_3bc_ast(Decl** ast, const char* output_fp){  
  output_file = fopen(output_fp, "wb");
  assert(output_file);
  println("\tMODE NILL 0d9");
  println("\tGOTO NILL :_start");
  for(size_t i=0; i< buf__len(ast); ++i){
    decl_3bc(ast[i]);
  }
  entry_3bc();
  fclose(output_file);  
}

#endif /* __3bc_assembler */

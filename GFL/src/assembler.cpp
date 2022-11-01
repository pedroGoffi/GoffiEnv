#ifndef __assembler
#define __assembler
#include "./ast.cpp"
#include "../../GFSL2/src/gfsl.cpp"
#define append_proc_inst(PROC, inst)\
  (PROC)->body[(PROC)->body_size++] = (inst);
#define append_new_inst(PROC, Type, Operand) \
  {					     \
  Inst i = {Type, Operand};		     \
  append_proc_inst(PROC, i);		     \
  }

void assembly_proc_stmt(PROC* proc, Stmt* stmt);

struct COMPILED_EXPR{  
  Type ckind;
};


Var** declared_global_vars = NULL;
Var** declared_local_vars  = NULL;

Var* Global_Var_find(const char* name){
  printf("var find.\n");
  for(size_t i=0; i < buf__len(declared_global_vars); ++i){
    Var* var = declared_global_vars[i];
    printf("tring to find %s == %s.\n", name, var->type_field->name);
    if(STR_CMP(var->type_field->name, name))
      return var;
  }
  return NULL;
}
Var* Local_Var_find(const char* name){
  printf("var find.\n");
  for(size_t i=0; i < buf__len(declared_local_vars); ++i){
    Var* var = declared_local_vars[i];
    printf("tring to find %s == %s.\n", name, var->type_field->name);
    if(STR_CMP(var->type_field->name, name))
      return var;
  }
  return NULL;
}

void Global_Var_push(Var* src){
  printf("pushing %s\n", src->type_field->name);
  
  Var* item = new Var{
    .type_field = src->type_field,
    .expr       = src->expr
  };

  buf__push(declared_global_vars, item);
}
void Local_Var_push(Var* src){
  printf("pushing %s\n", src->type_field->name);
  
  Var* item = new Var{
    .type_field = src->type_field,
    .expr       = src->expr
  };

  buf__push(declared_local_vars, item);
}
size_t get_size_of_type(Type* t){
  const char* name = t->name;
  switch(t->kind){
  case TypeKind::TYPE_I64: return 8;
  case TypeKind::TYPE_PTR: return 8;
  case TypeKind::TYPE_NONE: 
  case TypeKind::TYPE_F64: 
  case TypeKind::TYPE_CHAR: 
  case TypeKind::TYPE_ARRAY: 
  case TypeKind::TYPE_STRUCT: 
  case TypeKind::TYPE_ENUM: 
  case TypeKind::TYPE_PROC: 
  case TypeKind::TYPE_UNSOLVED:
    printf("TODO: get size of type unresolved.\n");
    exit(1);
  }
  {
    fprintf(stderr,
	    "ERROR: undefined type '%s'.\n", name);
    exit(1);
  }
}
void assembly_proc_body(PROC* proc, StmtList* block);
inline bool expect_type(TypeKind got, TypeKind expected){
  return (got == expected);
}
COMPILED_EXPR assembly_proc_expr(PROC* proc, Expr* expr){
  COMPILED_EXPR cexpr = {};
  Inst inst = {};
  switch(expr->kind){
  case EXPRKIND_INT:
    inst.type    = Op_type::PUSH_INT;
    inst.operand = expr->as.INT;
    append_proc_inst(proc, inst);
    break;
  case EXPRKIND_STRING_LITERAL: {
    char* allocated_str = NULL;
    // NOTE: this is used to strip the quotes from the string
    {
      for(size_t i=1; expr->as.STRING[i] != '"'; ++i){
	buf__push(allocated_str, expr->as.STRING[i]);
      }
    }

    size_t dataLoc = reserve_string_in_data(allocated_str);
    append_new_inst(proc, Op_type::PUSH_CSTR, dataLoc);
    buf__free(allocated_str);
  } break;
  case EXPRKIND_NAME: {
    const char* vname = expr->name;
    int gvar_pos = STATICS_pos(vname);
    if(gvar_pos != -1){
      {
	Var* gvar    = Global_Var_find(vname);
	assert(gvar);
	
	inst.type    = Op_type::PUSH_STATIC_PTR;
	inst.operand = gvar_pos;
	append_proc_inst(proc, inst);
	cexpr.ckind.kind  = gvar->type_field->type->kind;
      }
    }
    else if (PROC_is_local(vname)){
      {

	Var* lvar    = Local_Var_find(vname);
	assert(lvar);
	append_new_inst(proc,
			Op_type::PUSH_LOCAL_PTR,
			PROC_get_local_offset(vname));
	cexpr.ckind.kind  = lvar->type_field->type->kind;
      }
    }
    else if (GFL_MACROS_exists(vname)){
      Macro* macro = GFL_Macros_find(vname);
      assembly_proc_stmt(proc, macro->stmt);
    }
    else {      
      fprintf(stderr,
	      "ERROR: the variable '%s' is not declared in this scope.\n",
	      expr->name);
      exit(1);
    }
    if(!expect_type(cexpr.ckind.kind, TYPE_PTR)){
      append_new_inst(proc, Op_type::LOAD64, 0);
    }
  } break;
  case EXPRKIND_DERREF_NAME: {
    assembly_proc_expr(proc, expr->as.derref);
    append_new_inst(proc, Op_type::LOAD64, 0);
    // TODO: cexpr.ckind.kind = CEXPR_derref_type(expr->as.derref);
  } break;
  case EXPRKIND_REASIGN: {
    const char* vname = expr->name;
    int gvar_pos = STATICS_pos(vname);
    if(gvar_pos != -1){
      inst.type    = Op_type::PUSH_STATIC_PTR;
      inst.operand = gvar_pos;
    }
    else if (PROC_is_local(vname)){
      inst.type    = Op_type::PUSH_LOCAL_PTR;
      inst.operand = PROC_get_local_offset(vname);
    }
    else {
      fprintf(stderr,
	      "ERROR: variable '%s' is not declared in this scope.\n", vname);
      exit(1);
    }
    append_proc_inst(proc, inst);
    assembly_proc_expr(proc, expr->as.Reasign.to);
    {
      inst.type = Op_type::STORE64;
      append_proc_inst(proc, inst);
    }
    cexpr.ckind.kind = TYPE_NONE;
    //PUSH_STATIC_PTR
    //CASE(PUSH_LOCAL_PTR
  } break;
  case EXPR_BINARY_OP:
    assembly_proc_expr(proc, expr->as.BinaryOp.lhs);
    assembly_proc_expr(proc, expr->as.BinaryOp.rhs);
    switch(expr->as.BinaryOp.op){
    case OP_KIND_PLUS:
      inst.type = Op_type::PLUS;
      break;
    case OP_KIND_MINUS:
      inst.type = Op_type::MINUS;
      break;
    default:
      error_here("Unknown expr->as.BinaryOp.op.\n");
    }
    append_proc_inst(proc, inst);
    break;
  case EXPRKIND_PROC_CALL: {
    const char* pname = expr->as.Call.p_name;
    int call_arity = expr->as.Call.args_size;
    int ProcPos    = PROCS_pos(pname);
    if(ProcPos != -1){
      for(size_t i=0; i < expr->as.Call.args_size; ++i){
	assembly_proc_expr(proc, expr->as.Call.args[i]);
      }
      append_new_inst(proc, Op_type::CALL_PROC, ProcPos);
    }
    else if(STR_CMP(pname, "dump")){
      if(call_arity > 1){
	fprintf(stderr,
		"ERROR: the built-in procedure 'dump' expects one argument.\n");
	exit(1);
      }
      Expr* arg = expr->as.Call.args[0];
      assembly_proc_expr(proc, arg);
      {
	inst.type = Op_type::DUMP;
	append_proc_inst(proc, inst);
      }
    }
    else if (STR_CMP(pname, "SYSCALL")){

      if(call_arity < 2){
	fprintf(stderr,
		"ERROR: built-in 'SYSCALL' expects at least 2 arguments.\n");
	exit(1);
      }
      Expr* expr_sys_len = expr->as.Call.args[0];
      if(expr_sys_len->kind != EXPRKIND_INT){
	fprintf(stderr,
		"ERROR: SYSCAL first argument must be an unsigned integet.\n");
	exit(1);
      }
            
      int sys_len = expr_sys_len->as.INT + 1;
      int sys_al  = (int)buf__len(expr->as.Call.args) - 1;
      if(sys_len < 0){
	printf("ERROR: built-int procedure 'SYSCALL' expects a positive number in the first argument.\n");
	exit(1);
      }
      if(sys_len != sys_al){
	fprintf(stderr,
		"ERROR: expected '%i' arguments on the syscall but got '%ld'\n",
		sys_len,
		buf__len(expr->as.Call.args) - 1);
	exit(1);
      }      
      
      for(size_t i=1; i < buf__len(expr->as.Call.args); ++i){
	assembly_proc_expr(proc, expr->as.Call.args[i]);
      }
      
      {
	switch(sys_len - 1){
	case 0: 
	  append_new_inst(proc, Op_type::SYSCALL0, 0);
	  break;
	case 1: 
	  append_new_inst(proc, Op_type::SYSCALL1, 0);
	  break;
	case 2: 
	  append_new_inst(proc, Op_type::SYSCALL2, 0);
	  break;
	case 3: 
	  append_new_inst(proc, Op_type::SYSCALL3, 0);
	  break;
	case 4: 
	  append_new_inst(proc, Op_type::SYSCALL4, 0);
	  break;
	case 5: 
	  append_new_inst(proc, Op_type::SYSCALL5, 0);
	  break;
	case 6: 
	  append_new_inst(proc, Op_type::SYSCALL6, 0);
	  break;
	  
	default:
	  printf("WARNIGN: undefined assembly behavior for SYSCALL with '%i' num or arguments.\n", sys_len);
	  exit(1);
	}
      }
      cexpr.ckind.kind = TYPE_I64;
    }
    else {
      fprintf(stderr,
	      "ERROR: the procedure '%s' was not declared in this scope.\n",
	      pname);
      exit(1);
    }
    
  } break;
  default:
    error_here("Unknown expr->kind.\n");
  }
  return cexpr;
}
void assembly_proc_stmt(PROC* proc, Stmt* stmt){
  Inst inst;
  switch(stmt->kind){
  case STMTKIND_EXPR: {
    auto cexpr = assembly_proc_expr(proc, stmt->as.expr);
    if(cexpr.ckind.kind != TYPE_NONE){
      printf("found not kind none.\n");
      append_new_inst(proc, Op_type::DROP, 0);
    }
  } break;
  case STMTKIND_IF: {
    {
      inst.type = Op_type::IF;
      append_proc_inst(proc, inst);
    }
    assembly_proc_expr(proc, stmt->as.__while.expr);
    size_t do_pos = proc->body_size;
    {
      inst.type = Op_type::DO;
      append_proc_inst(proc, inst);
    }
    
    {
      assembly_proc_body(proc, stmt->as.__while.block);
    }
    Inst *Ido = &proc->body[do_pos];
    assert(Ido->type == Op_type::DO);    

    Ido->operand = proc->body_size;
    {
      inst.type    = Op_type::END;
      inst.operand = proc->body_size + 1;
      append_proc_inst(proc, inst);
    }
  } break;
  case STMTKIND_LOCAL_VAR: {
    LOCAL local = {};
    Var* var = stmt->as.var;
    Local_Var_push(var);
    
    assert(var);

    local.name = var->type_field->name;
    local.size = get_size_of_type(var->type_field->type);
    PROC_push_local(local);
    if(var->expr){
      append_new_inst(proc,
		      Op_type::PUSH_LOCAL_PTR,
		      PROC_get_local_offset(local.name));
      assembly_proc_expr(proc, stmt->as.var->expr);
      append_new_inst(proc,
		      Op_type::STORE64,
		      0);
    }
  } break;
  case STMTKIND_RETURN: {
    assembly_proc_expr(proc, stmt->as.expr);
    append_new_inst(proc,
		    Op_type::PROC_RETURN,
		    0);
  } break;
  default:    
    error_here("Unknown stmt->kind.\n");
  }
}
void assembly_proc_body(PROC* proc, StmtList* block){
  for(size_t i=0; i < block->stmts_size; i++){
    Stmt* stmt = block->stmts[i];
    assembly_proc_stmt(proc, stmt);
  }
}
void assembly_ast_into_gfsl(Decl** ast){
  for(size_t i=0; i < buf__len(ast); ++i){
    Decl* decl = ast[i];
    switch(decl->kind){
    case DeclKind::DECL_VAR: {

      Var var = decl->as.varDecl;
      Global_Var_push(&var);
      
      STATIC gvar = {};

      if(var.expr){
	fprintf(stderr,
		"ERROR: global var with expr is not implemented yet.\n");
	exit(1);
      }
      gvar.name = var.type_field->name;
      TypeKind kind = var.type_field->type->kind;
      if(kind == TYPE_I64){
	gvar.size = 8;
      }
      else if(kind == TYPE_PTR){
	gvar.size = 8;
      }
      else{ abort(); }
      
      STATICS_push(gvar);      
    } break;
    case DeclKind::DECL_PROC: {
      PROC proc = {};
      current_proc = &proc;
      proc.name = decl->name;
      if(decl->as.procDecl.args->argsList_size > 0){

	for(size_t i=0; i < decl->as.procDecl.args->argsList_size; ++i){
	  TypeSpec* ts = decl->as.procDecl.args->argsList[i];
	  LOCAL arg_local = {};
	  arg_local.name = ts->name;
	  arg_local.size     = get_size_of_type(ts->type);
	  PROC_push_local(arg_local);	  
	}
	if(STR_CMP(decl->name, "main")){
	  ProcArgs* args = decl->as.procDecl.args;
	  assert(args);
	  assert(args->argsList_size == 2);
	  size_t offset = PROC_get_local_offset(args->argsList[0]->name);
	  append_new_inst(&proc, Op_type::PUSH_LOCAL_PTR, offset);
	  append_new_inst(&proc, Op_type::ARGC, 0);
	  append_new_inst(&proc, Op_type::LOAD64, 0);
	  append_new_inst(&proc, Op_type::STORE64, 0);

	  offset = PROC_get_local_offset(args->argsList[1]->name);
	  append_new_inst(&proc, Op_type::PUSH_LOCAL_PTR, offset);
	  append_new_inst(&proc, Op_type::ARGV, 0);
	  //append_new_inst(&proc, Op_type::LOAD64, 0);	  
	  append_new_inst(&proc, Op_type::STORE64, 0);	  
	  
	}
	else {	
	  for(int i = (int)buf__len(decl->as.procDecl.args->argsList) - 1; i >= 0; --i){
	  
	    size_t offset = PROC_get_local_offset(decl->as.procDecl.args->argsList[i]->name);
	  
	  
	    append_new_inst(&proc, Op_type::PUSH_LOCAL_PTR, offset);
	    append_new_inst(&proc, Op_type::SWAP, 0);
	    append_new_inst(&proc, Op_type::STORE64, 0);	  
	  }
	}
      }
      assembly_proc_body(&proc, decl->as.procDecl.block);
      PROCS_push(proc);
      current_proc = NULL;
      buf__free(declared_local_vars);
    } break;
    default:
      error_here("Undefined kind.\n");
    }
  }
  buf__free(declared_global_vars);
}
#endif /* __assembler */

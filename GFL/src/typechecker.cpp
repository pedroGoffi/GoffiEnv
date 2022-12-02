#ifndef   __typechecker
#define  __typechecker
#include "./ast.cpp"
#include "./compiler.cpp"
#include "../../common/utils.cpp"
void typecheck_block(StmtList* sl);


Type* typecheck_expr(Expr* expr){
  switch(expr->kind){
  case EXPRKIND_INT:            return Type_int();
  case EXPRKIND_STRING_LITERAL: return Type_ptr(Type_char());
  case EXPRKIND_NEG:            return typecheck_expr(expr->as.expr);
  case EXPRKIND_NAME: {
    assert(expr->name);
    if(is_builtin(expr->name)){
      return Type_none();
    }     
    if(Var* local_var = Var_get(&local_vars, expr->name)){
      return local_var->type_field->type;
    }
    else if(Var* global_var = Var_get(&global_vars, expr->name)){
      return global_var->type_field->type;
    }
    else if(Macro* macro = GFL_Macros_find(expr->name)){
      return typecheck_expr(macro->expr);
    }
    else if(STR_CMP(expr->name, "pass")){
      return Type_none();
    }
    fprintf(stderr,
	    "ERORR: '%s' is not declares in this scope.\n",
	    expr->name);
    exit(1);
    
  }
  case EXPRKIND_PROC_CALL: {
    Call* call = &expr->as.call;
    if(Proc* proc = Proc_get(&procs, call->p_name)){
      size_t csz = call->args_size;
      size_t psz = proc->args->vars_size;
      if(csz != psz){
	for(size_t i=csz; i < buf__len(proc->args->vars); ++i){
	  Var* arg = proc->args->vars[i];
	  if(arg->expr){	
	    buf__push(call->args, arg->expr);
	    call->args_size++;	    
	  }
	  else {
	    fprintf(stderr,
		    "ERROR: expected %zu arguments but got %zu when calling procedure '%s'\n",
		    psz,
		    csz,
		    proc->name);
	    exit(1);
	  }
	}		
      }
      if(call->args_size != proc->args->vars_size){
	fprintf(stderr,
		"ERROR: expected %zu arguments but got %zu when calling procedure '%s'\n",
		proc->args->vars_size,
	        call->args_size,
		proc->name);
	exit(1);
      }
      return proc->ret_type;
    }
    else if (is_builtin(call->p_name)){
      return Type_none();
    }
    // procedure not declared
    fprintf(stderr,
	    "ERROR: the procedure '%s' was not declared in this scope.\n",
	    call->p_name);
    exit(1);
  }
  case EXPRKIND_DERREF_NAME: {
    Type* trhs = typecheck_expr(expr->as.derref);
    if(trhs->kind != TYPE_PTR){
      fprintf(stderr,
	      "ERROR: trying to derreference a non-pointer value.\n");
      exit(1);
    }
    return trhs->ptr.base;
  }
  case EXPRKIND_ADDROF_NAME: {
    Type* trhs = typecheck_expr(expr->as.addr_of);
    return Type_ptr(trhs);
  }
  case EXPRKIND_REASIGN: {
    Type* lhs = typecheck_expr(expr->as.Reasign.from);
    Type* rhs = typecheck_expr(expr->as.Reasign.to);
    if(!Type_cmp(lhs, rhs)){
      fprintf(stderr,
    	      "ERROR: can not re-asign the var from type '%s' to an type '%s'.\n",	      
    	      typekind_cstr(lhs), 
    	      typekind_cstr(rhs));
      exit(1);
    }
    return Type_none();
  }
  case EXPR_BINARY_OP: {
    fatal("ERROR: can not typecheck_op yet.\n");
    return Type_none();
  }
  case EXPRKIND_CAST: return expr->as.cast.type->type;
  case EXPRKIND_LOCAL_IF: {
    Type* lhs = typecheck_expr(expr->as.local_if.if_body);
    Type* rhs = typecheck_expr(expr->as.local_if.else_body);
    if(!Type_cmp(lhs, rhs)){
      fprintf(stderr,
	      "ERROR: local_if expected both sides types be the same type.\n");
      exit(1);
    }
    return lhs;
  }
  

  case EXPRKIND_ARRAY_ACCESS: {
    fatal("ARRAY ACCESS IS NOT IMPLEMENTED YET.\n");
    exit(1);
  } break;
  default:
    print_expr(expr);
    fprintf(stderr,
	    "ERROR: unreachable typecheck_expr.\n");
    exit(1);
  }
}
Type* typecheck_stmt(Stmt* st){
  switch(st->kind){
  case STMTKIND_EXPR:
    return typecheck_expr(st->as.expr);
  case STMTKIND_BLOCK: {
    typecheck_block(st->as.stmts);
    
    //Block* block = st->as.stmts;
    //for(size_t i=0; i < buf__len(block->stmts); ++i){
    //  typecheck_stmt(block->stmts[i]);
    //}
  } break;
  case STMTKIND_RETURN: {
    assert(current_decl);
    if(current_decl->as.procDecl.ret_type->kind == TYPE_NONE){
      fprintf(stderr,
	      "ERROR: return only works in non-void procedures.\n");
      exit(1);
    }
    Type* expected_type
      = current_proc->ret_type
      ? current_proc->ret_type
      : Type_none();
    Type* got_type = typecheck_expr(st->as.expr);
    if(!Type_cmp(got_type, current_proc->ret_type)){
      fprintf(stderr,
	      "ERROR: expected type '%s' in early return inside procedure '%s', but got type '%s'.\n",
	      typekind_cstr(expected_type),
	      current_decl->as.procDecl.name,
	      typekind_cstr(got_type));
      exit(1);
    }
  } break;
  case STMTKIND_LOCAL_VAR: {
    Var_push(&local_vars, st->as.var);
    Var* lv = st->as.var;
    
    if(lv->expr){

      Type* lhs_type	  = typecheck_expr(lv->expr);
      assert(lv->type_field);
      if(!lv->type_field->type){
	*lv->type_field->type = *lhs_type;
      }
      Type* expected_type =  lv->type_field->type;
      if(!Type_cmp(expected_type, lhs_type)){
	fprintf(stderr,
		"ERROR: can not asign the local variable %s of type %s to an type %s.\n",
		lv->type_field->name,
		typekind_cstr(expected_type),
		typekind_cstr(lhs_type));
	exit(1);
      }      
    }
    return Type_none();
  } 
  default:
    fprintf(stderr,
	    "ERROR: typecheck_stmt unreachable.\n");
    exit(1);
  }  
}
void typecheck_block(StmtList* sl){
  if(!sl) return;
  for(size_t i=0; i < buf__len(sl->stmts); ++i){
   typecheck_stmt(sl->stmts[i]);
  }
}
void typecheck_ast(Decl** ast){
  for(size_t i=0; i < buf__len(ast); ++i){
    Decl* node = ast[i];
    current_decl = node;
    switch(node->kind){
    case DECL_PROC:
      current_proc = &node->as.procDecl;
      Proc_push(&procs, node);
      typecheck_block(node->as.procDecl.block);
      current_proc = NULL;
      buf__free(local_vars);
      break;
    default:
      print_decl(node);
      fprintf(stderr,
	      "ERROR: typecheck_ast unreachable.\n");
      exit(1);
    }
  }
}
#endif /* __typechecker */

#ifndef   __typechecker
#define  __typechecker
#include "./ast.cpp"
#include "./compiler.cpp"
#include "../../common/utils.cpp"
void typecheck_block(StmtList* sl);


void complete_unsolved_type(Type** tptr){
  Type* type = *tptr;
  bool as_cte = type->is_const;
  printf("complete_unsolved_type: %s\n", type->name);
  assert(type->name);
  if(Typedef* tf = Typedef_get(&typedefs,
			       type->name)){
    *type = *tf->type;
    type->is_const = as_cte;
	    
  }
  else {
    fprintf(stderr,
	    "ERROR: the type '%s', was not declared in this scope.\n",
	    type->name);
    exit(1);
  }
}
void init_local_procedure_args(ProcArgs* args){
  for(size_t i=0; i < buf__len(args->vars); ++i){
    Var* arg   = args->vars[i];
    Type* type = Type_root(arg->type_field->type);
    
    raise_if_typedef(&type);
    Var_push(&local_vars, args->vars[i]);
  }
}
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
      size_t csz = buf__len(call->args);
      size_t psz = buf__len(proc->args->vars);
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
    raise_if_typedef(&trhs);
    if(trhs->kind != TYPE_PTR){
      fprintf(stderr,
	      "ERROR: trying to derreference a non-pointer value.\n");
      exit(1);
    }
    return trhs->ptr.base;
  }
  case EXPRKIND_ADDROF_NAME: {
    Type* trhs = typecheck_expr(expr->as.addr_of);
    raise_if_typedef(&trhs);
    return Type_ptr(trhs);
  }
  case EXPRKIND_REASIGN: {
    Type* lhs = typecheck_expr(expr->as.Reasign.from);
    if(lhs->is_const){
      fprintf(stderr,
	      "ERROR: can not re-asign a const variable.\n");
      exit(1);
    }
    Type* rhs = typecheck_expr(expr->as.Reasign.to);
    
    raise_if_typedef(&lhs);
    raise_if_typedef(&rhs);    
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
    Type* lhs = typecheck_expr(expr->as.BinaryOp.lhs);
    Type* rhs = typecheck_expr(expr->as.BinaryOp.rhs);
    raise_if_typedef(&lhs);
    raise_if_typedef(&rhs);
    
    bool  forbid_ptr_ptr = expr->as.BinaryOp.op == OP_KIND_PLUS;
    
    if(lhs->kind == TYPE_I64 and rhs->kind == TYPE_I64){
      return lhs;
    }
    if(lhs->kind == TYPE_PTR and rhs->kind == TYPE_PTR){
      if(forbid_ptr_ptr){
	fprintf(stderr,
		"ERROR: typecheck can not add two pointers.\n");
	exit(1);
      }
      fprintf(stderr,
	      "ERROR: could not type check binary op.\n");
      exit(1);
    }
    if(lhs->kind == TYPE_PTR or rhs->kind == TYPE_PTR){
      return lhs->kind == TYPE_PTR
	? lhs
	: rhs;	
    }

    printf("ERROR:\n");
    //exit(1);
  }
  case EXPRKIND_CAST: {
    Type* rhs =  expr->as.cast.type->type;
    assert(rhs);
    raise_if_typedef(&rhs);
    return rhs;
  }
  case EXPRKIND_LOCAL_IF: {
    Type* lhs = typecheck_expr(expr->as.local_if.if_body);
    Type* rhs = typecheck_expr(expr->as.local_if.else_body);
    raise_if_typedef(&lhs);
    raise_if_typedef(&rhs);    
    if(!Type_cmp(lhs, rhs)){
      fprintf(stderr,
	      "ERROR: local_if expected both sides types be the same type.\n");
      exit(1);
    }
    return lhs;
  }
  

  case EXPRKIND_ARRAY_ACCESS: {
    if(Var* var = Var_get(&local_vars, expr->as.ArrayAccess.name)){
      return var->type_field->type->ptr.base;
    }
    if(Var* var = Var_get(&global_vars, expr->as.ArrayAccess.name)){
      return var->type_field->type->ptr.base;
    }
    fprintf(stderr,
	    "ERROR: could not find the array: %s\n", expr->as.ArrayAccess.name);
    exit(1);
  } break;
  case EXPRKIND_FIELD_ACCESS: {};
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
    return Type_none();
  } break;
  case STMTKIND_IF:
  case STMTKIND_WHILE:
    break;
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
    Type* lhs   = lv->type_field->type;
    if(lhs->kind == TYPE_UNSOLVED){
      complete_unsolved_type(&lhs);
    }
    if(lv->expr){      
      Type* rhs	  = typecheck_expr(lv->expr);
      assert(lv->type_field);
      if(!lv->type_field->type){
	*lv->type_field->type = *rhs;
      }
      
      if(!Type_cmp(lhs, rhs)){
	fprintf(stderr,
		"ERROR: can not asign the local variable %s of type %s to an type %s.\n",
		lv->type_field->name,
		typekind_cstr(lhs),
		typekind_cstr(rhs));
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
    case DECL_VAR: {
      Var* var = &node->as.varDecl;
      if(var->expr){
	Type* lhs = var->type_field->type;
	if(lhs->kind == TYPE_UNSOLVED){
	  complete_unsolved_type(&lhs);	  
	}
	Type* rhs = typecheck_expr(var->expr);
	if(!Type_cmp(lhs, rhs)){
	  fprintf(stderr,
		  "ERROR: can not asign the global variable %s of type %s to an type %s.\n",
		  var->type_field->name,
		  typekind_cstr(lhs),
		  typekind_cstr(rhs));
	  exit(1);
	}
      }
      Var_push(&global_vars, var);
    } break;
    case DECL_PROC:
      current_proc = &node->as.procDecl;
      raise_if_typedef(&current_proc->ret_type);
      Proc_push(&procs, node);
      init_local_procedure_args(current_proc->args);
      typecheck_block(node->as.procDecl.block);
      current_proc = NULL;
      buf__free(local_vars);
      break;
    case DECL_IMPORT:
      typecheck_ast(node->as.Import.ast);
      break;
    case DECL_TYPEDEF: break;
    default:
      break;
      print_decl(node);
      fprintf(stderr,
	      "ERROR: typecheck_ast unreachable.\n");
      exit(1);
    }
  }
}
#endif /* __typechecker */

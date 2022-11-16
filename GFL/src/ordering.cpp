#ifndef __order__
#define __order__
#include "./ast.cpp"

typedef struct Sym Sym;
typedef struct Entity Entity;;
typedef struct VarEntity VarEntity;
typedef struct ResolvedExpr ResolvedExpr;
ResolvedExpr* order_expr(Expr* e);

void order_name(const char* name);
void order_decl_var(Decl* decl);
void order_decl_proc(Decl* decl);
void order_decl_struct(Decl* agg);
void  order_typedef(Decl* td);
Type* order_type(Type* tf);
void type_check(Type* a, Type* b, const char* err_msg);
const char* const __builtin_types__[] = {
  "u8", "u16", "u32", "u64",
  "i8", "i16", "i32", "i64",
  "f8", "f16", "f32", "f64",
};

Decl** ordered_decls = NULL;

enum SymState{
  UNORDERED,
  ORDERING,
  ORDERED
};
struct Sym{
  const char *name;
  Decl       *decl;
  SymState    state;

};
struct ResolvedExpr{
  Type* type;
  Expr* expr;
};

Sym* sym_list;
Sym* sym_get(const char* name){
  for(Sym* it=sym_list; it != buf__end(sym_list); ++it){
    if(it->name == name)
      return it;
  }
  return nullptr;
}

void sym_push(Decl* decl){
  assert(decl->name);
  if(sym_get(decl->name)){
    fprintf(stderr,
	    "ERROR: duplicated name '%s'.\n",
	    decl->name);
    exit(1);
  }
  Sym pushable = {
    .name   = decl->name,
    .decl   = decl,
    .state  = SymState::UNORDERED
  };
  buf__push(sym_list, pushable);
}
Type* order_type(Type* type){
  switch(type->kind){
  case TYPE_I64:
  case TYPE_F64:   
    break;
  case TYPE_ARRAY:
    order_type(type->array.base);
    if(type->array.size)
      order_expr(type->array.size);
    break;
  case TYPE_PTR:
    // NOTE: ptr is not ordered for now
    break;
  default:
    fprintf(stderr,
	    "ERROR: unhandled type kind at order_typefield.\n");
    exit(1);
  }
  return type;
}
void order_decl_var(Decl* decl){
  // TODO :order_type_field(var->type_field);
  Type* var_type = order_type(decl->as.varDecl.type_field->type);
  if(decl->as.varDecl.expr){
    ResolvedExpr* rexpr = order_expr(decl->as.varDecl.expr);
    type_check(var_type, rexpr->type, "at var declaration");    
  }
}
void order_decl_proc(Decl* decl){
  assert(decl->kind == DeclKind::DECL_PROC);
  if(STR_CMP(decl->name, "main")){
    if(decl->as.procDecl.ret_type){
      fprintf(stderr,
	      "ERROR: the 'main' procedure does not support return type.\n");
      exit(1);
    }
    ProcArgs *args = decl->as.procDecl.args;
    if(args->argsList_size > 0){
      if(args->argsList_size != 2){
	fprintf(stderr,
		"ERROR: the 'main' procedure only supports 2 or 0 arguments.\n")	;
	exit(1);
      }
      if(args->argsList[0]->type->kind != TYPE_I64){
	fprintf(stderr,
		"ERROR: the first argument on the 'main' procedure must be an i64.\n");
	exit(1);	
      }
      if(args->argsList[1]->type->kind != TYPE_PTR){
	fprintf(stderr,
		"ERROR: the first argument on the 'main' procedure must be an pointer.\n");
	exit(1);	
      }
      
    }
  }
  if(!decl->as.procDecl.ret_type){
    decl->as.procDecl.ret_type       = new Type;
    decl->as.procDecl.ret_type->kind = TYPE_I64;
  }
  // There is a return type  
}
void order_decl_struct(Decl* st){
  assert(st->kind == DeclKind::DECL_STRUCT);
  printf("NOTE: structs may change in the future, hence will not type check at the present moment.\n");
  exit(1);
}
ResolvedExpr* order_expr(Expr* e){
  Type* type = new Type;
  switch(e->kind){
  case EXPRKIND_INT: {
    type->kind = TYPE_I64;
  } break;
  case EXPRKIND_FLOAT: {
    type->kind = TYPE_F64;
  } break;
  case EXPRKIND_STRING_LITERAL: {
    type->kind = TYPE_PTR;
    type->ptr.base = new Type;
    type->ptr.base->kind = TYPE_CHAR;
  } break;

  case EXPRKIND_NAME: {
    order_name(e->name);        
  } break;
  case EXPRKIND_CAST: {
    // TODO: order typespec
    type = e->as.Cast.type->type;
    ResolvedExpr* re = order_expr(e->as.Cast.expr);
    if(re->type->kind == type->kind){
      printf("ERROR: Casting to the same expression type.\n");
      exit(1);
    }
  } break;
  case EXPRKIND_PROC_CALL: {
    const char* p_name = e->as.call.p_name;
    Sym* proc_sym = sym_get(p_name);
    // The procedure exists ?
    if(!proc_sym){
      fprintf(stderr,
	      "ERROR: the procedure '%s()' was not declared in this scope.\n",
	      p_name);
      // TODO: print procedure
      exit(1);
    }
    // the call match args and the procedure declaration is equal ?
    ProcArgs* proc_args = proc_sym->decl->as.procDecl.args;
    if(e->as.call.args_size != proc_args->argsList_size){
      fprintf(stderr,
	      "ERROR: expected %zu arguments for the procedure '%s', but got %zu.\n",
	      proc_args->argsList_size,
	      proc_sym->name,
	      e->as.call.args_size);
      exit(1);
    }
    
    for(size_t i=0; i < e->as.call.args_size; ++i){
      // the arguments is the same type ?
      ResolvedExpr* arg = order_expr(e->as.call.args[i]);
      Type* expected_type = proc_args->argsList[i]->type;
      type_check(expected_type,
		 arg->type,
		 "at procedure call");
    }
    /*
      OK: Then the type is the procedure return type
    */
    type = proc_sym->decl->as.procDecl.ret_type;
  } break;
  case EXPR_BINARY_OP:
    order_expr(e->as.BinaryOp.lhs);
    order_expr(e->as.BinaryOp.rhs);    
    break;
  case EXPRKIND_DERREF_NAME:
  case EXPRKIND_ADDROF_NAME:
    fprintf(stderr,
	    "[WARNING]: type check of addr_of or derref is not implemented yet.\n");
    break;
  case EXPRKIND_NONE:
  case EXPRKIND_REASIGN: 
  case EXPRKIND_FIELD_ACCESS: 
  case EXPRKIND_ARRAY_ACCESS: 
  default:
    fprintf(stderr,
	    "ERROR: unhandled expr kind at order_expr.\n");
    exit(1);
  }
  return new ResolvedExpr{
    .type = type,
    .expr = e
  };
}
void order_decl(Decl* decl){
  switch(decl->kind){
  case DeclKind::DECL_VAR:
    order_decl_var(decl);
    break;
  case DeclKind::DECL_PROC:   
    order_decl_proc(decl);
    break;
  case DeclKind::DECL_UNION:
  case DeclKind::DECL_ENUM:
  case DeclKind::DECL_STRUCT:   
    order_decl_struct(decl);
    break;
  case DeclKind::DECL_TYPEDEF:
    //order_typedef(decl);
    break;
  case DeclKind::DECL_CIMPORT:
  case DeclKind::DECL_NONE:
  default:
    fprintf(stderr,
	    "ERROR: undefined kidn at void order_decl(Decl* decl).\n");
    exit(1);
  }  
}
void order_sym(Sym* sym){
  if(sym->state == SymState::ORDERED) return;
  if(sym->state == SymState::ORDERING) {
    fprintf(stderr,
	    "ERROR: Cyclic dependency.\n");
    exit(1);
  }
  sym->state = SymState::ORDERING;
  order_decl(sym->decl);
  sym->state = SymState::ORDERED;
  buf__push(ordered_decls, sym->decl);
}
void order_name(const char* name){
  Sym* sym = sym_get(name);

  if(!sym){
    fprintf(stderr,
	    "ERROR: the name '%s' was not declared in this scope.\n", name);
    exit(1);
  }
  if(sym->state == SymState::ORDERED) return;
  if(sym->state == SymState::ORDERING){
    fprintf(stderr,
	    "ERROR: Cyclic dependency.\n");
    exit(1);
  }
  sym->state = SymState::ORDERING;
  order_decl(sym->decl);
  sym->state = SymState::ORDERED;
  buf__push(ordered_decls, sym->decl);
  
}
void order_sym_list(){
  for(Sym* it = sym_list; it != buf__end(sym_list); ++it){
    order_sym(it);
  }
}

void init_builtin_types(){
  for(const char* const type_name: __builtin_types__){    
    assert(!sym_get(type_name));
    Sym type = {
      .name  = type_name,
      .decl  = {},
      .state = SymState::ORDERED
    };
    buf__push(sym_list, type);
  }
}
Decl** order_ast(Decl** ast){
  // TODO: order ast is not fully implemented
  init_builtin_types();
  for(size_t i=0; i < buf__len(ast); i++){
    sym_push(ast[i]);
  }
  order_sym_list();  
 
  return ordered_decls;
}
void order_test(){
  assert(sym_get("foo") == NULL);
  Decl* cte = decl_var("foo",
		       type_int(),
		       expr_int(420));
  sym_push(cte);  
  assert(sym_get("foo")->decl == cte);
  TEST_OK;    
}
void type_check(Type* expected, Type* got, const char* err_msg){
  if(expected->kind != got->kind){
    fprintf(stderr,
	    "TypeError: expected '%s', but got '%s' %s\n",
	    human_type(expected),
	    human_type(got),
	    err_msg);
    exit(1);
  }
}
#endif /* __order__ */

// TODO: do beautiful prints on the ast
#ifndef __ast
#define __ast
#include <iostream>
#include <cassert>
#include <map>
#include "./token.cpp"
#include "../../common/utils.cpp"

typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct Var Var;
typedef struct Proc Proc;
typedef struct Type Type;
typedef struct TypeField TypeField;
typedef struct switchList switchList;
typedef struct Expr Expr;
typedef struct _FieldAccess _FieldAccess;
typedef struct Elif Elif;
typedef struct ProcArgs ProcArgs;
typedef struct TypeSpec TypeSpec;
static int ident;
#define ident() __ident()
#define newline() printf("\n")
void __ident(){
  int i = 0;
  while(i < ident){						
    printf(" "); ++i;
  }						
    
}
const char* human_type(Type* type);
void print_typespec(TypeSpec* t);
void print_type(Type* t);
void print_stmt_list(StmtList* b);

typedef enum ExprKind{
  EXPRKIND_NONE,
  EXPR_BINARY_OP,
  EXPR_COMPARASION,
  EXPRKIND_INT,
  EXPRKIND_FLOAT,
  EXPRKIND_STRING_LITERAL,  
  EXPRKIND_NAME,
  EXPRKIND_PROC_CALL,
  EXPRKIND_CAST,
  EXPRKIND_DERREF_NAME,
  EXPRKIND_ADDROF_NAME,
  EXPRKIND_REASIGN,
  EXPRKIND_FIELD_ACCESS,
  EXPRKIND_ARRAY_ACCESS,
} ExprKind;
typedef enum EXPR_BINARY_OP_KIND{
  OP_KIND_PLUS,
  OP_KIND_MINUS,  
} EXPR_BINARY_OP_KIND;

const char* human_expr_binary_op_kind(EXPR_BINARY_OP_KIND kind){
  switch(kind){
  case EXPR_BINARY_OP_KIND::OP_KIND_PLUS:  return "+";
  case EXPR_BINARY_OP_KIND::OP_KIND_MINUS: return "-";
  default:
    printf("ERROR: undefined kind at human_expr_binary_op_kind.\n");
    exit(1);
    break;
  }
}
typedef enum EXPR_CMP_KIND {
  LT  = 1,
  LTE = 2,
  EQ  = 3,
  NEQ = 4,
  GTE = 5,
  GT  = 6
} EXPR_CMP_KIND;
const char* human_expr_cmp_kind(EXPR_CMP_KIND kind){
  switch(kind){
  case EXPR_CMP_KIND::LT:	return "<";
  case EXPR_CMP_KIND::LTE:	return "<=";
  case EXPR_CMP_KIND::EQ:	return "==";
  case EXPR_CMP_KIND::NEQ:	return "!=";
  case EXPR_CMP_KIND::GTE:	return ">=";
  case EXPR_CMP_KIND::GT:	return ">";
  default:
    printf("ERROR: undefined kind at human_expr_cmp_kind.\n");
    exit(1);
    break;
    
  }
}
struct Expr{
  ExprKind kind;
  const char* name;
  union{
    int    INT;
    double FLOAT;
    const char* STRING;
    struct{
      EXPR_BINARY_OP_KIND op;
      Expr* lhs;
      Expr* rhs;
    } BinaryOp;
    struct{
      EXPR_CMP_KIND  op;
      Expr* lhs;
      Expr* rhs;
    } comparasion;
    struct{
      TypeSpec* type;
      Expr* expr;     
    } Cast;
    struct{
      const char* p_name;
      Expr** args;
      size_t args_size;
    } Call;
    struct{
      Expr* from;
      Expr* to;
    } Reasign;

    struct{      
      const char*  source;
      const char** fields;
      size_t       fields_size;
    } FieldAccess;
    struct{
      const char* name;
      Expr* expr;
    } ArrayAccess;

    Expr* derref;
    Expr* addr_of;
  } as;  
};


typedef enum Stmtkind{
  STMTKIND_RETURN,
  STMTKIND_BREAK,
  STMTKIND_CONTINUE,
  STMTKIND_STMT,
  STMTKIND_IF,        // O
  STMTKIND_WHILE,     // O
  STMTKIND_FOR,       // U
  STMTKIND_DO_WHILE,  // U
  STMTKIND_SWITCH,    // U
  STMTKIND_EXPR,      // O
  STMTKIND_LOCAL_VAR, // O

} Stmtkind;

struct Stmt{
  Stmtkind kind;
  union{
    struct{
      Expr*     expr;
      StmtList* block;
      Elif*     elif_nodes;
      StmtList* else_block;
    } __if;
    struct{
      Expr*     expr;
      StmtList* block;
    } __while;
    struct{
      Expr*     cond;
      switchList* node;
    } __switch;
    Var       *var;
    StmtList* stmts;
    Expr*     expr;      
  } as;
};
struct Var{
  TypeField* type_field;
  Expr*      expr;
};
struct switchList{
  Expr**     cond;
  StmtList** nodes;
  size_t     nodes_size;
};
struct Elif{
  Expr**      node_expr;
  StmtList**  node_block;
  size_t      nodes_size;
};
struct StmtList{
  Stmt**  stmts;
  size_t  stmts_size;
};
typedef enum TypeKind{
  TYPE_NONE,
  TYPE_I64,
  TYPE_F64,
  TYPE_CHAR,
  TYPE_PTR,
  TYPE_ARRAY,
  TYPE_STRUCT,
  TYPE_ENUM,
  TYPE_PROC,
  TYPE_UNSOLVED,
} TypeSpecKind;

TypeSpecKind TypeSpecKind_by_cstr(const char* type){
  // NOTE: STR_CMP is defined in ../../common/utils.cpp
#define DOIF_TYPE(str, ret_type)		\
  if(STR_CMP(type, str)){			\
    return ret_type;				\
  }
  DOIF_TYPE("i64",  TYPE_I64);
  DOIF_TYPE("f64",  TYPE_F64);
  DOIF_TYPE("char", TYPE_CHAR);
#undef DOIF_TYPE
  return TYPE_UNSOLVED;
}
const char* typekind_cstr(TypeKind kind){
  (void) kind;
  printf("ERROR: typekind_cstr is not implemented yet.\n");
  exit(1);
}
// TODO: create
struct Type{
  TypeKind kind;
  const char* name;
  union{
    struct{
      Type* base;
    } ptr;
    struct{
      Type* base;
      Expr* size;      
    } array;
    struct{
      TypeField* fields;
      size_t fields_size;
    } aggregate;
    struct{
      TypeField* params;
      size_t params_size;
      Type*  ret;
    } proc;
  };
};

const char* human_type(Type* type){
  switch(type->kind){
  case TYPE_I64:
    return "integer";
  case TYPE_F64:
    return "float";
  case TYPE_PTR:
    return "pointer";
  case TYPE_CHAR:
    return "char";
  case TYPE_ARRAY:
    return "array";
  case TYPE_STRUCT:
    return "struct";
  case TYPE_ENUM:
    return "procedure";
  case TYPE_PROC:
    return "procedure";
  case TYPE_UNSOLVED:
    return "unsolved type";
  case TYPE_NONE:
    return "none";    
  default:
    error_here("unhandled type kind.\n");
  }
}
// TODO: make a short version of this



Type* type_alloc(TypeKind kind){
  Type *t = new Type;
  t->kind = kind;
  return t;
}
Type* type_int(){
  return type_alloc(TYPE_I64);
}
typedef struct CachedPtrType{
  Type* base;
  Type* ptr;
} CachedPtrType;
CachedPtrType* cached_ptr_types;

Type* type_ptr(Type* base){
  for(CachedPtrType* it = cached_ptr_types; it != buf__end(cached_ptr_types); ++it){
    if(it->base == base){
      return it->ptr;
    }
  }
  Type* ptr = type_alloc(TYPE_PTR);
  ptr->ptr.base = base;
  
  // save this ptr
  CachedPtrType new_cached = {
    .base = base,
    .ptr  = ptr
  };
  buf__push(cached_ptr_types, new_cached);
  return ptr;
}


struct TypeField{
  const char* name;
  Type* type;
};
struct TypeSpec{
  const char* name;
  Type*       type;
};

enum DeclKind{
  DECL_NONE,
  DECL_ENUM,
  DECL_STRUCT,
  DECL_UNION,
  DECL_VAR,
  DECL_TYPEDEF,
  DECL_PROC,
  DECL_CIMPORT
};
struct Proc{
  ProcArgs  *args;
  Type      *ret_type;
  StmtList  *block;
};
struct Decl{
  DeclKind kind;
  const char* name;
  union{
    Var  varDecl;
    //Proc* procDecl;
    //struct{
    //  Type*     type;
    //  Expr*     expr;
    //} varDecl;
    struct{
      ProcArgs  *args;
      Type      *ret_type;
      StmtList  *block;
    } procDecl;
    struct{
      TypeSpec **fields;
      size_t     fields_size;
    } structDecl;
    struct{
      const char* FILENAME;
      bool isStd;
    } cimportDecl;
    struct{
      Type* type;
      Type* type_equivalent;
    } Typedef;
  } as;
};
Decl* decl_var(const char* name, Type* type, Expr* expr){
  (void) name;
  (void) type;
  (void) expr;
  printf("ERROR: decl_var is not implemented yet.\n");
  exit(1);
  return {};
}
struct ProcArgs{
  TypeSpec   **argsList;
  size_t       argsList_size;
};
Expr* expr_make_name(Expr* f){
  Expr* r = new Expr;
  r->kind = EXPRKIND_NAME;
  memcpy(r, f, sizeof(*f));
  return r;
}
Expr* expr_make_cmp(Expr* lhs, Expr* rhs, EXPR_CMP_KIND op){
  Expr* expr_cmp = new Expr;
  expr_cmp->kind = EXPR_COMPARASION;
  expr_cmp->as.comparasion.op  = op;
  expr_cmp->as.comparasion.lhs = lhs;
  expr_cmp->as.comparasion.rhs = rhs;
  return expr_cmp;
}
EXPR_BINARY_OP_KIND make_expr_binary_op_kind(){
  if(is_token(TOKEN_PLUS)) return EXPR_BINARY_OP_KIND::OP_KIND_PLUS;
  printf("ERROR: undefined binary operation symbol.\n");
  exit(1);
}
EXPR_CMP_KIND make_cmp_kind(){
  if(is_token(TOKEN_LESS)) return EXPR_CMP_KIND::LT;
  if(is_token(TOKEN_CMP_EQ))   return EXPR_CMP_KIND::EQ;
  printf("ERROR: undefined binary comparasion operation symbol.\n");
  exit(1);
}
Expr* expr_make_binary(Expr* lhs, Expr* rhs, EXPR_BINARY_OP_KIND op){
  Expr* expr_bin = new Expr;
  expr_bin->kind		= EXPR_BINARY_OP;
  expr_bin->as.BinaryOp.op	= op;
  expr_bin->as.BinaryOp.lhs	= lhs;
  expr_bin->as.BinaryOp.rhs	= rhs;
  return expr_bin;  
}
Expr* expr_int(int val){
  return new Expr{
    .kind = EXPRKIND_INT,
    .as   = {
      .INT = val
    }
  };
}
void print_expr(Expr* e){
  switch(e->kind){
  case EXPR_BINARY_OP:
    printf("( ");
    print_expr(e->as.BinaryOp.lhs);
    printf(" %s ", human_expr_binary_op_kind(e->as.BinaryOp.op));
    print_expr(e->as.BinaryOp.rhs);
    printf(" )");
    break;
  case EXPR_COMPARASION:
    printf("(cmp [(");
    print_expr(e->as.comparasion.lhs);
    printf(") (TODO: print the comparasion symbol) (");
    print_expr(e->as.comparasion.rhs);
    printf(")])");
    break;
  case EXPRKIND_FLOAT:
    printf("%f", e->as.FLOAT);
    break;
  case EXPRKIND_INT:
    printf("%d", e->as.INT);
    break;
  case EXPRKIND_CAST:
    printf("(cast [");
    print_type(e->as.Cast.type->type);
    printf("]::[");
    print_expr(e->as.Cast.expr);
    printf("])");    
    break;
  case EXPRKIND_NAME:
    printf("%s", e->name);
    break;
  case EXPRKIND_STRING_LITERAL:
    printf("string(\"%s\")",
	   e->as.STRING);
    break;
  case EXPRKIND_PROC_CALL:
    printf("call(%s[",
	   e->as.Call.p_name);
    for(size_t i=0;i<buf__len(e->as.Call.args);++i){
      printf((i>0)?", ":"");
      print_expr(e->as.Call.args[i]);
    }
    printf("])");
    break;
  case EXPRKIND_REASIGN:
    printf("Reasign([");
    print_expr(e->as.Reasign.from);
    printf("]->[");
    print_expr(e->as.Reasign.to);
    printf("])");
    break;
  case EXPRKIND_DERREF_NAME:
    printf("Derref(");
    print_expr(e->as.derref);
    printf(")");
    break;
  case EXPRKIND_ADDROF_NAME:
    printf("Addr(");
    print_expr(e->as.addr_of);
    printf(")");    
    break;
  case EXPRKIND_ARRAY_ACCESS:
  case EXPRKIND_FIELD_ACCESS:
  case EXPRKIND_NONE:
  default:
    printf("Unexpected ExprKind\n");
    exit(1);
  }
}
void print_type(Type* t){
  switch(t->kind){
  case TYPE_PTR:
    printf("ptr_to(");
    print_type(t->ptr.base);
    printf(")");
    break;

  case TYPE_UNSOLVED:
    printf("[unsolved: %s]", t->name);
    break;
  case TYPE_ARRAY:
    printf("array(");
    if(t->array.size)
      print_expr(t->array.size);
    printf(")");
    
    break;
  case TYPE_NONE:
    printf("none");
    break;
  case TYPE_I64:
    printf("i64");
    break;
  case TYPE_F64:
    printf("f64");
    break;
  case TYPE_CHAR:
    printf("char");
    break;
  case TYPE_PROC:
    printf("proc");
    break;
  case TYPE_STRUCT:
  case TYPE_ENUM:
  default:
    printf("Unexpected TypeSpecKind\n");
    exit(1);
  }
}
void print_typespec(TypeSpec* k){
  printf("[%s: ", k->name);
  print_type(k->type);
  printf("]");
}
void print_stmt(Stmt* s){
  switch(s->kind){
  case STMTKIND_RETURN:
    printf("return ");
    print_expr(s->as.expr);
    break;
  case STMTKIND_LOCAL_VAR:
    printf("(var %s[",
	   s->as.var->type_field->name);
    print_type(s->as.var->type_field->type);
    printf("]");
    if(s->as.var->expr){
      printf("=[");
      print_expr(s->as.var->expr);
      printf("]");
    }
    printf(")\n");
    break;


  case STMTKIND_IF:
    printf("(if \n");
    ident++;
    ident();
    printf("(");
    print_expr(s->as.__if.expr);
    printf(")\n");
    ident();
    printf("(\n");
    print_stmt_list(s->as.__if.block);
    ident();
    printf(")\n");
    ident--;
    ident();
    printf(")\n");
    if(s->as.__if.elif_nodes){
      for(size_t i = 0; i < s->as.__if.elif_nodes->nodes_size; ++i ){
	ident();
	printf("(elif");
	newline();
	ident++;
	ident();
	printf("(");
	print_expr(s->as.__if.elif_nodes->node_expr[i]);
	printf(")\n");
	ident++;
	print_stmt_list(s->as.__if.elif_nodes->node_block[i]);
	ident--;
	ident();
	printf(")\n");
      }
      ident--;
      ident--;      
      ident();
    }
    if(s->as.__if.else_block){
      ident();
      printf("(else");
      newline();
      ident++;
      print_stmt_list(s->as.__if.else_block);
      ident--;
      ident();
      printf(")");
      newline();
    }       
    break;
  case STMTKIND_EXPR:
    print_expr(s->as.expr);
    break;
  case STMTKIND_WHILE:
    printf("(while\n");
    ident++;
    ident();

    printf("( ");
    print_expr(s->as.__while.expr);
    printf(" )\n");
    ident();
    printf("{");
    print_stmt_list(s->as.__while.block);
    ident();
    printf("}\n");    
    ident--;
    ident();
    printf(")");
    break;
  case STMTKIND_SWITCH:
  case STMTKIND_BREAK:
  case STMTKIND_CONTINUE:
  case STMTKIND_STMT:
  case STMTKIND_DO_WHILE:
  case STMTKIND_FOR:
  default:
    printf("Unexpected StmtKind\n");
    exit(1);
  }
}
void print_stmt_list(StmtList* b){
  ident++;
  for(size_t i = 0; i < b->stmts_size; ++i){
    ident();
    print_stmt(b->stmts[i]);
    newline();
  }
  ident--;
}
void print_decl(Decl* d){
  switch(d->kind){
  case DeclKind::DECL_TYPEDEF:
    printf("typedef (");
    print_type(d->as.Typedef.type);
    printf(") = (");
    print_type(d->as.Typedef.type_equivalent);
    printf(")\n");    
    break;
  case DeclKind::DECL_VAR:
    printf("(var %s[",
	   d->name);
    print_type(d->as.varDecl.type_field->type);
    printf("] = ( ");
    print_expr(d->as.varDecl.expr);
    printf(" ))\n");
    break;
  case DeclKind::DECL_PROC:
    ident();
    printf("( proc [%s->",
	   d->name);
    if(d->as.procDecl.ret_type){
      print_type(d->as.procDecl.ret_type);
    }
    else{
      printf("unsolved");
    }
    printf("]\n");
    ident++;
    for(size_t i = 0; i < d->as.procDecl.args->argsList_size; ++i){
      ident();
      print_typespec(d->as.procDecl.args->argsList[i]);
      newline();
    }
    if(d->as.procDecl.block){    
      ident++;
      print_stmt_list(d->as.procDecl.block);
      ident--;
    }
    ident--;
    ident();
    printf(")");    
    newline();
    break;
  case DeclKind::DECL_STRUCT:
    printf("(struct %s\n", d->name);
    ident++;
    for(size_t i=0; i < d->as.structDecl.fields_size; ++i){
      ident();
      print_typespec(d->as.structDecl.fields[i]);
      newline();
    }
    ident--;
    ident();
    printf(")");
    break;
  case DeclKind::DECL_CIMPORT:
  case DeclKind::DECL_ENUM:
  case DeclKind::DECL_UNION:
  default:
    printf("Unexpected DeclKind\n");
    exit(1);
  }
  // NOTE: for now declarations can not be nested
  ident = 0;
  //assert(ident == 0 && "THIS MIGHT BE A BUG IN THE IDENT LOGIC");
}
void print_ast(Decl** ast){
  for(size_t i=0; i<buf__len(ast); ++i){
    print_decl(ast[i]);
    printf("\n");
    
  }
}

typedef Decl** AST_ROOT;
typedef Decl*  AST_NODE;


#endif /*__ast */
// TODO: do beautiful prints on the ast
#ifndef __ast
#define __ast
#include <iostream>
#include <cassert>
#include "./token.cpp"
#include "../../common/utils.cpp"

typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct Var Var;
typedef struct Proc Proc;
typedef struct Type Type;
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
struct Expr{
  ExprKind kind;
  union{
    int    INT;
    double FLOAT;
    const char* STRING;
    const char* name;
    struct{
        char op;
        Expr* lhs;
        Expr* rhs;
    } BinaryOp;
    struct{
      TokenKind  op;
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
  const char* name;
  Type* type;
  Expr* expr;
} ;
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
  TYPESPEC_NONE,
  TYPESPEC_NAME,
  TYPESPEC_CONST,
  TYPESPEC_PTR,
  TYPESPEC_ARRAY,
  TYPESPEC_UNSOLVED,
} TypeSpecKind;
// TODO: create
struct Type{
  TypeKind kind;
  bool     is_const;
  union{
    const char* type_name;
    Type* ptr_to;
    Type* as_const;
    struct{
      Type* type;
      Expr* array_size;      
    } array;
  };
};
struct TypeSpec{
  const char* name;
  Type*       type;
};

enum DeclKind{
  DECLKIND_NONE,
  DECLKIND_ENUM,
  DECLKIND_STRUCT,
  DECLKIND_UNION,
  DECLKIND_VAR,
  DECLKIND_TYPEDEF,
  DECLKIND_PROC,
  DECLKIND_CIMPORT
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
    //Var*  varDecl;
    //Proc* procDecl;
    struct{
      Type*     type;
      Expr*     expr;
    } varDecl;
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
Expr* expr_make_cmp(Expr* lhs, Expr* rhs, TokenKind op){
  Expr* expr_cmp = new Expr;
  expr_cmp->kind = EXPR_COMPARASION;
  expr_cmp->as.comparasion.op  = op;
  expr_cmp->as.comparasion.lhs = lhs;
  expr_cmp->as.comparasion.rhs = rhs;
  return expr_cmp;
}
Expr* expr_make_binary(Expr* lhs, Expr* rhs, char op){
  Expr* expr_bin = new Expr;
  expr_bin->kind		= EXPR_BINARY_OP;
  expr_bin->as.BinaryOp.op	= op;
  expr_bin->as.BinaryOp.lhs	= lhs;
  expr_bin->as.BinaryOp.rhs	= rhs;
  return expr_bin;
  
}
void print_expr(Expr* e){
  switch(e->kind){
  case EXPR_BINARY_OP:
    printf("(%c ", e->as.BinaryOp.op);    
    print_expr(e->as.BinaryOp.lhs);
    printf(" ");
    print_expr(e->as.BinaryOp.rhs);
    printf(" ) ");
    break;
  case EXPR_COMPARASION:
    printf("(cmp [(");
    print_expr(e->as.comparasion.lhs);
    printf(") %s (", human_readable_token(e->as.comparasion.op));
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
    printf("%s", e->as.name);
    break;
  case EXPRKIND_STRING_LITERAL:
    printf("string(\"%s\")",
	   e->as.name);
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
  case TYPESPEC_NAME:  
    printf("%s", t->type_name);
    break;
  case TYPESPEC_PTR:
    printf("ptr_to(");
    print_type(t->ptr_to);
    printf(")");
    break;
  case TYPESPEC_CONST:
    printf("const(");
    print_type(t->as_const);
    printf(")");
    break;
  case TYPESPEC_UNSOLVED:
    printf("unsolved");
    break;
  case TYPESPEC_ARRAY:
    printf("array(");
    if(t->array.array_size)
      print_expr(t->array.array_size);
    printf(")");
    
    break;
  case TYPESPEC_NONE:
    printf("none");
    break;

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
	   s->as.var->name);
    print_type(s->as.var->type);
    printf("]");
    if(s->as.var->expr){
      printf("=[");
      print_expr(s->as.var->expr);
      printf("])\n");
    }
    break;

  case STMTKIND_SWITCH:
  case STMTKIND_BREAK:
  case STMTKIND_CONTINUE:
  case STMTKIND_STMT:
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
  case DECLKIND_TYPEDEF:
    printf("typedef (");
    print_type(d->as.Typedef.type);
    printf(") = (");
    print_type(d->as.Typedef.type_equivalent);
    printf(")\n");    
    break;
  case DECLKIND_VAR:
    printf("(var %s[",
	   d->name);
    print_type(d->as.varDecl.type);
    printf("] = ( ");
    print_expr(d->as.varDecl.expr);
    printf(" ))\n");
    break;
  case DECLKIND_PROC:
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
  case DECLKIND_STRUCT:
    printf("(struct %s\n", d->name);
    ident++;
    for(size_t i=0; i< d->as.structDecl.fields_size; ++i){
      ident();
      print_typespec(d->as.structDecl.fields[i]);
      newline();
    }
    ident--;
    ident();
    printf(")");
    break;
  case DECLKIND_CIMPORT:
  case DECLKIND_NONE:
  case DECLKIND_ENUM:
  case DECLKIND_UNION:
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
// TODO

typedef Decl** AST_ROOT;
typedef Decl*  AST_NODE;

Type* type_ptr(Type* ptr_target){
  Type* result = new Type;
  result->kind = TYPESPEC_PTR;
  result->ptr_to = ptr_target;
  return result;
}
Type* type_name(const char* str="<std::type>"){
  Type* result = new Type;
  result->kind = TYPESPEC_NAME;
  result->type_name = str;
  return result;
}



#endif /*__ast */

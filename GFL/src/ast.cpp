// TODO: do beautiful prints on the ast
#ifndef __ast
#define __ast
#include <iostream>
#include <cassert>
#include <map>
#include "./token.cpp"
#include "../../common/utils.cpp"
#define MAX_STREAMS_SIZE 1024
typedef struct Stmt Stmt;
typedef struct Module Module;
typedef struct StmtList StmtList;
typedef StmtList Block;
typedef struct Var Var;
typedef struct Proc Proc;
typedef struct Type Type;
typedef struct TypeField TypeField;
typedef struct switchList switchList;
typedef struct Expr Expr;
typedef struct Call Call;
typedef struct _FieldAccess _FieldAccess;
typedef struct Elif Elif;
typedef struct ProcArgs ProcArgs;

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
void print_typefield(TypeField* t);
void print_expr(Expr* e);
void print_type(Type* t);
void print_stmt_list(Block* b);


struct Typedef {
  const char* name;
  Type*       type;
};

typedef enum ExprKind{
  EXPRKIND_NONE,
  EXPR_BINARY_OP,
  EXPR_COMPARASION,
  EXPRKIND_LOGIC,
  EXPRKIND_NEG,
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
  EXPRKIND_LOCAL_IF,
  EXPRKIND_NAMESPACE_GET,
} ExprKind;
typedef enum EXPR_BINARY_OP_KIND{
  OP_KIND_PLUS = 100,
  OP_KIND_MINUS,
  OP_KIND_DIV,
  OP_KIND_MULT,
  OP_KIND_SHR,
  OP_KIND_SHL,
  OP_KIND_OR,
  OP_KIND_AND
  
} EXPR_BINARY_OP_KIND;

const char* human_expr_binary_op_kind(EXPR_BINARY_OP_KIND kind){
  switch(kind){
  case EXPR_BINARY_OP_KIND::OP_KIND_PLUS:  return "+";
  case EXPR_BINARY_OP_KIND::OP_KIND_MINUS: return "-";
  case EXPR_BINARY_OP_KIND::OP_KIND_DIV:   return "/";
  case EXPR_BINARY_OP_KIND::OP_KIND_MULT:  return "*";
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
typedef enum TypeKind{
  TYPE_NONE,
  TYPE_ANY,
  TYPE_I64,
  TYPE_F64,
  TYPE_CHAR,
  TYPE_PTR,
  TYPE_ENUM,
  TYPE_PROC,
  TYPE_UNSOLVED,
} TypeFieldKind;

struct Type{
  TypeKind kind;
  size_t   size;
  bool     is_const;
  bool     is_unsigned;
  const char* name;
  union{
    struct{
      Type* base;
    } ptr;

    struct{
      TypeField* fields;
      size_t     fields_size;
    } aggregate;
    struct{
      TypeField* params;
      size_t params_size;
      Type*  ret;
    } proc;
  };
};
struct Call{
  const char* p_name;
  Expr** args;
  size_t args_size;
};
enum LogicKind{LK_OR, LK_AND};
struct Logic{
  LogicKind kind;
  Expr* lhs;
  Expr* rhs;
};
struct Cast{
  TypeField* type;
  Expr* expr;     
} ;
struct Expr{
  ExprKind    kind;
  const char* name;
  Type*       type;
  union{
    int    INT;
    double FLOAT;
    const char* STRING;
    struct {
      Expr* cond;
      Expr* if_body;
      Expr* else_body;
    } local_if;
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
    
    Call call;
    Cast cast;

    struct{
      Token token;
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
      Expr*       expr;
    } ArrayAccess;
    struct {
      const char* name;
      Expr*       rhs;
    } NamespaceGet;
    Logic logic;
    Expr* expr;
    Expr* derref;
    Expr* addr_of;
  } as;  
};
Expr* make_expr_logic(EXPR_BINARY_OP_KIND kind, Expr* lhs, Expr* rhs){
  Expr* r = new Expr;
  r->kind = EXPR_BINARY_OP;
  r->as.BinaryOp.op   = kind;
  r->as.BinaryOp.lhs  = lhs;
  r->as.BinaryOp.rhs  = rhs;
  return r;
}
const char* get_name_of_reasign(Expr* e){  
  if(e->kind == EXPRKIND_NAME){
    return e->name;
  }
  assert(e->kind == EXPRKIND_DERREF_NAME);
  return get_name_of_reasign(e->as.Reasign.from);
}

typedef enum Stmtkind{
  STMTKIND_RETURN,
  STMTKIND_BLOCK,
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
      Expr*   expr;
      Block*  block;
      Elif*   elif_nodes;
      Block*  else_block;
    } __if;
    struct{
      Expr*   expr;
      Block*  block;
    } __while;
    struct{
      Expr*     cond;
      switchList* node;
    } __switch;
    Var*    var;
    Block*  stmts;
    Expr*   expr;      
  } as;
};
struct Var{
  TypeField* type_field;
  size_t     offset;
  Expr*      expr;
  bool       passed_by_stack;
};
struct switchList{
  Expr**  cond;
  Block** nodes;
  size_t  nodes_size;
};
struct Elif{
  Expr**   node_expr;
  Block**  node_block;
  size_t   nodes_size;
};
struct StmtList{
  Stmt**  stmts;
  size_t  stmts_size;
};

struct Macro{
  const char* name;
  Expr*       expr;
};

Macro* GFL_macros = NULL;
#define INIT_MACRO(_name, _str, _expr) Macro _name = {_str, _expr}
int GFL_Macros_pos(const char* name){
  for(size_t i=0; i < buf__len(GFL_macros); ++i){
    if(STR_CMP(GFL_macros[i].name, name)){
      return (int)i;
    }
  }
  return -1;
}
Macro* GFL_Macros_find(const char* name){
  int idx = GFL_Macros_pos(name);
  if(idx < 0) return NULL;  
  return &GFL_macros[idx];
}

inline bool GFL_MACROS_exists(const char* name){
  return (GFL_Macros_pos(name) != -1);
}
void GFL_Macros_push(Macro macro){
  if(GFL_Macros_find(macro.name)){
    fprintf(stderr,
	    "ERROR: redeclaration of a macro %s.\n",
	    macro.name);
    exit(1);
  }
  buf__push(GFL_macros, macro);
}
TypeFieldKind TypeFieldKind_by_cstr(const char* type){
  // NOTE: STR_CMP is defined in ../../common/utils.cpp
#define DOIF_TYPE(str, ret_type)		\
  if(STR_CMP(type, str)){			\
    return ret_type;				\
  }
  DOIF_TYPE("any",  TYPE_ANY);
  DOIF_TYPE("i64",  TYPE_I64);
  
  DOIF_TYPE("f64",  TYPE_F64);
  DOIF_TYPE("char", TYPE_CHAR);;
  DOIF_TYPE("void", TYPE_NONE);  
#undef DOIF_TYPE
  return TYPE_UNSOLVED;
}
const char* typekind_cstr(Type* t){
  TypeKind kind = t->kind;
  switch(kind){
  case TypeKind::TYPE_NONE:   return "void";
  case TypeKind::TYPE_I64:    return "i64";
  case TypeKind::TYPE_F64:    return "f64";
  case TypeKind::TYPE_ANY:    return "any";
  case TypeKind::TYPE_CHAR:   return "char";
    
  case TypeKind::TYPE_PTR:  {
    assert(t->ptr.base);
    char* base = NULL;
    buf__push(base, '*');
    const char* body = typekind_cstr(t->ptr.base);
    for(size_t i=0; i < strlen(body); ++i){
      buf__push(base, *(body + i));
    }    
    return base;
  }
 
  case TypeKind::TYPE_ENUM:
  case TypeKind::TYPE_PROC:
  case TypeKind::TYPE_UNSOLVED:
    assert(t->name);
    return t->name;
    break;
  default:
    fprintf(stderr,
	    "ERROR: unreachable: typekind_cstr(TypeKind kind).\n");
    exit(1);
  }

}
// TODO: create
Type* Type_root(Type* mptr){
  assert(mptr);
  if(mptr->kind == TYPE_PTR){
    return Type_root(mptr->ptr.base);    
  }
  return mptr;
}
Type* Type_none(){
  auto* res = new Type;
  res->kind = TYPE_NONE;
  return res;
}
Type* Type_int(){
  auto* res = new Type;
  res->kind = TYPE_I64;
  return res;
}
Type* Type_char(){
  auto* res = new Type;
  res->kind = TYPE_CHAR;
  return res;
}
Type* Type_ptr(Type* base){
  auto* res     = new Type;
  res->kind     = TYPE_PTR;
  res->ptr.base = base;
  return res;
}
Type* Type_ptr_copy(Type* ptr){
  assert(ptr->kind == TYPE_PTR);
  auto* res = new Type;
  res->name = ptr->name;
  res->kind = TYPE_PTR;
  res->ptr.base = new Type;
  memcpy(res->ptr.base, ptr->ptr.base, sizeof(*ptr->ptr.base));
  return res;
}

int sizeof_type(Type* type){
  switch(type->kind){ 
  case TypeKind::TYPE_CHAR:      
    return 1;    
  case TypeKind::TYPE_ANY:
  case TypeKind::TYPE_I64:  
  case TypeKind::TYPE_PTR:  return 8;   
  case TypeKind::TYPE_UNSOLVED: break;
  case TypeKind::TYPE_F64: 
  case TypeKind::TYPE_ENUM:
  case TypeKind::TYPE_PROC:
    fprintf(stderr,
	    "ERROR: unhandled sizeof_type.\n");
    exit(1);
  case TypeKind::TYPE_NONE:
    fprintf(stderr,
	    "ERROR: type void does not have a size.\n");
    exit(1);
  }
  return 0;
}

bool Type_cmp(Type* got, Type* expected){
  if(got->kind == TYPE_PTR and expected->kind == TYPE_PTR){
    return Type_cmp(got->ptr.base, expected->ptr.base);
  }
  if(expected->kind == TYPE_ANY) return true;
  return got->kind == expected->kind;
}
Type* derref_type(Type* type){
  switch(type->kind){
  case TypeKind::TYPE_NONE:
  case TypeKind::TYPE_I64:
  case TypeKind::TYPE_F64:
  case TypeKind::TYPE_CHAR:
    return NULL;
  case TypeKind::TYPE_PTR: 
    assert(type->ptr.base);
    return type->ptr.base;
  case TypeKind::TYPE_UNSOLVED:
    fprintf(stderr,
	    "ERROR: could not solve Type during Parsing.\n");
    exit(1);
  case TypeKind::TYPE_ENUM:
  case TypeKind::TYPE_PROC:
  default:
    fprintf(stderr,
	    "ERROR: unreachable derref_type(Type* type).\n");
    exit(1);
  }
}
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
bool Type_is_byte_related(Type* type){
  printf("Type_is_byte_related(Type* type) :: %s\n",
	 human_type(type));
  return type->kind == TYPE_CHAR;
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
Type* Type_clone(Type* t){
  auto* res = type_alloc(t->kind);
  switch(t->kind){
  case TYPE_PTR:
    res->ptr.base = Type_clone(t->ptr.base);
    break;
  case TYPE_CHAR:
  case TYPE_I64:
  case TYPE_F64:
    break;
  default:
    printf("ERROR: Type_clone(Type* t).\n");
    exit(1);
  }
  return res;
}

struct TypeField{
  const char* name;
  Type* type;
};


enum DeclKind{
  DECL_NONE,
  DECL_ENUM,
  DECL_UNION,
  DECL_VAR,
  DECL_TYPEDEF,
  DECL_PROC,
  DECL_IMPORT, 
  DECL_NAMESPACE,
};
struct Proc{
  const char* name;
  ProcArgs  *args;
  size_t    stack_allocation;
  Type      *ret_type;
  StmtList  *block;
};
struct EnumField{
  const char* name;
  Expr*       expr;
  size_t      offset;
};
struct EnumFields{
  EnumField**  fields;
};
struct Decl{
  DeclKind    kind;
  const char* name;
  bool        used;
  union{
    Var  varDecl;
    Proc procDecl;    
    struct{
      EnumFields* fields;
    } Enum;
    struct{
      const char* FILENAME;
      bool isStd;
    } cimportDecl;
    Typedef type_def;
    //struct{
    //  const char* name;
    //  Type*       type;
    //} Typedef;
    struct{
      const char* name;
      Decl**      decls;
    } Namespace;
    struct{
      const char* path;
      Decl**      ast;
    } Import;
  } as;
};
const char** Included_fp = NULL;
bool Included_fp_find(const char* str){
  for(size_t i=0; i < buf__len(Included_fp); ++i){
    if(STR_CMP(str, Included_fp[i])) return true;
  }
  return false;
}
void Included_fp_push(const char* str){
  assert(!Included_fp_find(str));
  buf__push(Included_fp, str);
}
Decl**   namespaces = NULL;
Decl* namespaces_get(const char* root, const char* child=""){
  for(size_t i=0; i < buf__len(namespaces); ++i){
    if(STR_CMP(namespaces[i]->name, root))
      return namespaces[i];
  }
  for(size_t i=0; i < buf__len(namespaces); ++i){
    Decl** ast = namespaces[i]->as.Namespace.decls;
    for(size_t i=0; i < buf__len(ast); ++i){
      if(ast[i]->kind != DECL_NAMESPACE) continue;
      if(STR_CMP(ast[i]->name, child)){
	return ast[i];
      }
    }
  }
  return NULL;
}
void namespaces_push(Decl*** ns, Decl* d){
  assert(d->name);
  assert(!namespaces_get(d->name));  
  buf__push(*ns, d);
}
Decl* namespaces_get_expr(Expr* e){
  switch(e->kind)
  {
  case EXPRKIND_NAME: return namespaces_get(e->name);
  case EXPRKIND_PROC_CALL:
    return namespaces_get(e->as.call.p_name);
  case EXPRKIND_NAMESPACE_GET:
    return namespaces_get_expr(e->as.NamespaceGet.rhs);
  default:
    print_expr(e);
    printf("[I-ERR]: namespaces_get_expr(Expr* e)\n");
    exit(1);
  }
}
Decl* decl_var(const char* name, Type* type, Expr* expr){
  (void) name;
  (void) type;
  (void) expr;
  printf("ERROR: decl_var is not implemented yet.\n");
  exit(1);
  return {};
}
struct ProcArgs{
  Var**        vars;
  size_t       vars_size;
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
  if(is_token(TOKEN_PLUS))	return EXPR_BINARY_OP_KIND::OP_KIND_PLUS;
  if(is_token(TOKEN_TAKEAWAY))	return EXPR_BINARY_OP_KIND::OP_KIND_MINUS;
  if(is_token(TOKEN_DIV))	return EXPR_BINARY_OP_KIND::OP_KIND_DIV;
  if(is_token(TOKEN_STAR))	return EXPR_BINARY_OP_KIND::OP_KIND_MULT;
  if(is_token(TOKEN_SHR))	return EXPR_BINARY_OP_KIND::OP_KIND_SHR;
  if(is_token(TOKEN_SHL))	return EXPR_BINARY_OP_KIND::OP_KIND_SHL;
    
  printf("ERROR: undefined binary operation symbol.\n");
  exit(1);
}
EXPR_CMP_KIND make_cmp_kind(){
  if(is_token(TOKEN_LESS))	return EXPR_CMP_KIND::LT;
  if(is_token(TOKEN_LESSEQ))	return EXPR_CMP_KIND::LTE;
  if(is_token(TOKEN_GREATER))	return EXPR_CMP_KIND::GT;
  if(is_token(TOKEN_GREATEREQ)) return EXPR_CMP_KIND::GTE; 
  if(is_token(TOKEN_CMP_EQ))	return EXPR_CMP_KIND::EQ;
  if(is_token(TOKEN_CMP_NEQ))   return EXPR_CMP_KIND::NEQ;
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
    .name = "<built-in_integer>",
    .as   = {      
      .INT = val
    }
  };
}
Expr* expr_string(const char* str){
  return new Expr{
    .kind = EXPRKIND_STRING_LITERAL,
    .name = "<built-in_string>",
    .as   = {      
      .STRING = str
    }
  };
}
Expr* expr_reasign(Token token, Expr* from, Expr* to){
  Expr* rs = new Expr;
  rs->as.Reasign.token = token;
  rs->as.Reasign.from  = from;
  rs->as.Reasign.to    = to;
  rs->kind = EXPRKIND_REASIGN;
  return rs;
}
void print_ast(Decl** ast);
void print_expr(Expr* e){
  if(!e) return;
  switch(e->kind){
  case EXPRKIND_LOCAL_IF:
    printf("TODO print local if\n");
    break;
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
    printf(") %s (", human_expr_cmp_kind(e->as.comparasion.op));
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
    print_type(e->as.cast.type->type);
    printf("]::[");
    print_expr(e->as.cast.expr);
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
	   e->as.call.p_name);
    for(size_t i=0;i<buf__len(e->as.call.args);++i){
      printf((i>0)?", ":"");
      print_expr(e->as.call.args[i]);
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
  case EXPRKIND_NEG:
    printf("Neg(");
    print_expr(e->as.expr);
    printf(")");
    break;
  case EXPRKIND_NAMESPACE_GET:
    printf("Get namespace '%s' [ ", e->as.NamespaceGet.name);
    print_expr(e->as.NamespaceGet.rhs);
    printf(" ]\n");
    
    break;
  case EXPRKIND_ARRAY_ACCESS:
    printf("Array access\n");
    break;

  case EXPRKIND_FIELD_ACCESS:
    printf("EXPRKIND_FIELD_ACCESS");
    break;
  case EXPRKIND_NONE:
  default:
    printf("Unexpected ExprKind\n");
    exit(1);
  }
}
void print_type(Type* t){
  if(!t) return;
  switch(t->kind){
  case TYPE_PTR:
    printf("ptr_to(");
    print_type(t->ptr.base);
    printf(")");
    break;

  case TYPE_UNSOLVED:
    printf("[unsolved: %s]", t->name);
    break;    
  case TYPE_NONE:
    printf("none");
    break;
  case TYPE_ANY:
    printf("any");
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
  case TYPE_ENUM:
  default:
    printf("Unexpected TypeFieldKind\n");
    exit(1);
  }
}

void print_typespec(TypeField* k){
  printf("[%s: ", k->name);
  print_type(k->type);
  printf("]");
}
void print_stmt(Stmt* s){
  switch(s->kind){
  case STMTKIND_BLOCK:
    printf("block [ \n");
    ident++;
    print_stmt_list(s->as.stmts);
    ident--;
    ident();
    printf("]\n");
    break;
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
  case STMTKIND_BREAK:
    ident();
    printf("[kw-break]\n");
    break;
  case STMTKIND_CONTINUE:
    ident();
    printf("[kw-continue]\n");
    break;
  case STMTKIND_SWITCH:
  case STMTKIND_STMT:
  case STMTKIND_DO_WHILE:
  case STMTKIND_FOR:
  default:
    printf("Unexpected StmtKind\n");
    exit(1);
  }
}
void print_stmt_list(Block*  b){
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
    for(size_t i = 0; i < buf__len(d->as.procDecl.args->vars); ++i){
      ident();
      Var* var = d->as.procDecl.args->vars[i];
      print_typespec(var->type_field);
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

  case DeclKind::DECL_TYPEDEF:
    printf("Typedef\n");
    break;
  case DeclKind::DECL_IMPORT:
    print_ast(d->as.Import.ast);
    break;
  case DeclKind::DECL_NAMESPACE:
    printf("Namespace\n");
    break;
    return;

  case DeclKind::DECL_UNION:
    printf("union\n");
    return;
  case DeclKind::DECL_ENUM:

  default:
    
    printf("Unexpected DeclKind\n");
    return;//exit(1);
  }
  // NOTE: for now declarations can not be nested
  ident = 0;
  printf("\n");
  //assert(ident == 0 && "THIS MIGHT BE A BUG IN THE IDENT LOGIC");
}
void print_ast(Decl** ast){
  for(size_t i=0; i<buf__len(ast); ++i){
    print_decl(ast[i]);
    
  }
}

typedef Decl** AST_ROOT;
typedef Decl*  AST_NODE;


#endif /*__ast */

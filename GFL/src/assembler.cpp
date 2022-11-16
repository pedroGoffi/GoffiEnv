#ifndef __assembler
#define __assembler
#include "./ast.cpp"
#define GFL_RET_STACK_CAP 4096
static void gen_expr(Expr* expr);

Var** local_vars = NULL;
// UNIMPLEMENTED Var** global_vars = NULL; 
Var* Var_get(Var*** vars, const char* name){
  for(size_t i=0; i < buf__len(*vars); ++i){
    if(STR_CMP((*vars)[i]->type_field->name, name)){
      return (*vars)[i];
    }
  }
  return NULL;
}
void Var_push(Var*** vars, Var* var){
  assert(!Var_get(vars, var->type_field->name));

  buf__push(*vars, new Var{
      .type_field = var->type_field,
      .offset     = var->offset,
      .expr       = var->expr
    });
}
Var* Var_from_expr(Expr* expr){  
  return Var_get(&local_vars, expr->name);
}
#define unreachable()					\
  printf("ERROR: unrechable %s.\n", __FUNCTION__);	\
  exit(1);
static FILE* output_file;
static int   depth;

static Decl* current_decl;
static const char* argreg8[]  = {"dil", "sil", "dl", "cl", "r8b", "r9b"};
static const char *argreg16[] = {"di", "si", "dx", "cx", "r8w", "r9w"};
static const char *argreg32[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
static const char *argreg64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};


static int count(void) {
  static int i = 1;
  return i++;
}
__attribute__((format(printf, 1, 2)))
static void println(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(output_file, fmt, ap);
  va_end(ap);
  fprintf(output_file, "\n");
}
static void push(void){
  println("\tpush rax");
  depth++;
}
static void pop(const char* reg){
  println("\tpop %s", reg);
  depth--;
}
static const char *reg_dx(int sz) {
  switch (sz) {
  case 1: return "dl";
  case 2: return "dx";
  case 4: return "edx";
  case 8: return "rdx";
  }
  unreachable();
}

static const char *reg_ax(int sz) {
  switch (sz) {
  case 1: return "al";
  case 2: return "ax";
  case 4: return "eax";
  case 8: return "rax";
  }
  unreachable();
}
static void load(Type* ty){
  assert(ty->kind == TYPE_I64);
  if(ty->size != 4){
    unreachable();
  }
  println("\tmov rax, [rax]");
}
static void store(Type* ty){
  pop("rdi");
  assert(ty->kind == TYPE_I64);
  if(ty->size != 4){
    unreachable();
  }
  println("\tmov [rdi], rax");
}
static Var* Expr_get_var(Expr* expr){
  unreachable();
}
static void gen_addr(Var* var){
  TypeField* tf = var->type_field;
  assert(tf->type->kind == TYPE_I64);
  println("\tlea rax, QWORD [rbp - %i]", (int)var->offset);
}
static bool is_builin_procedure(Call* call){
  if(STR_CMP(call->p_name, "__print")){
    assert(call->args_size == 1);
    gen_expr(call->args[0]);
    println("\tmov rdi, rax");
    println("\tcall __print");
    return true;
  }
  return false;
}
static void cmp_zero(Type *ty) {
  switch (ty->kind) {
  //case TYPE_F32:
  //  println("  xorps %%xmm1, %%xmm1");
  //  println("  ucomiss %%xmm1, %%xmm0");
  //  return;
  case TYPE_F64:
    println("  xorpd %%xmm1, %%xmm1");
    println("  ucomisd %%xmm1, %%xmm0");
    return;
  }
  //if (is_integer(ty) && ty->size <= 4)
  //  println("  cmp eax, 0");

  println("\tcmp rax, 0");
}
static void gen_expr(Expr* expr){
  switch(expr->kind){
  case EXPRKIND_INT:
    println("\tmov rax, %zu", (size_t)expr->as.INT);
    break;
  case EXPRKIND_NAME: {
    
    if(Var* lv = Var_get(&local_vars, expr->name)){
      gen_addr(lv);
      load(lv->type_field->type);
      return;
    }
    else if (Macro* macro = GFL_Macros_find(expr->name)){
      gen_expr(macro->expr);
      return;
    }
    else {
      fprintf(stderr,
	      "ERROR: %s is not declared in this scope.\n",
	      expr->name);
      exit(1);
    }
  } break;
  case EXPRKIND_PROC_CALL: {
    if(is_builin_procedure(&expr->as.call)) return;
    printf("ERROR: '%s' procedures are not implemented yet.\n",
	   expr->as.call.p_name);
    exit(1);
  } break;
  case EXPRKIND_REASIGN: {
    Var* from = Var_from_expr(expr->as.Reasign.from);
    
    gen_addr(from);
    push();
    gen_expr(expr->as.Reasign.to);
    store(from->type_field->type);    
  } break;
  case EXPR_BINARY_OP: {
#define bin(op, reg_a, reg_b) println("\t%s %s, %s", op, reg_a, reg_b)
    const char* op;
    switch(expr->as.BinaryOp.op){
    case OP_KIND_PLUS:
      op = "add";
      break;
    case OP_KIND_MINUS:
      op = "sub";
      break;
    default:
      unreachable();
    }
    static size_t e_d = 0;
    e_d += 2;
    gen_expr(expr->as.BinaryOp.lhs);
    push();
    gen_expr(expr->as.BinaryOp.rhs);
    e_d -= 2;
    // TODO: optimize this section
    if(e_d == 0){
      pop("rbx");
      bin(op, "rax", "rbx");      
    }
    else {
      push();
      pop("rax");
      pop("rbx");
      bin(op, "rax", "rbx");
    }   
#undef bin
  } break;
    
  default:
    print_expr(expr);
    printf("\n");
    unreachable();
  }
}
size_t calc_offset(size_t offset){
  static size_t _offset = 0;
  _offset += offset;
  return _offset;
}
static void gen_stmt(Stmt* stmt){
  switch(stmt->kind){
  case STMTKIND_EXPR:
    gen_expr(stmt->as.expr);
    break;
  case STMTKIND_IF: {
    int if_lb = count();
    int end_lb = if_lb;
    gen_expr(stmt->as.__if.expr);
    // TODO: embbed type in expr
    cmp_zero(Type_int());

    println("\tje .L.else.%i", if_lb);    
    for(size_t i=0; i < buf__len(stmt->as.__if.block->stmts); ++i){
      gen_stmt(stmt->as.__if.block->stmts[i]);
    }
    println("\tjmp .L.else.end.%i", end_lb);    
    println(".L.else.%i:", if_lb);
    Elif* elif = stmt->as.__if.elif_nodes;
    if(elif){
      for(size_t i=0; i < elif->nodes_size; ++i){
	if_lb = count();
	gen_expr(elif->node_expr[i]);
	cmp_zero(Type_int());
	println("\tje .L.else.%i", if_lb);
	for(size_t j=0; j < buf__len(elif->node_block[i]->stmts); ++j){
	  gen_stmt(elif->node_block[i]->stmts[j]);;
	}
	println("\tjmp .L.else.end.%i", end_lb);
	println(".L.else.%i:", if_lb);
      }
    }
    if(stmt->as.__if.else_block){
      for(size_t i=0; i < buf__len(stmt->as.__if.else_block->stmts); ++ i){
    	gen_stmt(stmt->as.__if.else_block->stmts[i]);
      }
    }
    println(".L.else.end.%i:", end_lb);
  } break;
  case STMTKIND_RETURN:
    if(stmt->as.expr){
      gen_expr(stmt->as.expr);      
    }
    assert(current_decl);
    println("\tjmp .L.return.%s", current_decl->as.procDecl.name);
    break;
  case STMTKIND_LOCAL_VAR:
    stmt->as.var->offset = calc_offset(stmt->as.var->offset);
    Var_push(&local_vars, stmt->as.var);
    if(stmt->as.var->expr){
      Var* lv = stmt->as.var;
      gen_addr(lv);
      push();
      gen_expr(lv->expr);
      store(lv->type_field->type);
    }
    break;
  case STMTKIND_BREAK:
  case STMTKIND_CONTINUE:
  case STMTKIND_STMT:
  case STMTKIND_WHILE:    
  case STMTKIND_FOR:   
  case STMTKIND_DO_WHILE: 
  case STMTKIND_SWITCH:
  default:
    unreachable();
  }
}
static void gen_proc_prologue(Decl* procDecl){
  println("\tsection .text");
  println("%s:", procDecl->as.procDecl.name);
  println("\tpush rbp");
  println("\tmov rbp, rsp");
  println("\tsub rsp, %zu", procDecl->as.procDecl.stack_allocation);
  if(procDecl->as.procDecl.args->argsList_size > 0){
    fprintf(stderr,
	    "ERROR: procedure params are not implemented yet.\n");
    exit(1);
  }
  
}
void AssemblerPROC(Decl* decl){
  gen_proc_prologue(decl);
  for(size_t idx=0; idx < decl->as.procDecl.block->stmts_size; ++idx){   
    Stmt* stmt = decl->as.procDecl.block->stmts[idx];
    gen_stmt(stmt);
    printf("ok\n");
  }
  if(STR_CMP(decl->as.procDecl.name, "main")){
    printf("MAIN\n\n");
    println("\tmov rax, 0");
  }
  println(".L.return.%s:", decl->as.procDecl.name);
  println("\tmov rsp, rbp");
  println("\tpop rbp");
  println("\tret");
  
}
void AssemblerAST(Decl** ast, const char* output_fp){
  output_file = fopen(output_fp, "w");
  assert(output_file);
  println("\tglobal _start");
  println("__print:");
  println("\tpush  rbp");
  println("\tmov   rbp, rsp");
  println("\tsub   rsp, 64");
  println("\tmov   QWORD  [rbp-56], rdi");
  println("\tmov   QWORD  [rbp-8], 1");
  println("\tmov   eax, 32");
  println("\tsub   rax, QWORD  [rbp-8]");
  println("\tmov   BYTE  [rbp-48+rax], 10");
  println(".__print_loop:");
  println("\tmov   rcx, QWORD  [rbp-56]");
  println("\tmov   rdx, -3689348814741910323");
  println("\tmov   rax, rcx");
  println("\tmul   rdx");
  println("\tshr   rdx, 3");
  println("\tmov   rax, rdx");
  println("\tsal   rax, 2");
  println("\tadd   rax, rdx");
  println("\tadd   rax, rax");
  println("\tsub   rcx, rax");
  println("\tmov   rdx, rcx");
  println("\tmov   eax, edx");
  println("\tlea   edx, [rax+48]");
  println("\tmov   eax, 31");
  println("\tsub   rax, QWORD  [rbp-8]");
  println("\tmov   BYTE  [rbp-48+rax], dl");
  println("\tadd   QWORD  [rbp-8], 1");
  println("\tmov   rax, QWORD  [rbp-56]");
  println("\tmov   rdx, -3689348814741910323");
  println("\tmul   rdx");
  println("\tmov   rax, rdx");
  println("\tshr   rax, 3");
  println("\tmov   QWORD  [rbp-56], rax");
  println("\tcmp   QWORD  [rbp-56], 0");
  println("\tjne   .__print_loop");
  println("\tmov   eax, 32");
  println("\tsub   rax, QWORD  [rbp-8]");
  println("\tlea   rdx, [rbp-48]");
  println("\tlea   rcx, [rdx+rax]");
  println("\tmov   rax, QWORD  [rbp-8]");
  println("\tmov   rdx, rax");
  println("\tmov   rsi, rcx");
  println("\tmov   edi, 1");
  println("\tmov   rax, 1");
  println("\tsyscall");
  println("\tnop");
  println("\tleave");
  println("\tret");
  println("global _start");
  println("segment .text");	
  assert(buf__len(ast) == 1);
  assert(ast[0]->kind == DECL_PROC);
  current_decl = ast[0];
  AssemblerPROC(ast[0]);
  println("_start:");
  println("\tcall main");
  println("\tmov rdi, rax");
  println("\tmov rax, 60");
  println("\tsyscall");
}

#endif /* __assembler */

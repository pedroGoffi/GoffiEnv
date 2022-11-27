#ifndef __assembler
#define __assembler
#include "./ast.cpp"

// compiler behaviour vars
static bool  load_vars = true;
static bool  ns_ctx    = false;
static char* ns_name   = NULL;

int     DEBUG_COUNT = 0;
#define DEBUG_OK							\
  if(1) printf("[%i]: PASSED BY %s\n", DEBUG_COUNT++, __FUNCTION__);
#define unimplemented(st)				\
  printf("UNIMEPLMENTED %s: %s\n", __FUNCTION__, #st);	\
  exit(1);
#define unreachable()					\
  printf("ERROR: unrechable %s.\n", __FUNCTION__);	\
  exit(1);

int align_to(int n, int align) {
  return (n + align - 1) / align * align;
}
static Type* gen_expr(Expr* expr);
static void gen_expr_cmp(Expr* expr);

size_t rbp_stack_offset = 0;
Var**  local_vars	= NULL;
Var**  global_vars	= NULL;
const char** strings	= NULL;
Proc** procs		= NULL;
Proc* Proc_get(Proc*** procs, const char* name){
  for(size_t i=0; i < buf__len(*procs); ++i){
    if(STR_CMP((*procs)[i]->name, name))
      return (*procs)[i];
  }
  return NULL;
}
Var* Var_get(Var*** vars, const char* name){
  for(size_t i=0; i < buf__len(*vars); ++i){
    if(STR_CMP((*vars)[i]->type_field->name, name)){
      return (*vars)[i];
    }
  }
  return NULL;
}
void  Proc_push(Proc*** procs, Decl* decl){
  assert(decl->kind == DECL_PROC);
  if(Proc_get(procs, decl->name)){
    fprintf(stderr,
	    "ERROR: the procedure '%s' was already declared.\n",
	    decl->name);
    exit(1);
  }

  buf__push(*procs, &decl->as.procDecl);
  
}

void Var_push(Var*** vars, Var* var){
  assert(!Var_get(vars, var->type_field->name));
  buf__push(*vars, var);
}
Var* Var_from_expr(Expr* expr){
  if(expr->kind != EXPRKIND_NAME){
    unreachable();
  }
  Var* lv = Var_get(&local_vars, expr->name);
  if(lv) return lv;
  
  Var* gv = Var_get(&global_vars, expr->name);
  if(gv) return gv;
  
  unreachable();
}
static FILE* output_file;
static int   depth;
static Decl* current_decl;

static const char* argreg8[]  = {"dil", "sil", "dl", "cl", "r8b", "r9b"};
static const char *argreg16[] = {"di", "si", "dx", "cx", "r8w", "r9w"};
static const char *argreg32[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
static const char *argreg64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
#define MAX_ARGREG64 6

//// The table for type casts
//static char i32i8[] = "movsbl %al, %eax";
//static char i32u8[] = "movzbl %al, %eax";
//static char i32i16[] = "movswl %ax, %eax";
//static char i32u16[] = "movzwl %ax, %eax";
//static char i32f32[] = "cvtsi2ssl %eax, %xmm0";
//static char i32i64[] = "movsxd %eax, %rax";
//static char i32f64[] = "cvtsi2sdl %eax, %xmm0";
//static char i32f80[] = "mov %eax, -4(%rsp); fildl -4(%rsp)";
//
//static char u32f32[] = "mov %eax, %eax; cvtsi2ssq %rax, %xmm0";
//static char u32i64[] = "mov %eax, %eax";
//static char u32f64[] = "mov %eax, %eax; cvtsi2sdq %rax, %xmm0";
//static char u32f80[] = "mov %eax, %eax; mov %rax, -8(%rsp); fildll -8(%rsp)";
//
//static char i64f32[] = "cvtsi2ssq %rax, %xmm0";
//static char i64f64[] = "cvtsi2sdq %rax, %xmm0";
//static char i64f80[] = "movq %rax, -8(%rsp); fildll -8(%rsp)";
//
//static char u64f32[] = "cvtsi2ssq %rax, %xmm0";
//static char u64f64[] =
//  "test %rax,%rax; js 1f; pxor %xmm0,%xmm0; cvtsi2sd %rax,%xmm0; jmp 2f; "
//  "1: mov %rax,%rdi; and $1,%eax; pxor %xmm0,%xmm0; shr %rdi; "
//  "or %rax,%rdi; cvtsi2sd %rdi,%xmm0; addsd %xmm0,%xmm0; 2:";
//static char u64f80[] =
//  "mov %rax, -8(%rsp); fildq -8(%rsp); test %rax, %rax; jns 1f;"
//  "mov $1602224128, %eax; mov %eax, -4(%rsp); fadds -4(%rsp); 1:";
//
//static char f32i8[] = "cvttss2sil %xmm0, %eax; movsbl %al, %eax";
//static char f32u8[] = "cvttss2sil %xmm0, %eax; movzbl %al, %eax";
//static char f32i16[] = "cvttss2sil %xmm0, %eax; movswl %ax, %eax";
//static char f32u16[] = "cvttss2sil %xmm0, %eax; movzwl %ax, %eax";
//static char f32i32[] = "cvttss2sil %xmm0, %eax";
//static char f32u32[] = "cvttss2siq %xmm0, %rax";
//static char f32i64[] = "cvttss2siq %xmm0, %rax";
//static char f32u64[] = "cvttss2siq %xmm0, %rax";
//static char f32f64[] = "cvtss2sd %xmm0, %xmm0";
//static char f32f80[] = "movss %xmm0, -4(%rsp); flds -4(%rsp)";
//
//static char f64i8[] = "cvttsd2sil %xmm0, %eax; movsbl %al, %eax";
//static char f64u8[] = "cvttsd2sil %xmm0, %eax; movzbl %al, %eax";
//static char f64i16[] = "cvttsd2sil %xmm0, %eax; movswl %ax, %eax";
//static char f64u16[] = "cvttsd2sil %xmm0, %eax; movzwl %ax, %eax";
//static char f64i32[] = "cvttsd2sil %xmm0, %eax";
//static char f64u32[] = "cvttsd2siq %xmm0, %rax";
//static char f64i64[] = "cvttsd2siq %xmm0, %rax";
//static char f64u64[] = "cvttsd2siq %xmm0, %rax";
//static char f64f32[] = "cvtsd2ss %xmm0, %xmm0";
//static char f64f80[] = "movsd %xmm0, -8(%rsp); fldl -8(%rsp)";
//
//#define FROM_F80_1                                           \
//  "fnstcw -10(%rsp); movzwl -10(%rsp), %eax; or $12, %ah; " \
//  "mov %ax, -12(%rsp); fldcw -12(%rsp); "
//
//#define FROM_F80_2 " -24(%rsp); fldcw -10(%rsp); "
//
//static char f80i8[] = FROM_F80_1 "fistps" FROM_F80_2 "movsbl -24(%rsp), %eax";
//static char f80u8[] = FROM_F80_1 "fistps" FROM_F80_2 "movzbl -24(%rsp), %eax";
//static char f80i16[] = FROM_F80_1 "fistps" FROM_F80_2 "movzbl -24(%rsp), %eax";
//static char f80u16[] = FROM_F80_1 "fistpl" FROM_F80_2 "movswl -24(%rsp), %eax";
//static char f80i32[] = FROM_F80_1 "fistpl" FROM_F80_2 "mov -24(%rsp), %eax";
//static char f80u32[] = FROM_F80_1 "fistpl" FROM_F80_2 "mov -24(%rsp), %eax";
//static char f80i64[] = FROM_F80_1 "fistpq" FROM_F80_2 "mov -24(%rsp), %rax";
//static char f80u64[] = FROM_F80_1 "fistpq" FROM_F80_2 "mov -24(%rsp), %rax";
//static char f80f32[] = "fstps -8(%rsp); movss -8(%rsp), %xmm0";
//static char f80f64[] = "fstpl -8(%rsp); movsd -8(%rsp), %xmm0";
//
//static char *cast_table[][11] = {
//  // i8   i16     i32     i64     u8     u16     u32     u64     f32     f64     f80
//  {NULL,  NULL,   NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i8
//  {i32i8, NULL,   NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i16
//  {i32i8, i32i16, NULL,   i32i64, i32u8, i32u16, NULL,   i32i64, i32f32, i32f64, i32f80}, // i32
//  {i32i8, i32i16, NULL,   NULL,   i32u8, i32u16, NULL,   NULL,   i64f32, i64f64, i64f80}, // i64
//
//  {i32i8, NULL,   NULL,   i32i64, NULL,  NULL,   NULL,   i32i64, i32f32, i32f64, i32f80}, // u8
//  {i32i8, i32i16, NULL,   i32i64, i32u8, NULL,   NULL,   i32i64, i32f32, i32f64, i32f80}, // u16
//  {i32i8, i32i16, NULL,   u32i64, i32u8, i32u16, NULL,   u32i64, u32f32, u32f64, u32f80}, // u32
//  {i32i8, i32i16, NULL,   NULL,   i32u8, i32u16, NULL,   NULL,   u64f32, u64f64, u64f80}, // u64
//
//  {f32i8, f32i16, f32i32, f32i64, f32u8, f32u16, f32u32, f32u64, NULL,   f32f64, f32f80}, // f32
//  {f64i8, f64i16, f64i32, f64i64, f64u8, f64u16, f64u32, f64u64, f64f32, NULL,   f64f80}, // f64
//  {f80i8, f80i16, f80i32, f80i64, f80u8, f80u16, f80u32, f80u64, f80f32, f80f64, NULL},   // f80
//};

static int count(void) {
  static int i = 1;
  return i++;
}
#define println(...)				\
  {						\
    fprintf(output_file, __VA_ARGS__);		\
    fprintf(output_file, "\n");			\
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
static const char* word_from_size(size_t sz){
  if(sz == 1) return "byte";
  if(sz == 2) return "word";
  if(sz == 4) return "dword";
  if(sz == 8) return "qword";  
  return "";
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
static Type* load(Type* ty){
  
  switch(ty->kind){
  case TYPE_I64: {
    const char* inst = ty->is_unsigned? "movz" : "movs";
    // TODO: sz 1 and 2
    if(ty->size == 1){
      println("\t%sx rax, %s [rax]", inst,
	      word_from_size(ty->size));      
    } else if (ty->size == 2){
      println("\t%sx rax, %s [rax]", inst,
	      word_from_size(ty->size));
    }
    else if (ty->size == 4){
      println("\tmovsxd rax, %s [rax]",
	      word_from_size(ty->size));
    }
    else {
      println("\tmov rax, [rax]");
    }
  } break;
  case TYPE_PTR:
    println("\tmov rax, %s [rax]",
	    word_from_size(ty->size));
    return ty->ptr.base;
  case TYPE_CHAR:
    println("\tmovzx eax, BYTE [eax]");
    break;
  case TYPE_ARRAY: break;
  default:
    unreachable();
  }
  return ty;
}

static void store(Type* ty){
  // the register rax carry the var value 
  pop("rdi"); // var register
  switch(ty->kind){
    
  case TYPE_PTR: 
  case TYPE_I64:
  case TYPE_ARRAY:
  case TYPE_CHAR:
    // TODO
    if (ty->size == 1){
      println("\tmov %s [rdi], al", word_from_size(ty->size));
    } else if (ty->size == 2){
      println("\tmov %s [rdi], ax", word_from_size(ty->size));
    } else if (ty->size == 4){
      println("\tmov %s [rdi], eax", word_from_size(ty->size));
    } else {
      println("\tmov %s [rdi], rax", word_from_size(ty->size));
    }
    break;
  default:
    unreachable();
  }    
}
static Var* Expr_get_var(Expr* expr){
  unreachable();
}
static int push_callargs(Expr** args){
  int stack=0;

  for(size_t i=0; i < buf__len(args); ++i){
    if( i >= MAX_ARGREG64 ){      
      for(size_t j=i; j < buf__len(args); ++j){
	gen_expr(args[j]);
	push();
	stack++;
      }
      return stack;
    }
    gen_expr(args[i]);
    println("\tmov %s, rax", argreg64[i]);
  }
  return stack;
}

static void gen_addr(Var* var){
  TypeField* tf = var->type_field;
  switch(tf->type->kind){
  case TYPE_PTR:
  case TYPE_I64:    
    println("\tlea rax, %s [rbp - %i]",
	    word_from_size(tf->type->size),
	    (int)var->offset);
    
    break;
  case TYPE_ARRAY:
    break;
  case TYPE_CHAR:
    println("\tlea eax, %s [ebp - %i]",
	    word_from_size(tf->type->size),
	    (int)var->offset);
    break;
  default:
    unreachable();
  }
}
static void set_callargs_passed_by_stacK_range(ProcArgs* pa, size_t begin){
  for(size_t i=begin; i < buf__len(pa->vars); ++i){
    pa->vars[i]->passed_by_stack = true;    
  }
}
static void store_callargs(ProcArgs* pa){
  Var** vargs = pa->vars;  
  for(size_t i=0; i < buf__len(vargs); ++i){
    Var* arg = vargs[i];
    if(i >= MAX_ARGREG64) {      
      rbp_stack_offset = vargs[i - 1]->offset;
      set_callargs_passed_by_stacK_range(pa, i);
      break;
    }
    Type* ty = arg->type_field->type;
    switch(ty->kind){
    case TYPE_PTR:
    case TYPE_I64:
      // TODO
      if (ty->size == 1){
	println("\tmov [rbp - %zu], %s",
		arg->offset,
		argreg8[i]);
	
      } else if (ty->size == 2){    
	println("\tmov [rbp - %zu], %s",
		arg->offset,
		argreg16[i]);

      } else if (ty->size == 4){    
	println("\tmov [rbp - %zu], %s",
		arg->offset,
		argreg32[i]);
      } else {			    
	println("\tmov [rbp - %zu], %s",
		arg->offset,
		argreg64[i]);
      }
      break;
    default:
      unreachable();
    }
  }
  DEBUG_OK;
}
static void init_args(ProcArgs* pargs){
  for(size_t i=0; i < buf__len(pargs->vars); ++i){
    Var_push(&local_vars, pargs->vars[i]);
  }
}


static bool test_make_syscall(Call* call, const char* estr, int sys_len){
  if(STR_CMP(call->p_name, estr)){
    assert(call->args_size == sys_len);
    int LAST_REG = sys_len - 1;
    for(int i = 0; i < LAST_REG; ++i){
      gen_expr(call->args[i]);
      println("\tmov %s, rax", argreg64[i]);
    }
    gen_expr(call->args[LAST_REG]);    
    println("\tsyscall");
    return true;
  }
  return false;
}
static bool is_builin_procedure(Call* call){
#define DOIF(_name, ...) \
  if(STR_CMP(call->p_name, _name)) {{__VA_ARGS__} return true;}
  DOIF("__print__", {
      for(size_t i=0; i < call->args_size; ++i){;
	gen_expr(call->args[i]);
	println("\tmov rdi, rax");
	println("\tcall __print");
      }
    });
  DOIF("__asm__", {
      for(size_t i=0; i < call->args_size; ++i){
	assert(call->args[i]->kind = EXPRKIND_STRING_LITERAL);
	const char* _asm_ = call->args[i]->as.STRING;
	println("\t%s", _asm_);
      }
      
    });
  DOIF("__alloc__", {
      static int alloc_count = 1;
      assert(buf__len(call->args) == 1);
      gen_expr(call->args[0]);
      println("\tmov rdi, rax");
      println("\tadd rdi, 15");
      println("\tand edi, 0xfffffff0");
      println("\tmov rcx, [rbp + %i]", current_proc->stack_allocation);
      println("\tsub rcx, rsp");
      println("\tmov rax, rsp");
      println("\tsub rsp, rdi");
      println("\tmov rdx, rsp");
      println(".L.ALLOC.%i:", alloc_count);
      println("\tcmp r8, 0");
      println("\tje .L.ALLOC.END.%i", alloc_count);
      println("\tmov r8, rax");
      println("\tmov rdx, r8");
      println("\tinc rdx");
      println("\tinc rax");
      println("\tdec rcx");
      println("\tjmp .L.ALLOC.%i", alloc_count);
      println(".L.ALLOC.END.%i:", alloc_count);
      println("\tmov rax, [rbp + %i]", current_proc->stack_allocation);
      println("\tsub rax, rdi");
      println("\tmov [rbp + %i], rax", current_proc->stack_allocation);
      alloc_count++;
    });
#define DOIF_SYSCALL(sys_len)\
  if(test_make_syscall(call, "SYSCALL" #sys_len, sys_len + 1)) return true;
  
  DOIF_SYSCALL(0);
  DOIF_SYSCALL(1);
  DOIF_SYSCALL(2);
  DOIF_SYSCALL(3);
  DOIF_SYSCALL(4);
  DOIF_SYSCALL(5);
  DOIF_SYSCALL(6);
   
     
#undef DOIF
#undef DOIF_SYSCALL
  return false;
}
static void cmp_zero(Type *ty) {
  switch (ty->kind) {
  case TYPE_I64:
    println("\tcmp rax, 0");
    return;
  }
  unreachable();  
}
static void gen_expr_cmp(Expr* expr){
  static int cmp_count = 0;
  if(expr->kind != EXPR_COMPARASION){
    gen_expr(expr);
    return;
  }
  Expr*         lhs = expr->as.comparasion.lhs;
  Expr*         rhs = expr->as.comparasion.rhs;      
  EXPR_CMP_KIND  op = expr->as.comparasion.op;
  gen_expr(rhs);
  push();
  gen_expr(lhs);
  pop("rbx");
  // rax = lhs
  // rbx = rhs

  println("\tcmp rax, rbx");
  switch(op){
  case LT:
    println("\tjge .L.CMP.UNMATCH.%i", cmp_count);
    break;
  case LTE:
    println("\tjg .L.CMP.UNMATCH.%i", cmp_count);
    break;    
  case EQ:
    println("\tjne .L.CMP.UNMATCH.%i", cmp_count);
    break;    
  case NEQ:
    println("\tje .L.CMP.UNMATCH.%i", cmp_count);
    break;        
  case GTE:
    println("\tjb .L.CMP.UNMATCH.%i", cmp_count); 
    break;    
  case GT:
    println("\tjbe .L.CMP.UNMATCH.%i", cmp_count);
                   
    break;
  default:
    unreachable();
  }
  println("\tmov rax, 1");
  println("\tjmp .L.CMP.END%i", cmp_count);
  println(".L.CMP.UNMATCH.%i:", cmp_count);
  println("\tmov rax, 0");
  println(".L.CMP.END%i:", cmp_count);
  cmp_count++;
}

static Type* gen_expr(Expr* expr){
  switch(expr->kind){
  case EXPRKIND_INT:
    println("\tmov rax, %zu", (size_t)expr->as.INT);
    return Type_int();
    
  case EXPRKIND_STRING_LITERAL: {
    static int str_count = 0;
    buf__push(strings, expr->as.STRING);
    println("\tmov rax, DATA%i", str_count);
    str_count++;
  } return Type_ptr(Type_char());
    
  case EXPRKIND_NAME: {    
    if(Var* lv = Var_get(&local_vars, expr->name)){
      gen_addr(lv);
      if(load_vars) {
	load(lv->type_field->type);
	return lv->type_field->type;
      }
      return Type_ptr(lv->type_field->type);
    }
    else if(Var* gv = Var_get(&global_vars, expr->name)){
      println("\tmov rax, %s", gv->type_field->name);
      if(load_vars) {
	load(gv->type_field->type);
	return gv->type_field->type;
      }
      return Type_ptr(gv->type_field->type);
    }
    else if (Macro* macro = GFL_Macros_find(expr->name)){
      return gen_expr(macro->expr);
    }
    else {
      print_expr(expr);
      fprintf(stderr,
	      "ERROR: %s is not declared in this scope.\n",
	      expr->name);
      exit(1);
    }
  } break;
    
  case EXPRKIND_PROC_CALL: {
    if(is_builin_procedure(&expr->as.call)) return Type_none();    
    Call* call = &expr->as.call;    
    Proc* proc = Proc_get(&procs, call->p_name);    
    if(!proc){
      Sym* after_declares_proc = sym_get(call->p_name);
      
      if(!after_declares_proc){ 
	// Never declared
	fprintf(stderr,
		"ERROR: the procedure '%s' was not declared in this scope.\n",
		call->p_name);
	exit(1);
      }
      // Declared after the call
      proc = &after_declares_proc->decl->as.procDecl;
    }      
    if(proc->args){
      size_t call_argsSize = call->args_size;
      size_t proc_argsSize = proc->args->vars_size;
      if(call_argsSize != proc_argsSize){
	assert(call_argsSize < proc_argsSize);
	for(size_t i=call_argsSize; i < buf__len(proc->args->vars); ++i){
	  Var* param = proc->args->vars[i];
	  if(param->expr){
	    buf__push(call->args, param->expr);
	  }
	  else {
	    fprintf(stderr,
		    "ERROR: expected %zu arguments but got %zu when calling procedure '%s'\n",
		    proc_argsSize,
		    call_argsSize,
		    proc->name);
	    exit(1);
	  }
	}	
      }
    }
    int stack_args = push_callargs(call->args);
    int gp = 0, fp = 0;    
    println("\tcall %s", call->p_name);
    if(stack_args > 0) {
      println("\tadd rsp, %i", stack_args * 8);
      rbp_stack_offset = 0;
    }
    // TODO: search on why add rsp after call
    //int dealloc_size = (stack_args - 2) * 8;
    //if(dealloc_size > 0) println("\tadd rsp, %i", dealloc_size);
    
    depth -= stack_args;
    return proc->ret_type;
  } break;
  case EXPRKIND_NAMESPACE_GET: {
    assert(false && "unimplemented");
  } break;
  case EXPRKIND_DERREF_NAME: {
    Type* type = gen_expr(expr->as.derref);
    if(type->kind != TYPE_PTR){
      fprintf(stderr,
	      "ERROR: trying to derreference a non-pointer value.\n");
      exit(1);
    }    
    return load(type->ptr.base);    
  } break;
  case EXPRKIND_ADDROF_NAME: {
    // TODO: check if is a lvalue // non memory addr
    load_vars = false;
    Type* type = gen_expr(expr->as.addr_of);
    load_vars = true;
    return type;
  } break;
  case EXPRKIND_REASIGN: {    
    load_vars = false;
    Type* tfrom = gen_expr(expr->as.Reasign.from);
    load_vars = true;
    push();
    gen_expr(expr->as.Reasign.to);
    store(tfrom);
    return tfrom;
  } break;
  case EXPR_BINARY_OP: {
#define bin(op, reg_a, reg_b) println("\t%s %s, %s", op, reg_a, reg_b)
    const char* op;
    // NOTE: e_d stands for expression depth
    static size_t e_d = 0;
    e_d += 2;
    gen_expr(expr->as.BinaryOp.rhs);
    push();
    gen_expr(expr->as.BinaryOp.lhs);
    e_d -= 2;
    switch(expr->as.BinaryOp.op){
    case OP_KIND_PLUS:
      op = "add";
      break;
    case OP_KIND_MINUS:
      op = "sub";
      break;
    case OP_KIND_DIV:

      if(e_d == 0){
	pop("rbx");
      }
      else {
	pop("rax");
	pop("rbx");	
      }
      println("\tdiv rbx");	
      return Type_int();
    case OP_KIND_MULT:
      if(e_d == 0){
	pop("rbx");
	println("\tmul rbx");
      }
      else {
	pop("rax");
	pop("rbx");
	println("\tmul rbx");
      }
      return Type_int();
    default:
      unreachable();
    }        
    // TODO: optimize this section
    if(e_d == 0){
      pop("rbx");
    }
    else {
      push();
      pop("rbx");
      pop("rax");
    }
    bin(op, "rax", "rbx");
#undef bin
  } break;
  case EXPRKIND_CAST:
    gen_expr(expr->as.Cast.expr);
    return expr->as.Cast.type->type;
  case EXPR_COMPARASION: 
    gen_expr_cmp(expr);
    break;
  case EXPRKIND_LOCAL_IF: {
    static size_t local_if_count = 0;
    gen_expr(expr->as.local_if.cond);
    cmp_zero(Type_int());
    println("\tje  .L.LOCAL_IF_ELSE.%zu", local_if_count);
    gen_expr(expr->as.local_if.if_body);
    println("\tjmp .L.LOCAL_IF_END.%zu", local_if_count);
    println(".L.LOCAL_IF_ELSE.%zu:", local_if_count);
    gen_expr(expr->as.local_if.else_body);
    println(".L.LOCAL_IF_END.%zu:", local_if_count);
    local_if_count++;
  } break;
  case EXPRKIND_NEG:
    // TODO: expr with Type
    gen_expr(expr->as.expr);
    println("\tneg rax");
    break;
  default:
    print_expr(expr);
    unreachable();
  }
}

static void gen_stmt(Stmt* stmt){
  static int while_lb;
  switch(stmt->kind){
  case STMTKIND_EXPR:
    gen_expr(stmt->as.expr);
    break;
  case STMTKIND_BLOCK: {
    Block* block = stmt->as.stmts;
    for(size_t i=0; i < buf__len(block->stmts); ++i){
      gen_stmt(block->stmts[i]);
    }
  } break;
  case STMTKIND_IF: {
    int if_lb = count();
    int end_lb = if_lb;
    gen_expr(stmt->as.__if.expr);
    cmp_zero(Type_int());
    println("\tje .L.else.%i", if_lb);

    for(size_t i=0; i < buf__len(stmt->as.__if.block->stmts); ++i){
      gen_stmt(stmt->as.__if.block->stmts[i]);
    }
    println("\tjmp .L.else.end.%i", end_lb);    
    println(".L.else.%i:", if_lb);
    
    if(Elif* elif = stmt->as.__if.elif_nodes){
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
    if(current_decl->as.procDecl.ret_type->kind == TYPE_NONE){
      fprintf(stderr,
	      "ERROR: return only works in non-void procedures.\n");
      exit(1);
    }
    if(stmt->as.expr){
      gen_expr(stmt->as.expr);      
    }
    assert(current_decl);
    println("\tjmp .L.return.%s", current_decl->as.procDecl.name);
    break;
  case STMTKIND_LOCAL_VAR:
    Var_push(&local_vars, stmt->as.var);
    if(stmt->as.var->expr){
      Var* lv = stmt->as.var;
      gen_addr(lv);
      push();
      gen_expr(lv->expr);
      //store(v);
      store(lv->type_field->type);
    }
    break;
  case STMTKIND_WHILE:{
    while_lb = count();
    int saved_lb = while_lb;
    println(".L.LOOP_BEGIN.%i:", saved_lb);
    gen_expr(stmt->as.__while.expr);
    cmp_zero(Type_int());
    println("\tje .L.LOOP_BREAK.%i", saved_lb);
    
    for(size_t i=0; i < buf__len(stmt->as.__while.block->stmts); ++i){
      gen_stmt(stmt->as.__while.block->stmts[i]);
      while_lb = saved_lb;
    }
    println("\tjmp .L.LOOP_BEGIN.%i", saved_lb);
    println(".L.LOOP_BREAK.%i:", saved_lb);
  } break;
  case STMTKIND_BREAK:
    println("\tjmp .L.LOOP_BREAK.%i", while_lb);
    break;
  case STMTKIND_CONTINUE:
    println("\tjmp .L.LOOP_BEGIN.%i", while_lb);
    break;
  case STMTKIND_STMT:  
  case STMTKIND_FOR:   
  case STMTKIND_DO_WHILE: 
  case STMTKIND_SWITCH:
  default:
    print_stmt(stmt);
    unreachable();
  }
}
static void store_gp(int r, int offset, int sz){
  switch(sz){
  case 1:
    println("\tmov [rbp + %i], %s", offset, argreg8[r]);
    break;
  case 2:
    println("\tmov [rbp + %i], %s", offset, argreg16[r]);
    break;
  case 4:
    println("\tmov [rbp + %i], %s", offset, argreg32[r]);
    break;
  case 8:
    println("\tmov [rbp + %i], %s", offset, argreg64[r]);
    break;    
  default:
    for(int i = 0; i < sz; ++i){
      println("\tmov [rbp + %i], %s", offset + i, argreg8[r]);
      println("\tshr %s, 8", argreg64[r]);
    }
  }
}
static void gen_proc_params(Decl* procDecl){
  int gp = 0, fp = 0;
  ProcArgs* pa  = procDecl->as.procDecl.args;  
  
  if(pa){
    if(pa->vars_size > MAX_ARGREG64){
      fatal("Compiler can not handle more than %zu arguments, got %zu\n",
	    MAX_ARGREG64,
	    pa->vars_size);
      exit(1);
    }
    store_callargs(pa);
    DEBUG_OK;
  }
}
static void gen_proc_prologue(Decl* procDecl){
  println("\tglobal %s", procDecl->as.procDecl.name);
  println("\tsection .text");
  println("%s:", procDecl->as.procDecl.name);
  if(STR_CMP(procDecl->as.procDecl.name, "main")){
    Proc pmain = procDecl->as.procDecl;
    if(pmain.args->vars_size > 0){
      Var* argc = Var_get(&local_vars, pmain.args->vars[0]->type_field->name);
      Var* argv = Var_get(&local_vars, pmain.args->vars[1]->type_field->name);
      printf("TODO: argc|argv\n");
      exit(1);
    }
  }
  println("\tpush rbp");      
  println("\tmov  rbp, rsp");
  int alloc = align_to((int)procDecl->as.procDecl.stack_allocation, 8);
  if(alloc > 0) println("\tsub rsp, %i", alloc);
  gen_proc_params(procDecl); 
}
void AssemblerPROC(Decl* decl){
  // prologue
  gen_proc_prologue(decl);
  // block
  for(size_t idx=0; idx < decl->as.procDecl.block->stmts_size; ++idx){	
    Stmt* stmt = decl->as.procDecl.block->stmts[idx];			
    gen_stmt(stmt);							
  }
  // return
  println(".L.END_OF_PROC.__%s__:", decl->as.procDecl.name);
  if(STR_CMP(decl->as.procDecl.name, "main")){   
    println("\tmov eax, 0");
  }
  println(".L.return.%s:", decl->as.procDecl.name);

  println("\tleave");
  println("\tret");
  
}
void AssemblyNode(Decl* decl){

  switch(decl->kind){
  case DECL_PROC:
    current_proc = &decl->as.procDecl;
    init_args(decl->as.procDecl.args);
    Proc_push(&procs, decl);
    current_decl = decl;
    AssemblerPROC(decl);
    buf__free(local_vars);
    break;
  case DECL_VAR: {
    Var* global_var = &decl->as.varDecl;
    Var_push(&global_vars, global_var);
  } break;
  case DECL_NAMESPACE:
    ns_ctx  = true;
    ns_name = (char*)decl->as.Namespace.name;
    for(size_t i=0; i< buf__len(decl->as.Namespace.decls); ++i){
      Decl* ns = decl->as.Namespace.decls[i];      
      strcat((char*)ns->name, "_");
      strcat((char*)ns->name, ns_name);
      AssemblyNode(ns);
    }
    break;
  case DECL_IMPORT: 
    for(size_t i=0; i < buf__len(decl->as.Import.ast); ++i){
      if(decl->as.Import.ast[i]->kind != DECL_IMPORT)
	println("\t;; file %s", decl->as.Import.path);
      AssemblyNode(decl->as.Import.ast[i]);
    }
    break;
  case DECL_ENUM: {
    EnumFields* efs = decl->as.Enum.fields;
    for(size_t i=0; i< buf__len(efs->fields); ++i){
      EnumField* field = efs->fields[i];
      if(field->expr){
	printf("ERROR: compiler can not handle enumerations with expression.\n");
	exit(1);
      }

      GFL_Macros_push({
	  .name = field->name,
	  .expr = expr_int((int)field->offset)
	});
    }
  } break;
  case DECL_TYPEDEF: break;
  default:
    unreachable();
  }
  DEBUG_OK;
  
}
void AssemblerAST(Decl** ast, const char* output_fp){
  output_file = fopen(output_fp, "wb");
  assert(output_file);
  println("\tglobal _start");
  println("__print:");
  println("\tpush rbp");
  println("\tmov rbp, rsp");
  println("\tsub rsp, 64");
  println("\tmov DWORD [rbp-52], edi");
  println("\tmov DWORD [rbp-4], 1");
  println("\tmov eax, DWORD [rbp-4]");
  println("\tmov edx, 32");
  println("\tsub rdx, rax");
  println("\tmov BYTE [rbp-48+rdx], 10");
  println(".L2:");
  println("\tmov ecx, DWORD [rbp-52]");
  println("\tmov edx, ecx");
  println("\tmov eax, 3435973837");
  println("\timul rax, rdx");
  println("\tshr rax, 32");
  println("\tmov edx, eax");
  println("\tshr edx, 3");
  println("\tmov eax, edx");
  println("\tsal eax, 2");
  println("\tadd eax, edx");
  println("\tadd eax, eax");
  println("\tsub ecx, eax");
  println("\tmov edx, ecx");
  println("\tmov eax, edx");
  println("\tlea ecx, [rax+48]");
  println("\tmov eax, DWORD [rbp-4]");
  println("\tmov edx, 31");
  println("\tsub rdx, rax");
  println("\tmov eax, ecx");
  println("\tmov BYTE [rbp-48+rdx], al");
  println("\tadd DWORD [rbp-4], 1");
  println("\tmov eax, DWORD [rbp-52]");
  println("\tmov edx, eax");
  println("\tmov eax, 3435973837");
  println("\timul rax, rdx");
  println("\tshr rax, 32");
  println("\tshr eax, 3");
  println("\tmov DWORD [rbp-52], eax");
  println("\tcmp DWORD [rbp-52], 0");
  println("\tjne .L2");
  println("\tmov eax, DWORD [rbp-4]");
  println("\tmov edx, 32");
  println("\tsub rdx, rax");
  println("\tlea rax, [rbp-48]");
  println("\tlea rcx, [rax+rdx]");
  println("\tmov eax, DWORD [rbp-4]");
  println("\tmov edx, eax");
  println("\tmov rsi, rcx");
  println("\tmov edi, 1");
  println("\tmov rax, 1");
  println("\tsyscall");
  println("\tnop");
  println("\tleave");
  println("\tret");
  for(size_t i=0; i < buf__len(namespaces); ++i){
    AssemblyNode(namespaces[i]);
  }

  for(size_t j=0; j < buf__len(ast); ++j){
    AssemblyNode(ast[j]);
    
  }


  println("_start:");
  for(size_t i=0; i < buf__len(global_vars); ++i){
    Var* global_var = global_vars[i];
    if(global_var->expr){
      gen_expr(global_var->expr);
      println("\tmov rdi, %s", global_var->type_field->name);
      println("\tmov [rdi], rax");
    }
  }
  println("\tcall main");
  println("\tmov rdi, rax");
  println("\tmov rax, 60");
  println("\tsyscall");
  if(buf__len(global_vars) > 0){
    println("\tsegment .bss");
    for(size_t i=0; i < buf__len(global_vars); ++i){
      Var* global_var = global_vars[i];
      println("\t%s: resb %zu",
	      global_var->type_field->name,
	      global_var->type_field->type->size);
    }
  }
  if(buf__len(strings) > 0){
    println("\tsegment .data");
    for(size_t i=0; i < buf__len(strings); ++i){
      const char* str = strings[i];
      fprintf(output_file, "\tDATA%zu: db ", i);
      for(size_t i=0; i < buf__len(str); ++i){
	fprintf(output_file, "%i, ", (int)str[i]);
      }
      println("");
    }
  }
  fclose(output_file);  
}

#endif /* __assembler */

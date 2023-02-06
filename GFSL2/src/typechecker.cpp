#ifndef __typechecler
#define __typechecler
#include "./gfsl.cpp"


typedef struct TypeList TypeStack;


void dump_type_stack(TypeStack* ts, const char* str){
  printf("%s  [", str);
  for(size_t i=0; i < ts->stack_size; ++i){
    printf("<%s>, ", Type_cstr(ts->stack[i]));
  }
  printf("]\n");
}
void TypeStack_push(TypeStack* ts, GType type){
  assert(ts->stack_size < GFSL_TYPESTACK_CAP);
  ts->stack[ts->stack_size++] = type;
}

bool expect_stack(TypeStack ts, TypeList tl){
  if(ts.stack_size != tl.stack_size)
    return false;
  
  for(size_t i=0; i < ts.stack_size; ++i){
    if(ts.stack[i] != tl.stack[i]) return false;
  }
  return true;
  
}
inline GType TypeStack_back(TypeStack* ts){
  return ts->stack[ts->stack_size - 1];
}
GType TypeStack_offset(TypeStack* ts, size_t i){
  assert(i < ts->stack_size);
  return ts->stack[i];
}
GType TypeStack_pop(TypeStack* ts){
  GType res = ts->stack[ts->stack_size - 1];
  ts->stack_size--;
  return res;
}
GType* TypeStack_reverse(TypeStack ts){
  GType* tmp = NULL;
  for(size_t i=0; i < ts.stack_size; ++i){
    buf__push(tmp, TypeStack_pop(&ts));
  }
  return tmp;
}
inline bool expect_arity(TypeStack* stack, size_t i){
  if(stack->stack_size < i){
    return false;
  }
  return true;
}
bool TypeStack_pop_count(TypeStack* ts, size_t pop_count){
  if(!expect_arity(ts, pop_count)) return false;
  for(size_t i=0; i < pop_count; ++i){
    TypeStack_pop(ts);
  }
  return true;
}
inline bool expect_type(TypeStack* tstack, GType type){
  if(tstack->stack[tstack->stack_size - 1] != type){
    return false;
  }
  return true;
}
#define error(...)				\
  printf("ERROR: ");				\
  printf(__VA_ARGS__);				\
  exit(1);
#define type_error(...)				\
  printf("TypeError: ");			\
  printf(__VA_ARGS__);				\
  exit(1);
void typecheck_inst(TypeStack* ts, Inst inst){
#define CASE(knd, ...) case knd: {__VA_ARGS__} break;
#define CASE_TODO(x) case x: printf("[TODO]: typecheck %s.\n", #x); break;
  switch(inst.type){
    CASE(PUSH_INT, {
	TypeStack_push(ts, GTYPE_INT);});
    CASE(DUMP, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'dump' require one element in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	if(a != GTYPE_INT){
	  fprintf(stderr,
		  "ERROR: expected 'int' for dump instrisic.\n");
	  exit(1);
	}
      });
    CASE(DROP, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'drop' require one element in the stack.\n");
	}
	TypeStack_pop(ts);
      });
    CASE(PUSH_STR, {
	TypeStack_push(ts, GTYPE_INT);
	TypeStack_push(ts, GTYPE_PTR);
      });
    CASE(MEM, {
	TypeStack_push(ts, GTYPE_PTR);
      });
    CASE(PUSH_PTR, {
	TypeStack_push(ts, GTYPE_PTR);
      });
    CASE(PUSH_STATIC_PTR, {
	TypeStack_push(ts, GTYPE_PTR);
      });
    CASE(PUSH_LOCAL_PTR, {
      	TypeStack_push(ts, GTYPE_PTR);
      });
    CASE(PLUS, {
	if(!expect_arity(ts, 2)){
	  error("Intrisic [PLUS, '+'], requires two elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	GType b = TypeStack_pop(ts);
	if(a == GTYPE_PTR and b == GTYPE_PTR){
	  fprintf(stderr,
		  "Intrisic [PLUS, '+'] works with +(ptr, data) but not with +(ptr, ptr).\n");
	  exit(1);
	}
	else if(a == GTYPE_PTR or b == GTYPE_PTR){
	  TypeStack_push(ts, GTYPE_PTR);
	}
	else {
	  TypeStack_push(ts, GTYPE_INT);
	}
      });
    CASE(MINUS, {
	if(!expect_arity(ts, 2)){
	  error("Intrisic [MINUS, '-'], requires two elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	GType b = TypeStack_pop(ts);
	if(a == GTYPE_PTR and b == GTYPE_PTR){
	  TypeStack_push(ts, GTYPE_PTR);
	}
	else if(a == GTYPE_PTR or b == GTYPE_PTR){
	  TypeStack_push(ts, GTYPE_PTR);
	}
	else {
	  TypeStack_push(ts, GTYPE_INT);
	}
      });
    CASE(DUP, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'dup', equires one elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	TypeStack_push(ts, a);
	TypeStack_push(ts, a);	  
      });
    CASE(CALL_PROC, {
	PROC proc = procs.procs[inst.operand];
	if(!expect_arity(ts, proc.args.stack_size)){
	  fprintf(stderr,
		  "ERROR: expected %zu arguments in the stack, but got %zu\n",
		  proc.args.stack_size,
		  ts->stack_size);
	  exit(1);
	}
	// TODO: type check this
	for(size_t i=0; i < proc.args.stack_size; i++){
	  TypeStack_pop(ts);
	}
	for(size_t i=0; i < proc.ret.stack_size; i++){
	  printf("extend in call-proc  %s\n", Type_cstr(proc.ret.stack[i]));
	  TypeStack_push(ts, proc.ret.stack[i]);
	}
      });
    CASE(DUMP_STACK, {
	dump_type_stack(ts, "Dump stack: \t");
      });
    CASE_TODO(PROC_SKIP);
    CASE_TODO(PROC_ENTRY);
    CASE_TODO(PROC_RETURN);
    CASE_TODO(PROC_LEAVE);

    CASE_TODO(DIVMOD);
    CASE_TODO(IDIVMOD);
    CASE(EQUALS, {
	if(!expect_arity(ts, 2)){
	  error("Intrisic '==', requires two elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_BOOL);	
      });
    CASE_TODO(NOT_EQUALS);
    CASE(MULT,{
	if(!expect_arity(ts, 2)){
	  error("Intrisic [MULT, '*'], requires two elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	GType b = TypeStack_pop(ts);
	if(a == GTYPE_PTR and b == GTYPE_PTR){
	  error("Intrisic '*' works with ptr*data or data*data\n");
	}
	else if (a == GTYPE_PTR or b == GTYPE_PTR){
	  TypeStack_push(ts, GTYPE_PTR);
	}
	else {
	  TypeStack_push(ts, GTYPE_INT);
	}
      });
    CASE_TODO(GTHAN);
    CASE_TODO(LTHAN);    
    CASE(ARGC, {TypeStack_push(ts, GTYPE_PTR);});
    CASE(ARGV, {TypeStack_push(ts, GTYPE_PTR);});

    CASE_TODO(OVER); 
    CASE(SWAP, {
	if(!expect_arity(ts, 2)){
	  error("Intrisic 'swap', requires two elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	GType b = TypeStack_pop(ts);
	TypeStack_push(ts, a);
	TypeStack_push(ts, b);	
      }); 
    CASE_TODO(ROT);  
    CASE_TODO(SHL);
    CASE_TODO(SHR);
    CASE_TODO(OR);
    CASE_TODO(AND);
    CASE(NOT, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'not', requires one elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	if(a != GTYPE_BOOL){
	  error("keyword NOT expects boolean on the top of the stack.\n");
	}
	TypeStack_push(ts, GTYPE_BOOL);
      });
    CASE_TODO(CAST);
    CASE_TODO(IF);
    CASE_TODO(ELSE);
    CASE_TODO(WHILE);
    CASE(DO, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'do' requires 1 elements in the stack.\n");
	}
	GType a = TypeStack_pop(ts);
	if(a != GTYPE_BOOL){
	  fprintf(stderr,
		  "ERROR: 'do' blocks expect %s on the top of the stack, but got '%s'\n",
		  Type_cstr(GTYPE_BOOL),
		  Type_cstr(a));
	  exit(1);
	}
      });
    CASE_TODO(END);
    CASE(SYSCALL0, {
	if(!expect_arity(ts, 1)){
	  error("Intrisic 'syscall0' requires 1 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_INT);	  
      }); 
    CASE(SYSCALL1, {
	if(!expect_arity(ts, 2)){
	  error("Intrisic 'syscall1' requires 2 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(SYSCALL2, {
	if(!expect_arity(ts, 3)){
	  error("Intrisic 'syscall2' requires 3 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(SYSCALL3, {
	if(!expect_arity(ts, 4)){
	  error("Intrisic 'syscall3' requires 4 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(SYSCALL4, {
	if(!expect_arity(ts, 5)){
	  error("Intrisic 'syscall4' requires 5 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);	  
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(SYSCALL5, {
	if(!expect_arity(ts, 6)){
	  error("Intrisic 'syscall5' requires 6 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(SYSCALL6, {
	if(!expect_arity(ts, 7)){
	  error("Intrisic 'syscall6' requires 7 elements in the stack.\n");
	}
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	
	TypeStack_pop(ts);
	TypeStack_pop(ts);
	TypeStack_pop(ts);

	TypeStack_pop(ts);
	
	TypeStack_push(ts, GTYPE_INT);	  
      });
    CASE(LOAD8, {
	if(!expect_arity(ts, 1)){
	  printf("todo document this\n");
	  exit(1);
	}
	GType ptr = TypeStack_pop(ts);
	if(ptr != GTYPE_PTR){
	  error("Intrisic load8 expect '%s', but got '%s'\n",
		Type_cstr(GTYPE_PTR),
		Type_cstr(ptr));
	  exit(1);
	}
	TypeStack_push(ts, GTYPE_INT);
      });
    CASE(LOAD64,{
	if(!expect_arity(ts, 1)){
	  printf("todo document this\n");
	  exit(1);
	}
	GType ptr = TypeStack_pop(ts);
	if(ptr != GTYPE_PTR){
	  error("Intrisic load expect '%s', but got '%s'\n",
		Type_cstr(GTYPE_PTR),
		Type_cstr(ptr));
	  exit(1);
	}
	TypeStack_push(ts, GTYPE_INT);
      });  
    CASE_TODO(STORE8);
    CASE(STORE64, {
	if(!expect_arity(ts, 2)){
	  printf("todo document this\n");
	  exit(1);
	}
	GType Data = TypeStack_pop(ts);
	GType Dst  = TypeStack_pop(ts);
	if(Dst != GTYPE_PTR){
	  error("Intrisic load expects a pointer to be the second element on the stack, got '%s'.\n",
	        Type_cstr(Dst));
	}
	(void) Data;
	(void) Dst;
      });
    CASE(EXTEND_MACRO, {
	// TODO: get rid of this section
	MACRO macro = macros.macros[inst.operand];
	for(size_t i=0; i < macro.body_size; ++i){
	  Inst macro_inst = macro.body[i];
	  typecheck_inst(ts, macro_inst);
	}
      });
    CASE(CAST_INT, {
	if(!expect_arity(ts, 1)){
	  fprintf(stderr,
		  "ERROR: expected at least one element on the stack for casting to '%s'.\n",
		  Type_cstr(GTYPE_INT));
	  exit(1);
	}
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_INT);	
      });
    CASE(CAST_PTR, {
	if(!expect_arity(ts, 1)){
	  fprintf(stderr,
		  "ERROR: expected at least one element on the stack for casting to '%s'.\n",
		  Type_cstr(GTYPE_PTR));
	  exit(1);
	}
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_PTR);

      })
     CASE(CAST_BOOL, {
	if(!expect_arity(ts, 1)){
	  fprintf(stderr,
		  "ERROR: expected at least one element on the stack for casting to '%s'.\n",
		  Type_cstr(GTYPE_BOOL));
	  exit(1);
	}
	TypeStack_pop(ts);
	TypeStack_push(ts, GTYPE_BOOL);

      })
      
  case NUM_OF_OPERANDS:
  default:
    printf("unreachable, typecheck_procedure\n");
    exit(1);
  }
#undef CASE
#undef CASE_TODO
}
void typecheck_procedure(PROC* proc){
  TypeStack ts = {};
  
  for(size_t i=0; i < proc->args.stack_size; ++i){
    TypeStack_push(&ts, proc->args.stack[i]);
  }
  
  
  for(size_t i=0; i < proc->body_size; ++i){
    Inst inst = proc->body[i];
    typecheck_inst(&ts, inst);
  }
  
  if(ts.stack_size != proc->ret.stack_size){    
    dump_type_stack(&ts,        "got stack\t");
    dump_type_stack(&proc->ret, "expected stack\t");
    error("expected %zu elements on the stack, but got %zu on the procedure '%s'\n",
	  proc->ret.stack_size,
	  ts.stack_size,
	  proc->name);
    exit(1);
  }
  
  for(size_t i=0; i < proc->ret.stack_size; i++){

    GType expected   = proc->ret.stack[proc->ret.stack_size - i - 1];
    GType got        = TypeStack_pop(&ts);
    if(expected != got){
      
    error("expected '%s' to be the %zu element on the top of the stack, but got '%s', on procedure '%s'\n",
	   Type_cstr(expected),
	  i,
	  Type_cstr(got),
	  proc->name);
    exit(1);
    }
    //  dump_type_stack(&ts,        "got stack\t");
    //  dump_type_stack(&proc->ret, "expected stack\t");
    //  error("expected diferent data types in the stack on procedure '%s'\n",
    //	    proc->name);
    //  exit(1);    
    //}
  }
}
void typecheck_program(gfsl_vr* vr){
  // TODO: get rid of vr
  (void) vr;
  for(size_t i=0; i < procs.procs_size; ++i){
    typecheck_procedure(&procs.procs[i]);
  }
 
}
#endif /* __typechecler */

#ifndef __gfsl
#define  __gfsl
#define GFSL_PROGRAM_CAPACITY 1024
#define GFSL_STACK_CAPACITY   1024
#include <iostream>
#include <cassert>
#include <cstring>
#include "./lexer.cpp"
typedef struct gfsl_vr gfsl_vr;
enum Op_type{
  DUMP_STACK,
  CALL_PROC,
  PUSH_LOCAL_MEM,
  ALLOC_LOCAL_MEM,
  PROC_SKIP,
  PROC_ENTRY,
  PROC_RETURN,
  PROC_LEAVE,
  PUSH_INT,
  PUSH_STR,
  PLUS,
  MINUS,
  DIVMOD,
  IDIVMOD,
  EQUALS,
  NOT_EQUALS,
  MULT,
  GTHAN,
  LTHAN,    
  MEM,
  ARGC,
  ARGV,
  DUMP,
  DUP,
  DROP, 
  OVER, // a b -> a b a 
  SWAP, // a b -> b a
  ROT,  // a b c -> c b a
  PUSH_PTR,
  //  -----
  SHL,
  SHR,
  OR,
  AND,
  NOT,
  CAST,
  // ----
  IF,
  ELSE,
  WHILE,
  DO,
  END,

  SYSCALL,

  LOAD8,
  LOAD64,  
  STORE8,
  STORE64,
  
  NUM_OF_OPERANDS        
};
struct Inst{
  Op_type  type;
  uint64_t operand;
};
struct gfsl_vr{
  Inst    program[GFSL_PROGRAM_CAPACITY];
  size_t  program_size;
  size_t  ip;
  
  int64_t stack[GFSL_STACK_CAPACITY];
  size_t  stack_size;

  gfsl_vr() = default;
};
void gfsl_vr_push_inst(gfsl_vr* vr, Inst inst){
  assert(vr->program_size + 1 <=GFSL_PROGRAM_CAPACITY);

  vr->program[vr->program_size++] = inst;
}
#define GFSL_PUSH_INST(_type, _operand)					\
  gfsl_vr_push_inst(vr, {.type=(_type), .operand=((uint64_t)_operand)}); 							


inline int64_t GFSL_STACK_POP(gfsl_vr* vr){
  if(vr->stack_size < 1){
    printf("ERROR: stack underflow.\n");
    exit(1);
  }
  int64_t result = vr->stack[vr->stack_size - 1];
  vr->stack_size--;
  return result;
}

void gfsl_from_text(gfsl_vr* vr, const char* text){
  init_stream(text);

   while(*stream){
    if(expect_name("dump")){
      printf("dump.\n");
      GFSL_PUSH_INST(DUMP, 0);
    }
    else if (expect_name("dup")){
      GFSL_PUSH_INST(DUP, 0);
    }
    else if (expect_name("swap")){
      GFSL_PUSH_INST(SWAP, 0);
    }
    else if (expect_name("over")){
      GFSL_PUSH_INST(OVER, 0);
    }
    else if (expect_name("rot")){
      GFSL_PUSH_INST(ROT, 0);
    }
    else if (expect_name("drop")){
      GFSL_PUSH_INST(DROP, 0);
    }
    else if (expect_name("divmod")){
      GFSL_PUSH_INST(DIVMOD, 0);
    }
    else if (expect_name(">>")){
      GFSL_PUSH_INST(SHL, 0);
    }
    else if (expect_name("<<")){
      GFSL_PUSH_INST(SHR, 0);
    }
    else if (expect_name("and")){
      GFSL_PUSH_INST(AND, 0);
    }
    else if (expect_name("or")){
      GFSL_PUSH_INST(OR, 0);
    }
    else if (expect_name("not")){
      GFSL_PUSH_INST(NOT, 0);
    }
    else if (expect_name("argv")){
      GFSL_PUSH_INST(ARGV, 0);
    }
    else if (expect_name("argc")){
      GFSL_PUSH_INST(ARGC, 0);
    }
    else if (expect_name("store8")){
      GFSL_PUSH_INST(STORE8, 0);
    }
    else if (expect_name("store")){
      GFSL_PUSH_INST(STORE64, 0);
    }
    else if (expect_name("load8")){
      GFSL_PUSH_INST(LOAD8, 0);
    }
    else if (expect_name("load")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("mem")){
      GFSL_PUSH_INST(PUSH_PTR, 0);
    }
    else if (expect_name("if")){
      printf("ERROR: op not implemented.\n");
      exit(1);
      
      GFSL_PUSH_INST(IF, 0);
    }
    else if (expect_name("while")){
      printf("ERROR: op not implemented.\n");
      exit(1);
      
      GFSL_PUSH_INST(WHILE, 0);
    }
    else if (expect_name("elif")){
      printf("ERROR: op not implemented.\n");
      exit(1);
      
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("else")){
      printf("ERROR: op not implemented.\n");
      exit(1);
      
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("do")){
      printf("ERROR: op not implemented.\n");
      exit(1);
      
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("cast(int)")){
      printf("ERROR: cast are not implemented yet.\n");
      exit(1);
    }
    else if (expect_name("cast(ptr)")){
      printf("ERROR: cast are not implemented yet.\n");
      exit(1);
    }
    else if (expect_name("cast(bool)")){
      printf("ERROR: cast are not implemented yet.\n");
      exit(1);
    }         	         
    
    else if (expect_name("syscall0")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall1")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall2")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall3")){    
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall4")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall5")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    else if (expect_name("syscall6")){
      GFSL_PUSH_INST(LOAD64, 0);
    }
    
    
    else if (expect_name("+")){
      GFSL_PUSH_INST(PLUS, 0);
    }
    else if (expect_name("-")){
      GFSL_PUSH_INST(PLUS, 0);
    }
    else if (expect_name("*")){
      GFSL_PUSH_INST(MULT, 0);
    }
    
    else if (expect_token(TOKEN_INT)){
      printf("push int\n");
      GFSL_PUSH_INST(PUSH_INT, token.INT);
    }

    else {
      printf("ERROR: undefined token: %s\n", token.name);
      exit(1);
    }
  } 
}
void gfsl_from_file(gfsl_vr* vr, const char* fp){
  const char* ftext = get_file_text(fp);
  assert(ftext);
  gfsl_from_text(vr, ftext);
}
void gfsl_simulate_program(gfsl_vr* vr){
  for(;vr->ip < vr->program_size;){
    Inst inst = vr->program[vr->ip];
    switch(inst.type){
    case PUSH_INT:{
      vr->stack[vr->stack_size++] = inst.operand;
      vr->ip++;
    } break;
      
    case PLUS:{
      int64_t a = vr->stack[vr->stack_size - 2];
      int64_t b = vr->stack[vr->stack_size - 1];
      vr->stack_size -= 2;
      
      vr->stack[vr->stack_size++] = a + b;
      vr->ip++;
    } break;
    case DUMP:{
      int64_t value = vr->stack[vr->stack_size - 1];
      printf("%ld\n", value);
      vr->stack_size--;
      vr->ip++;
    } break;
    default:
      printf("ERROR: unhandled vr->type in simulate program.\n");
      exit(1);
    }
  }
}
// TODO: void gfsl_compile_program(gfsl_vr* vr){
// TODO:   for(size_t i=0; i < vr->program_size; ++i){
// TODO:     Inst inst = vr->program[i];
// TODO:     switch(inst.type){
// TODO:     }
// TODO:   }
// TODO: }
#endif /*  __gfsl */

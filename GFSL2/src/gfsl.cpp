#ifndef __gfsl
#define  __gfsl
#define GFSL_PROGRAM_CAPACITY		    1024
#define GFSL_STACK_CAPACITY		    1024
#define GFSL_PROC_CAPACITY		    1024
#define GFSL_STATIC_CAPACITY		    1024
#define GFSL_LOCAL_MEMORY_CAPACITY	    1024
#define GFSL_IMPORTS_CAPACITY               1024
#define GFSL_COMPILED_RETURN_STACK_CAPACITY 4096
#define GFSL_TYPESTACK_CAP 1024

#include <iostream>
#include <cassert>
#include <cstring>
#include "./lexer.cpp"
size_t instruction_number = 0;
#define advance_label() instruction_number++;
#define make_label(inst)			\
  fprintf(stream, ".L%zu:\t;; [INFO]: %s\n",	\
	  instruction_number++,			\
	  human_inst(inst));
#define make_ret_label(proc_name)				\
  fprintf(stream, ".L%zu:\t;; [INFO]: return from %s\n",	\
	  instruction_number,					\
	  proc_name);

typedef struct gfsl_vr gfsl_vr;
void gfsl_from_file(gfsl_vr* vr, const char* fp);

enum Op_type{
  DUMP_STACK,
  CALL_PROC,

  PROC_SKIP,
  PROC_ENTRY,
  PROC_RETURN,
  PROC_LEAVE,
  PUSH_INT,
  CAST_INT,
  CAST_PTR,
  CAST_BOOL,
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
  PUSH_STATIC_PTR,
  PUSH_LOCAL_PTR,
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
  SYSCALL0,
  SYSCALL1,
  SYSCALL2,
  SYSCALL3,
  SYSCALL4,
  SYSCALL5,
  SYSCALL6,
  
  LOAD8,
  LOAD64,  
  STORE8,
  STORE64,
  EXTEND_MACRO,
  NUM_OF_OPERANDS        
};
const char* human_op_type(Op_type type){
  switch(type){
#define CASE(k) case k: return (#k);
    CASE(DUMP_STACK);
    CASE(CALL_PROC);
    CASE(PROC_SKIP);
    CASE(PROC_ENTRY);
    CASE(PROC_RETURN);
    CASE(PROC_LEAVE);
    CASE(PUSH_INT);
    CASE(PUSH_STR);
    CASE(PLUS);
    CASE(MINUS);
    CASE(DIVMOD);
    CASE(IDIVMOD);
    CASE(EQUALS);
    CASE(NOT_EQUALS);
    CASE(MULT);
    CASE(GTHAN);
    CASE(LTHAN);    
    CASE(MEM);
    CASE(ARGC);
    CASE(ARGV);
    CASE(DUMP);
    CASE(DUP);
    CASE(DROP); 
    CASE(OVER);
    CASE(SWAP);
    CASE(ROT); 
    CASE(PUSH_PTR);
    CASE(PUSH_STATIC_PTR);
    CASE(PUSH_LOCAL_PTR);
    CASE(CAST_INT);
    CASE(CAST_PTR);
    CASE(CAST_BOOL);
    
    CASE(SHL);
    CASE(SHR);
    CASE(OR);
    CASE(AND);
    CASE(NOT);
    CASE(CAST);
    CASE(IF);
    CASE(ELSE);
    CASE(WHILE);
    CASE(DO);
    CASE(END);
    CASE(SYSCALL0);
    CASE(SYSCALL1);
    CASE(SYSCALL2);
    CASE(SYSCALL3);
    CASE(SYSCALL4);
    CASE(SYSCALL5);
    CASE(SYSCALL6);  
    CASE(LOAD8);
    CASE(LOAD64);  
    CASE(STORE8);
    CASE(STORE64);
    CASE(EXTEND_MACRO);
  case NUM_OF_OPERANDS:
  default:
    fprintf(stderr,
	    "ERROR: human OpType unreachable kind.\n");
    exit(1);
#undef CASE
  }
}
inline bool is_op_type_inline_context_forbiden(Op_type type){
  // TODO: add support to some of those OPS
#define OR(k) (type == (k))
  return  OR(Op_type::PROC_RETURN)
    ||	  OR(Op_type::WHILE)
    ||	  OR(Op_type::IF)    
    ;  
#undef OR
}
struct Inst{
  Op_type  type;
  size_t operand;
};
struct IMPORT{
  const char* fp[GFSL_IMPORTS_CAPACITY];
  size_t      fp_size;
};
IMPORT imports = {};
bool IMPORT_find(const char* name){
  for(size_t i=0; i < imports.fp_size; ++i){
    if(STR_CMP(imports.fp[i], name)) return true;
  }
  return false;
}
void IMPORT_push(const char* name){
  assert(imports.fp_size < GFSL_IMPORTS_CAPACITY);
  imports.fp[imports.fp_size++] = name;
}
Inst* inst_from_text();
const char* human_inst(Inst inst){
  // TODO: change static var to ptr
  static char static_buffer[258];
  sprintf(static_buffer,
	  "(INST)[type = %s, operand = %li]",
	  human_op_type(inst.type),
	  inst.operand);
  return static_buffer;
}
struct gfsl_vr{
  Inst    program[GFSL_PROGRAM_CAPACITY];
  size_t  program_size;
 
  gfsl_vr() = default;
};
struct MACRO{
  const char* name;
  Inst   body[GFSL_PROGRAM_CAPACITY];
  size_t body_size;
};
struct MACROS{
  MACRO  macros[GFSL_PROGRAM_CAPACITY];
  size_t macros_size = 0;
  MACROS() = default;
};
MACROS macros = {};
int MACROS_pos(const char* name){
  for(size_t i=0; i < macros.macros_size; ++i){
    if(STR_CMP(macros.macros[i].name, name)){
      return (int)i;
    }
  }
  return -1;
}
MACRO* MACROS_find(const char* name){
  size_t pos = MACROS_pos(name);
  if(pos>0)
    return &macros.macros[pos];
  return NULL;
}
inline bool MACROS_exists(const char* name){
  return (MACROS_pos(name) != -1);
}
void MACROS_push(MACRO m){
  assert(macros.macros_size < GFSL_PROGRAM_CAPACITY);
  macros.macros[macros.macros_size++] = m;
}
//
struct LOCAL{
  const char* name;
  size_t size   = 0 ;
  size_t offset = 0;
};
struct LOCALS{
  LOCAL  locals[GFSL_LOCAL_MEMORY_CAPACITY];
  size_t total_offset = 0;
  size_t locals_size  = 0;
};
enum Type{TYPE_UNDEFINED, TYPE_INT, TYPE_PTR, TYPE_BOOL, TYPE_VOID};
Type Type_from_text(const char* str){
  printf("Type_from_text : %s\n", str);
  if(STR_CMP(str, "int"))
    return TYPE_INT;
  if(STR_CMP(str, "ptr"))
    return TYPE_PTR;
  if(STR_CMP(str, "void"))
    return TYPE_VOID;
  if(STR_CMP(str, "bool"))
    return TYPE_BOOL;
  
  printf("ERROR: undefined type '%s'\n", str);
  exit(1); 
}
const char* Type_cstr(Type type){
  switch(type){
  case TYPE_INT:  return "int";
  case TYPE_PTR:  return "ptr";
  case TYPE_VOID: return "void";
  case TYPE_BOOL: return "boolean";
  case TYPE_UNDEFINED: break;
  }
  printf("ERROR: unreachabel at Type_cstr");
  exit(1); 
  
}
struct TypeList{
  Type   stack[GFSL_TYPESTACK_CAP];
  size_t stack_size;
};
void TypeList_push(TypeList* tl, Type t){
  assert(tl->stack_size < GFSL_TYPESTACK_CAP);
  tl->stack[tl->stack_size++] = t;
}

struct PROC{
  const char* name;
  bool        is_inline;
  TypeList    args; 
  TypeList    ret;  
  Inst        body[GFSL_PROGRAM_CAPACITY];
  size_t      body_size;
  LOCALS      locals;
  bool        have_local;
};
PROC* current_proc = NULL;
void PROC_push_local(LOCAL local){
  if(current_proc->is_inline){
    fprintf(stderr,
	    "ERROR: inline procs does not accept any local memory.\n");
    exit(1);
  }
  local.offset = current_proc->locals.total_offset;
  current_proc->locals.locals[current_proc->locals.locals_size++] = local;
  current_proc->locals.total_offset += local.size + 1;
  current_proc->have_local = true;


}
size_t PROC_get_local_offset(const char* name){
  for(size_t i=0; i < current_proc->locals.locals_size; ++i){
    if(STR_CMP(current_proc->locals.locals[i].name, name)){
      return current_proc->locals.locals[i].offset;
    }
  }
  abort();
}
inline bool PROC_is_local(const char* name){
  if(current_proc){
    for(size_t i=0; i < current_proc->locals.locals_size; ++i){
      if(STR_CMP(current_proc->locals.locals[i].name, name)){
	return true;
      }
    }
  }
  return false;
}
struct PROCS{
  PROC   procs[GFSL_PROC_CAPACITY];
  size_t procs_size;
};

PROCS procs = {};

int PROCS_pos(const char* name){
  for(size_t i=0; i < procs.procs_size; ++i){
    if(STR_CMP(procs.procs[i].name, name)){
      return i;
    }
  }
  return -1;
}
inline bool PROCS_exists(const char* name){
  return (PROCS_pos(name) != -1);
}
PROC* PROCS_find(const char* name){
  int pos = PROCS_pos(name);
  if (pos != -1 )
    return &procs.procs[pos];
  return NULL;
}
void PROCS_push(PROC p){
  if(p.is_inline and STR_CMP(p.name, "main")){
    fprintf(stderr,
	    "ERROR: the 'main' procedure can not be inlined.\n");
    exit(1);
  }
  if(PROCS_pos(p.name) != -1){
    fprintf(stderr,
	    "ERROR: redefinition of the procedure '%s'.\n", p.name);
    exit(1);
  }
  assert(procs.procs_size < GFSL_PROC_CAPACITY);
  procs.procs[procs.procs_size++] = p;
}

//
struct STATIC{
  const char* name;
  size_t size;
};
struct STATICS{
  STATIC statics[GFSL_STATIC_CAPACITY];
  size_t statics_size;
};
STATICS static_memory = {};
int STATICS_pos(const char* name){
  for(size_t i=0; i<static_memory.statics_size; ++i){
    if(STR_CMP(static_memory.statics[i].name, name)){
      return (int)i;
    }
  }
  return -1;
}
void STATICS_push(STATIC _static){
  assert(static_memory.statics_size < GFSL_STATIC_CAPACITY);
  static_memory.statics[static_memory.statics_size++] = _static;
}
inline bool STATICS_exists(const char* name){
  return (STATICS_pos(name) != -1);
}
STATIC* STATICS_find(const char* name){
  int pos = STATICS_pos(name);
  if(pos != -1){
    return &static_memory.statics[pos];
  }
  return NULL;
}
//
void gfsl_vr_push_inst(gfsl_vr* vr, Inst inst){
  assert(vr->program_size + 1 <=GFSL_PROGRAM_CAPACITY); 
  vr->program[vr->program_size++] = inst;
}
#define GFSL_PUSH_INST(_type, _operand)					\
  gfsl_vr_push_inst(vr, {.type=(_type), .operand=((size_t)_operand)});

#define GLOBAL_DATA_STR_CAP 1024
struct DATA{
  const char* str[GLOBAL_DATA_STR_CAP];
  size_t      str_size;
};
DATA global_data = {};
size_t reserve_string_in_data(const char* str){
  assert(global_data.str_size < GLOBAL_DATA_STR_CAP);
  global_data.str[global_data.str_size++] = str;
  return (size_t)(global_data.str_size - 1);
}
void extend_macro(){
  /*
    syntax:
    macro <name> <body> end
  */
  MustExpectName("macro");
  MACRO macro = {};
  macro.body_size = 0;

  macro.name = token.name;
  next_token();
  while(!token_is_name("end") and *stream){
    {
      if (macro.body_size >= GFSL_PROGRAM_CAPACITY){
	fprintf(stderr,
		"ERROR: macro is grather than the program capacity.\n");
	exit(1);
      }
    }
    Inst* child = inst_from_text();
    if(is_op_type_inline_context_forbiden(child->type)){
      fprintf(stderr,
	      "ERROR: OP '%s' is forbiden in macro definition.\n",
	      human_op_type(child->type));
      exit(1);
    }
    macro.body[macro.body_size++] = *child;
    next_token();
  }

  // is EOF?
  if(!*stream){
    fprintf(stderr,
	    "ERROR: reach end of the file before end of macro definition.\n");
    exit(1);
  }
  // skip END keyword
  MustExpectName("end");
  MACROS_push(macro);
}

void extend_memory_allocation(){
  STATIC _static = {};
  MustExpectName("static");
  _static.name = token.name;
  next_token();
  assert(token.kind == TOKEN_INT);
  _static.size = token.INT;
  next_token();
  MustExpectName("end");
  STATICS_push(_static);
}
void extend_local_allocation(){
  if(!current_proc){
    fprintf(stderr,
	    "ERROR: 'local' keyword only works inside procedures.\n");
    exit(1);
  }
  LOCAL local  = {};  
  MustExpectName("local");
  local.name   = token.name;
  next_token();
  local.size   = token.INT;  
  next_token();
  MustExpectName("end");
  PROC_push_local(local);
}

Inst* inst_from_text(){

  Inst* inst = new Inst;
  inst->operand = 0;
  if(token_is_name("dump")){
    inst->type = DUMP;
  }
  else if (is_token(TOKEN_PLUS)){
    inst->type = PLUS;
  }
  else if (is_token(TOKEN_TAKEAWAY)){
    inst->type = MINUS;
  }
  else if (is_token(TOKEN_STAR)){
    inst->type = MULT;
  }
  else if (is_token(TOKEN_LESS)){
    inst->type = LTHAN;
  }
  else if (token_is_name("dup")){
    inst->type = DUP;
  }
  else if (token_is_name("swap")){
    inst->type = SWAP;
  }
  else if (token_is_name("over")){
    inst->type = OVER;
  }
  else if (token_is_name("rot")){
    inst->type = ROT;
  }
  else if (token_is_name("drop")){
    inst->type = DROP;
  }
  else if (token_is_name("divmod")){
    inst->type = DIVMOD;
  }
  else if (token_is_name(">>")){
    inst->type = SHL;
  }
  else if (token_is_name("<<")){
    inst->type = SHR;
  }
  else if (token_is_name("and")){
    inst->type = AND;
  }
  else if (token_is_name("or")){
    inst->type = OR;
  }
  else if (token_is_name("not")){
    inst->type = NOT;
  }
  else if (token_is_name("argv")){
    inst->type = ARGV;
  }
  else if (token_is_name("argc")){
    inst->type = ARGC;
  }
  else if (token_is_name("store8")){
    inst->type = STORE8;
  }
  else if (token_is_name("store")){
    inst->type = STORE64;
  }
  else if (token_is_name("load8")){
    inst->type = LOAD8;
  }
  else if (token_is_name("load")){
    inst->type = LOAD64;
  }
  else if (token_is_name("mem")){
    inst->type = PUSH_PTR;
  }
  else if (token_is_name("if")){
    inst->type = IF;      
  }
  else if (token_is_name("while")){
    inst->type = WHILE;      
  }
  else if (token_is_name("elif")){
    printf("ERROR: op elif not implemented.\n");
    exit(1);            
  }
  else if (token_is_name("else")){
    printf("ERROR: op else not implemented.\n");
    exit(1);            
  }
  else if (token_is_name("do")){
    inst->type = DO;
  }
  else if (token_is_name("end")){
    inst->type = END;
  }
  
  else if (token_is_name("cast(int)")){
    printf("ERROR: cast are not implemented yet.\n");
    exit(1);
  }
  else if (token_is_name("cast(ptr)")){
    printf("ERROR: cast are not implemented yet.\n");
    exit(1);
  }
  else if (token_is_name("cast(bool)")){
    printf("ERROR: cast are not implemented yet.\n");
    exit(1);
  }         	         
  else if (token_is_name("return")){
    inst->type = PROC_RETURN; 
  }    
  else if (token_is_name("syscall0")){
    inst->type = SYSCALL0;
  }
  else if (token_is_name("syscall1")){
    inst->type = SYSCALL1;
  }
  else if (token_is_name("syscall2")){
    inst->type = SYSCALL2;
  }
  else if (token_is_name("syscall3")){    
    inst->type = SYSCALL3;
  }
  else if (token_is_name("syscall4")){
    inst->type = SYSCALL4;
  }
  else if (token_is_name("syscall5")){
    inst->type = SYSCALL5;
  }
  else if (token_is_name("syscall6")){
    inst->type = SYSCALL6;
  }           
  else if (token_is_name("static")){
    extend_memory_allocation();
    return NULL;
  }
  else if (token_is_name("local")){
    extend_local_allocation();
    return NULL;
  }

  // TODO: parse cmp tokens
  else if (is_token(TOKEN_INT)){
    inst->type = PUSH_INT;
    inst->operand = (size_t)token.INT;
  }
  else if (is_token(TOKEN_STRING)){
    size_t string_loc = reserve_string_in_data(token.STRING);
    inst->type = PUSH_STR;
    inst->operand = string_loc;
  }
  else if (MACROS_exists(token.name)){
    size_t macro_loc = MACROS_pos(token.name);
    inst->type = EXTEND_MACRO;
    inst->operand = macro_loc;
  }  
  else if (STATICS_exists(token.name)){
    inst->type    = PUSH_STATIC_PTR;
    inst->operand = STATICS_pos(token.name);
  }
  else if (PROC_is_local(token.name)){
    inst->type    = PUSH_LOCAL_PTR;
    inst->operand = PROC_get_local_offset(token.name);
  }
  else if (PROCS_exists(token.name)){
    inst->type    = CALL_PROC;
    inst->operand = PROCS_pos(token.name);
  }
  else if (STR_CMP(token.name, "???"))       inst->type = DUMP_STACK;
  else if (STR_CMP(token.name, "cast-int"))  inst->type = CAST_INT;  
  else if (STR_CMP(token.name, "cast-ptr"))  inst->type = CAST_PTR;
  else if (STR_CMP(token.name, "cast-bool")) inst->type = CAST_BOOL;
  
  else {
    printf("ERROR: undefined token: %s\n", token.name);    
    exit(1);
    
  }
  return inst;
}
void extend_import(gfsl_vr* vr){
  MustExpectName("import");
  assert(token.kind == TOKEN_STRING);
  const char* file_path = token.STRING;
  //next_token();
  if(IMPORT_find(file_path)) {
    printf("[DEV-NOTE]: %s is already imported.\n", file_path);
  }
  else {
    printf("[DEV-note]: %s will be included.\n", file_path);
    {
      const char* before_fcontent = stream;
      IMPORT_push(file_path);
      gfsl_from_file(vr, file_path);      
      init_stream(before_fcontent);
      
    }
  }
}

void extend_proc(bool inlinable){
  PROC proc = {};
  
  proc.is_inline = inlinable;
  // NOTE: change MustExpectName for respective instructions
  MustExpectName("proc");
  assert(token.kind == TOKEN_NAME);
  proc.name = token.name;
  proc.args.stack_size = 0;
  proc.ret.stack_size  = 0;
  next_token();
  if(!is_token(TOKEN_OPEN_C_PAREN)){
    while(!is_token(TOKEN_DOUBLE_DOT) and !is_token(TOKEN_OPEN_C_PAREN)){
      assert(token.kind == TOKEN_NAME);
      Type arg = Type_from_text(token.name);
      
      assert(proc.args.stack_size < GFSL_TYPESTACK_CAP);      
      proc.args.stack[proc.args.stack_size++] = arg;
      
      next_token();
    }
  }

  if(STR_CMP(proc.name, "main") and proc.args.stack_size > 0){
    printf("ERROR: the 'main' procedure does not accept any argument.\n");
    exit(1);
  }
  if(expect_token(TOKEN_DOUBLE_DOT)){
    while(!is_token(TOKEN_OPEN_C_PAREN)){
      assert(token.kind == TOKEN_NAME);
      Type ret = Type_from_text(token.name);
      
      assert(proc.ret.stack_size < GFSL_TYPESTACK_CAP);
      proc.ret.stack[proc.ret.stack_size++] = ret;
      
      next_token();
    }    
  }

  if(STR_CMP(proc.name, "main") and proc.ret.stack_size > 0){
    printf("ERROR: the 'main' procedure does not return any argument.\n");
    exit(1);
  }

  MustExpect(TOKEN_OPEN_C_PAREN);
  current_proc = &proc;  
  size_t  prev[GFSL_PROGRAM_CAPACITY];
  size_t  prev_size = 0;

  while(*stream  and !is_token(TOKEN_CLOSE_C_PAREN)){
    Inst* inst = inst_from_text();

    if(inst){
      if(inst->type == Op_type::IF or inst->type == Op_type::WHILE){
	prev[prev_size++] = proc.body_size;
      }

      if(inst->type == Op_type::DO){
	prev[prev_size++] = proc.body_size;
      }
      if(inst->type == Op_type::END){
	if(prev_size < 2){
	  fprintf(stderr,
		  "SyntaxError: undefined behavior of 'do' keyword.\n");
	  exit(1);
	}
	size_t pre_end_pos = prev[--prev_size];
	size_t pre_do_pos  = prev[--prev_size];
	Inst* pre_do  = &proc.body[pre_do_pos];
	Inst* pre_end = &proc.body[pre_end_pos];
	if(pre_do->type == Op_type::IF){
	  // if pre_do == IF
	  assert(pre_end->type == Op_type::DO);
	  pre_end->operand = proc.body_size + 1;
	  inst->operand    = proc.body_size + 1;
	  // if 1 do
	  // end	  
	}
	else if(pre_do->type == Op_type::WHILE){
	  assert(pre_end->type == Op_type::DO);
	  // pre_do   == WHILE
	  // pre_end  == DO
	  // inst     == END
	  pre_end->operand = proc.body_size + 2;
	  inst->operand = pre_do_pos;
	}
	else {
	  fprintf(stderr,
		  "ERROR: end can only close 'if' or 'while' blocks.\n");
	  exit(1);
	}
      }
      if(proc.is_inline){
	if(is_op_type_inline_context_forbiden(inst->type)){
	  fprintf(stderr,
		  "ERROR: the Op '%s' is forbiden in inline procedure context.\n",
		  human_op_type(inst->type));
	  exit(1);
	}
      }
      proc.body[proc.body_size++] = *inst;      
      next_token();
    }    
  }
  if(prev_size != 0){
    fprintf(stderr,
	    "ERROR: found an unclosed block.\n"
	    "TODO:  print the localization of the block.\n");
    
    exit(1);
  }
  if(0){
    printf("| CROSS REFERENCE '%s'|\n", proc.name);
    for(size_t i=0; i<proc.body_size; ++i){
      printf("INST[%s]\n", human_inst(proc.body[i]));
    }

  }

  if(!*stream){
    fprintf(stderr,
	    "ERROR: expected end of the procedure but got EOF.\n");

  }
  
  MustExpect(TOKEN_CLOSE_C_PAREN);

  PROCS_push(proc);
  current_proc = NULL;
  
}
void gfsl_from_text(gfsl_vr* vr, const char* text){
  init_stream(text);

  while(*stream){
    bool inlinable = expect_name("inline");
     
    if(token_is_name("macro")){
      if(inlinable){
	fprintf(stderr,
		"ERROR: wrong use of inline, in macro declaration.\n");
	exit(1);
      }
      
      extend_macro();
    }
    else if (token_is_name("import")){
      if(inlinable){
	fprintf(stderr,
		"ERROR: wrong use of inline, in import declaration.\n");
	exit(1);
      }
      
      extend_import(vr);
    }
    else if (token_is_name("proc")){
      extend_proc(inlinable);       
    }
    else {

      Inst* inst = inst_from_text();
      if(inst){
	GFSL_PUSH_INST(inst->type, inst->operand);	 
	next_token();
      }
    }
  } 
}
void gfsl_from_file(gfsl_vr* vr, const char* fp){
  const char* ftext = get_file_text(fp);
  assert(ftext);
  gfsl_from_text(vr, ftext);
}

void generate_header(FILE* stream){
  fprintf(stream, "BITS 64\n"); 
  fprintf(stream, "dump:\n");
  fprintf(stream, "\tpush  rbp\n");
  fprintf(stream, "\tmov   rbp, rsp\n");
  fprintf(stream, "\tsub   rsp, 64\n");
  fprintf(stream, "\tmov   QWORD  [rbp-56], rdi\n");
  fprintf(stream, "\tmov   QWORD  [rbp-8], 1\n");
  fprintf(stream, "\tmov   eax, 32\n");
  fprintf(stream, "\tsub   rax, QWORD  [rbp-8]\n");
  fprintf(stream, "\tmov   BYTE  [rbp-48+rax], 10\n");
  fprintf(stream, ".dump_loop:\n");
  fprintf(stream, "\tmov   rcx, QWORD  [rbp-56]\n");
  fprintf(stream, "\tmov   rdx, -3689348814741910323\n");
  fprintf(stream, "\tmov   rax, rcx\n");
  fprintf(stream, "\tmul   rdx\n");
  fprintf(stream, "\tshr   rdx, 3\n");
  fprintf(stream, "\tmov   rax, rdx\n");
  fprintf(stream, "\tsal   rax, 2\n");
  fprintf(stream, "\tadd   rax, rdx\n");
  fprintf(stream, "\tadd   rax, rax\n");
  fprintf(stream, "\tsub   rcx, rax\n");
  fprintf(stream, "\tmov   rdx, rcx\n");
  fprintf(stream, "\tmov   eax, edx\n");
  fprintf(stream, "\tlea   edx, [rax+48]\n");
  fprintf(stream, "\tmov   eax, 31\n");
  fprintf(stream, "\tsub   rax, QWORD  [rbp-8]\n");
  fprintf(stream, "\tmov   BYTE  [rbp-48+rax], dl\n");
  fprintf(stream, "\tadd   QWORD  [rbp-8], 1\n");
  fprintf(stream, "\tmov   rax, QWORD  [rbp-56]\n");
  fprintf(stream, "\tmov   rdx, -3689348814741910323\n");
  fprintf(stream, "\tmul   rdx\n");
  fprintf(stream, "\tmov   rax, rdx\n");
  fprintf(stream, "\tshr   rax, 3\n");
  fprintf(stream, "\tmov   QWORD  [rbp-56], rax\n");
  fprintf(stream, "\tcmp   QWORD  [rbp-56], 0\n");
  fprintf(stream, "\tjne   .dump_loop\n");
  fprintf(stream, "\tmov   eax, 32\n");
  fprintf(stream, "\tsub   rax, QWORD  [rbp-8]\n");
  fprintf(stream, "\tlea   rdx, [rbp-48]\n");
  fprintf(stream, "\tlea   rcx, [rdx+rax]\n");
  fprintf(stream, "\tmov   rax, QWORD  [rbp-8]\n");
  fprintf(stream, "\tmov   rdx, rax\n");
  fprintf(stream, "\tmov   rsi, rcx\n");
  fprintf(stream, "\tmov   edi, 1\n");
  fprintf(stream, "\tmov   rax, 1\n");
  fprintf(stream, "\tsyscall\n");
  fprintf(stream, "\tnop\n");
  fprintf(stream, "\tleave\n");
  fprintf(stream, "\tret\n");
  fprintf(stream, "global _start\n");
  fprintf(stream, "segment .text\n");	
}
size_t macro_offset = 0;
void gfsl_compile_inst(FILE* stream, Inst inst){  
  make_label(inst);
  
  switch(inst.type){
  case Op_type::PUSH_INT:
    fprintf(stream, "\tpush %lu\n", inst.operand);
    break;
  case Op_type::PUSH_PTR:
    if(inst.operand > 0){
      fprintf(stream, "\tmov rax, buffer\n");
      fprintf(stream, "\tadd rax, %lu\n", inst.operand);
      fprintf(stream, "\tpush rax\n");
    }
    else {
      fprintf(stream, "\tpush buffer\n");
    }	
    break;
  case Op_type::PUSH_LOCAL_PTR: {
    fprintf(stream, "\tmov rax, [ret_stack_rsp]\n");
    fprintf(stream, "\tadd rax, %zu\n", inst.operand);
    fprintf(stream, "\tpush rax\n");      
  } break;
  case PUSH_STATIC_PTR: {
    fprintf(stream, "\tpush _STATIC%zu\n", inst.operand);
    break;
  }
  case Op_type::PUSH_STR:
    fprintf(stream, "\tpush %zu\n", strlen(global_data.str[inst.operand]));
    fprintf(stream, "\tpush STR%zu\n", inst.operand);
    break;
  case Op_type::DUP: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpush rax\n");
    fprintf(stream, "\tpush rax\n");
    break;
  }
  case Op_type::PLUS: {
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tadd rax, rbx\n");
    fprintf(stream, "\tpush rax\n");      
  } break;
  case Op_type::MINUS: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tsub rbx, rax\n");
    fprintf(stream, "\tpush rbx\n");      
  } break;
  case Op_type::MULT: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tmul rbx\n");
    fprintf(stream, "\tpush rax\n");

  } break;
  case Op_type::SWAP: { 
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpush rax\n");
    fprintf(stream, "\tpush rbx\n");
  } break;
  case Op_type::ARGC: {
    fprintf(stream, "mov rax, [args_ptr]\n");
    fprintf(stream, "\tpush rax\n");
  } break;
  case Op_type::ARGV: {
    fprintf(stream, "\tmov rax, [args_ptr]\n");
    fprintf(stream, "\tadd rax, 8\n");
    fprintf(stream, "\tpush rax\n");      
  } break;
  case Op_type::OVER:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpush rbx\n");
    fprintf(stream, "\tpush rax\n");
    fprintf(stream, "\tpush rbx\n");
    break;      
  case Op_type::DROP:
    fprintf(stream, "\tpop rax\n");
    break;
  case Op_type::ROT:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpop rcx\n");
    fprintf(stream, "\tpush rbx\n");
    fprintf(stream, "\tpush rax\n");
    fprintf(stream, "\tpush rcx\n");
    break;
  case Op_type::SYSCALL0:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");      
    break;
  case Op_type::SYSCALL1:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");      
    break;
  case Op_type::SYSCALL2:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tpop rsi\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");
    break;
  case Op_type::SYSCALL3:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tpop rsi\n");
    fprintf(stream, "\tpop rdx\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");
    break;
  case Op_type::SYSCALL4:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tpop rsi\n");
    fprintf(stream, "\tpop rdx\n");
    fprintf(stream, "\tpop r10\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");            
    break;
  case Op_type::SYSCALL5:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tpop rsi\n");
    fprintf(stream, "\tpop rdx\n");
    fprintf(stream, "\tpop r10\n");
    fprintf(stream, "\tpop r8\n");
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");      
    break;
  case Op_type::SYSCALL6:
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tpop rsi\n");
    fprintf(stream, "\tpop rdx\n");
    fprintf(stream, "\tpop r10\n");
    fprintf(stream, "\tpop r8\n");
    fprintf(stream, "\tpop r9\n");      
    fprintf(stream, "\tsyscall\n");
    fprintf(stream, "\tpush rax\n");      
    break;
  case Op_type::STORE8: {
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tmov BYTE [rax], bl\n");
  } break;
  case Op_type::STORE64: {
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tmov [rax], rbx\n");
  } break;
  case Op_type::LOAD8: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\txor rbx, rbx\n");
    fprintf(stream, "\tmov bl, BYTE [rax]\n");
    fprintf(stream, "\tpush rbx\n");
  } break;
  case Op_type::LOAD64: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\txor rbx, rbx\n");
    fprintf(stream, "\tmov rbx, [rax]\n");
    fprintf(stream, "\tpush rbx\n");
      
  } break;
  case Op_type::IF: break;
  case Op_type::WHILE: break;
  case Op_type::DO: {
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\ttest rax, rax\n");
    fprintf(stream, "\tjz .L%zu\n", inst.operand + macro_offset);
  } break;
  case Op_type::END: {
    fprintf(stream, "\tjmp .L%zu\n", inst.operand + macro_offset);    
  } break; 
  case Op_type::DUMP:
    fprintf(stream, "\tpop rdi\n");
    fprintf(stream, "\tcall dump\n");
    break;
  case Op_type::EXTEND_MACRO: {
    MACRO macro = macros.macros[inst.operand];
    fprintf(stream, "\t;;START MACRO: %s, of length: %zu\n",
	    macro.name,
	    macro.body_size);
    {

      for(size_t i=0; i < macro.body_size; ++i){	
	gfsl_compile_inst(stream, macro.body[i]);
	macro_offset++;
      }
    }
    fprintf(stream, "\t;;END MACRO: %s\n", macro.name);
  } break;
  case Op_type::CALL_PROC: {
    // PROC pos at operand
    PROC procedure = procs.procs[inst.operand];
    if(procedure.is_inline){
      fprintf(stream, "\t;;start of inline procedure %s\n", procedure.name);

      for(size_t i=0; i< procedure.body_size; ++i){
	gfsl_compile_inst(stream, procedure.body[i]);
	macro_offset++;
      }
      fprintf(stream, "\t;;end of inline procedure %s\n", procedure.name);
    }      
    else {
      fprintf(stream, "\tmov rax, rsp\n");
      fprintf(stream, "\tmov rsp, [ret_stack_rsp]\n");
      fprintf(stream, "\tcall P%zu\n", inst.operand);
      fprintf(stream, "\tmov [ret_stack_rsp], rsp\n");
      fprintf(stream, "\tmov rsp, rax\n");	
    }      
  } break;
  case Op_type::PROC_RETURN: {
    fprintf(stream, "\tmov rax, rsp\n");
    fprintf(stream, "\tmov rsp, [ret_stack_rsp]\n");
    if(current_proc->have_local){
      fprintf(stream, "\tadd rsp, %zu\n", current_proc->locals.total_offset - 1);
    }
    fprintf(stream, "\tret\n");
  } break;
  case Op_type::LTHAN: {
    fprintf(stream, "\tmov rcx, 0\n");
    fprintf(stream, "\tmov rdx, 1\n");
    fprintf(stream, "\tpop rbx\n");
    fprintf(stream, "\tpop rax\n");
    fprintf(stream, "\tcmp rax, rbx\n");
    fprintf(stream, "\tcmovl rcx, rdx\n");
    fprintf(stream, "\tpush rcx\n");

  } break;
  case Op_type::DUMP_STACK:
  case Op_type::CAST_INT:
  case Op_type::CAST_PTR:
  case Op_type::CAST_BOOL:
    break;
  default:
    fprintf(stderr,
	    "ERROR: unknown instruction %s.\n",
	    human_inst(inst));
    exit(1);
  }
}
void gfsl_compile_procs(FILE* stream){
  for(size_t i=0; i < procs.procs_size; ++i){
    PROC proc = procs.procs[i];
    current_proc = &proc;
    size_t locals_usage = proc.locals.total_offset - 1;
    if(!proc.is_inline){
      instruction_number = 0;
      fprintf(stream, "P%zu: ;; Procedure '%s'\n", i, proc.name);
      if(proc.have_local){
	fprintf(stream, "\tsub rsp, %zu\n", locals_usage);
      }
      fprintf(stream, "\tmov [ret_stack_rsp], rsp\n");
      fprintf(stream, "\tmov rsp, rax\n");
      
      for(size_t j=0; j < proc.body_size; ++j){
	gfsl_compile_inst(stream, proc.body[j]);
      }
      macro_offset = 0;
      make_ret_label(proc.name);
      fprintf(stream, "\tmov rax, rsp\n");
      fprintf(stream, "\tmov rsp, [ret_stack_rsp]\n");
      if(proc.have_local){
	fprintf(stream, "\tadd rsp, %zu\n", locals_usage); // total mem
      }
      fprintf(stream, "\tret\n");

    }
    current_proc = NULL;
  }
}
void gfsl_compile_program(gfsl_vr* vr, const char* output_filepath){

  FILE* stream = fopen(output_filepath, "wb");
  assert(stream);
  generate_header(stream);
  for(size_t i=0; i < vr->program_size; ++i){
    gfsl_compile_inst(stream, vr->program[i]);
  }
  macro_offset = 0;
  gfsl_compile_procs(stream);
  {
    int main_loc = PROCS_pos("main");
    if(main_loc == -1){
      fprintf(stderr,
	      "ERROR: could not find the 'main' procedure.\n");
      exit(1);
    }
    fprintf(stream, "_start:\n");
    fprintf(stream, "\tmov [args_ptr], rsp\n");
    fprintf(stream, "\tmov rax, ret_stack_end\n");
    fprintf(stream, "\tmov [ret_stack_rsp], rax\n");                
    fprintf(stream, "\tcall P%i\n", main_loc);
    fprintf(stream, "\tmov rdi, 0\n");
    fprintf(stream, "\tmov rax, 60\n");
    fprintf(stream, "\tsyscall\n");
    
  }
  if(global_data.str_size > 0){
    fprintf(stream, "segment .data\n");
    for(size_t strloc=0; strloc < global_data.str_size; ++strloc){
      fprintf(stream, "STR%zu: db \"%s\", 0\n",
	      strloc,
	      global_data.str[strloc]);
    }
    
  }
  
  fprintf(stream, "segment .bss\n");
  fprintf(stream, "args_ptr: resq 1 \n");
  fprintf(stream, "ret_stack_rsp: resq 1 \n");
  fprintf(stream, "ret_stack: resb %i\n", GFSL_COMPILED_RETURN_STACK_CAPACITY);
  fprintf(stream, "ret_stack_end: resq 1 \n");
  
  for(size_t staticloc=0; staticloc<static_memory.statics_size; staticloc++){
    fprintf(stream, "_STATIC%zu: resb %zu\n",
	    staticloc,
	    static_memory.statics[staticloc].size);
  }
  fclose(stream);
}

#endif /*  __gfsl */

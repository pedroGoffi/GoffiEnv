/*
TODO: make native call only works with native names nstead of labels and vice versa
*/
#ifndef __machine__
#define __machine__
#include <iostream>
#include "./errors.cpp"
#include "./lexer.cpp"
#include "../../common/utils.cpp"
#include "../../common/string.cpp"
#include <unistd.h>

#define writefd(...) fprintf(fd, __VA_ARGS__)
#define MACHINE_STACK_CAPACITY		1024
#define MACHINE_PROGRAM_CAPACITY	1024
#define LABEL_CAPACITY			1024
#define UNSOLVED_ADDR_CAPACITY		1024
#define MACHINE_NATIVE_ID_CAPACITY	1024
#define GLOBAL_DEF_CAPACITY             1024
#define GFM_MAX_INCLUDE_LEVEL           1024
#define MN_MEMORY_CAPACITY              1024
#define GLOBAL_STRING_CAPACITY          1023
#define CFLAG_LT   1
#define CFLAG_LTE  2
#define CFLAG_EQ   3
#define CFLAG_NEQ  4
#define CFLAG_GTE  5
#define CFLAG_GT   6
static size_t rec_level = 0;
typedef struct Machine Machine;
typedef struct Inst Inst;
typedef struct Label Label;
typedef struct Unsolved_Addr Unsolved_Addr;
typedef struct Native_id Native_id;
typedef struct Define Define;
typedef struct Global_Def Global_Def;
typedef struct Buffer Buffer;;
typedef union  Word Word;
typedef int64_t Memory_Addr;
typedef int64_t Inst_Addr;

typedef CPUErrors (*Machine_native)(Machine*);
void Buffer_write(Buffer* buffer, void* data, size_t size);
void Machine_slurp_code_from_file(Machine* mn, const char* fp);
void Machine_slurp_file_node(Machine* mn, const char* fp);
union Word{
  union{
    int64_t i64;
    double  f64;
    void*   ptr;
  } as;
};
#define Word_i64(n) Word{.as={.i64=(n)}}
#define Word_f64(n) Word{.as={.f64=(n)}}
#define Word_ptr(n) Word{.as={.ptr=(n)}}
struct Define{
  InternStr name;
  Word      value;
  
};
struct Global_Def{
  Define defines[GLOBAL_DEF_CAPACITY];
  size_t defines_size;
  Global_Def() = default;
};
Global_Def global_def = {};
void Global_Def_push(Global_Def* df, const char* name, Word value){
  assert(df->defines_size <= GLOBAL_DEF_CAPACITY);
  df->defines[df->defines_size++] = {
    .name  = Internstr(name),
    .value = value
  };
}
int Global_Def_find(Global_Def* df, const char* name){
  for(size_t i=0; i< df->defines_size; ++i){
    if(strcmp(df->defines[i].name.str, name) == 0){
      return (int)i;
    }
  }
  return -1;
}

struct Global_string{
  const char* var_name;
  size_t      str_loc;
  size_t      str_len;
};
struct Global_strings{
  Global_string strs[GLOBAL_STRING_CAPACITY];
  size_t        strs_size;
};
Global_strings global_strings = {};


enum InstType{
  Noop,
  Halt,
  Push,
  Dup,
  Not,
  Swap,
  Addi,  Addf,
  Minusi,Minusf,
  Multi, Multf,
  Divi,  Divf,
  Jmp,
  Jmp_if,
  Call,
  Ret,
  Cmp,
  Get_addr,
  Native,
  Load8,
  Load64,
  Store8,
  Store64
};
struct Buffer{
  size_t size;
  size_t capacity;
  char* data;
};
struct Inst{
  InstType type;
  Word     operand;
};
struct Machine{
  Word   stack[MACHINE_STACK_CAPACITY];
  size_t stack_size;
  
  Word   return_stack[MACHINE_STACK_CAPACITY];
  size_t return_stack_size;
  
  Inst   program[MACHINE_PROGRAM_CAPACITY];
  size_t program_size;
  size_t ip;
  bool   has_entry;
  int    entry_point;
  
  int8_t memory[MN_MEMORY_CAPACITY];
  size_t memory_size;

  Buffer buffer;

  int    halt;
  
  Machine_native natives[MACHINE_NATIVE_ID_CAPACITY];
  size_t natives_size;
  Machine() = default;
};
struct Label{
  InternStr name;
  Word      addr;
};
struct Unsolved_Addr{
  InternStr name;
  Word      addr;  
};

struct Native_Record{
  InternStr name;
  size_t    loc;
  Native_Record() = default;
};
struct Natives_Record{
  Native_Record records[MACHINE_NATIVE_ID_CAPACITY];
  size_t        records_size;
  Natives_Record() = default;
};
Natives_Record natives_record = {};
size_t Global_string_push(Machine* mn, const char* id, const char* str){
  global_strings.strs[global_strings.strs_size++] = {
    .var_name = id,
    .str_loc  = mn->buffer.size,
    .str_len  = strlen(str)
  };
  Buffer_write(&mn->buffer, (void*)str, strlen(str));
  return global_strings.strs_size - 1;
}
int Global_string_find(Machine* mn, const char* id){
  (void) mn;
  for(size_t i=0; i < global_strings.strs_size; ++i){
    if(strcmp(global_strings.strs[i].var_name, id) == 0){
      return (int)i;
    }
  }
  return -1;
}
Word Native_Record_find(const char* str){
  for(size_t i=0; i < natives_record.records_size; i++){
    if(strcmp(natives_record.records[i].name.str, str) == 0){
      return Word_i64((int)natives_record.records[i].loc);
    }
  }
  return Word_i64(-1);
}
struct Label_Table{
  Label labels[LABEL_CAPACITY];
  size_t labels_size;
  
  Unsolved_Addr unsolved_addr[UNSOLVED_ADDR_CAPACITY];
  size_t unsolved_addr_size;
  Label_Table() = default;
};

void Buffer_resize(Buffer* buffer, size_t new_cap){
  buffer->capacity = new_cap;  
  buffer->data     = (char*)realloc(buffer->data, buffer->capacity);
}
void Buffer_free(Buffer* buffer){
  free(buffer->data);
  buffer->data = NULL;
  buffer->size = 0;
  buffer->capacity = 0;
}
void Buffer_write(Buffer* buffer, void* data, size_t size){
  if(buffer->size + size > buffer->capacity){
    Buffer_resize(buffer, buffer->capacity * 2 + size);
  }
  memcpy(buffer->data + buffer->size, data, size);
  buffer->size += size;
}
void Buffer_write_cstr(Buffer* buffer, const char* cstr){
  Buffer_write(buffer, (void*)cstr, strlen(cstr));
}
Label_Table label_table = {};
// Buffer write_buffer = {};
Word Label_Table_find(const Label_Table* lt, InternStr str){
  for(size_t i=0; i<lt->labels_size; ++i){
    if(InternStr_eq(lt->labels[i].name, str)){
      return lt->labels[i].addr;
    }
  }
  return Word_i64(-1);
}
void Label_Table_push(Label_Table* lt, Label lb){
  assert(lt->labels_size < LABEL_CAPACITY);
  lt->labels[lt->labels_size++] = lb;
}
void Label_Table_push_unsolved_jmp(Label_Table* lt, Label lb){
  assert(lt->unsolved_addr_size < LABEL_CAPACITY);
  const Unsolved_Addr unsolved = {
    .name  = lb.name,
    .addr  = lb.addr
  };
  lt->unsolved_addr[lt->unsolved_addr_size++] = unsolved;
}
int Machine_push_inst(Machine* mn, Inst inst){
  assert(mn->program_size + 1 < MACHINE_PROGRAM_CAPACITY);
  mn->program[mn->program_size++] = inst;
  return (int)(mn->program_size - 1);
}

Word Word_by_token(Machine* mn, Token tk){
  if(tk.kind == TOKEN_INT){
    return Word_i64(tk.INT);
  } else if(tk.kind == TOKEN_NAME){
    Word Addr = Label_Table_find(&label_table, Internstr(tk.name));
    if (Addr.as.i64 >= 0){
      return Addr;
    } else if ((Addr = Native_Record_find(tk.name)).as.i64 != -1){
      return Addr;
    } else if ((Addr = Word_i64(Global_Def_find(&global_def, tk.name))).as.i64 != -1){
      return global_def.defines[Addr.as.i64].value;
    } else if ((Addr = Word_i64(Global_string_find(mn, tk.name))).as.i64 != -1){
      return Addr;
    } else {
      printf("WARNING: Unsolved: %s\n", tk.name);
      Label_Table_push_unsolved_jmp(&label_table, {
	  .name = Internstr(tk.name),
	  .addr = Word_i64((int)mn->program_size)
	});
    }
    return Word_i64(-1);
  } else if(tk.kind == TOKEN_FLOAT){
    return Word_f64(tk.FLOAT);
  }
  fprintf(stderr,
	  "ERROR: unexpected Word starts with: %s\n", tk.name);
  exit(1);
}

#define dumpf_word(str, wrd)				\
  fprintf(str, "<i64= %li,f64= %f,ptr= %p",		\
	  (wrd).as.i64,					\
	  (wrd).as.f64,					\
	  (wrd).as.ptr					\
    )
#define dump_word(wrd) dumpf_word(stdout, (wrd))
#define dumpf_word_i64(str, wrd)  fprintf((str), "%li", (wrd).as.i64)
const char* InstType_Cstr(InstType type){
  switch(type){
  case InstType::Halt:   return "halt";
  case InstType::Not:    return "not";
  case InstType::Cmp:    return "cmp";
  case InstType::Push:	 return "push";
  case InstType::Swap:   return "swap";
  case InstType::Addi:	 return "addi";
  case InstType::Minusi: return "minusi";    
  case InstType::Multi:	 return "multi";
  case InstType::Divi:	 return "divi";
  case InstType::Native: return "native";
  case InstType::Addf:	 return "addf";
  case InstType::Minusf: return "minusf";    
  case InstType::Multf:	 return "multf";
  case InstType::Get_addr:return "get_addr";
  case InstType::Divf:	 return "divf";
  case InstType::Jmp:	 return "jmp";
  case InstType::Jmp_if: return "jmp_if";
  case InstType::Ret:	 return "ret";
  case InstType::Call:   return "call";    
  case InstType::Dup:	 return "dup";
  case InstType::Noop:	 return "noop";
  case InstType::Store8: return "store8";
  case InstType::Store64:return "store";
  case InstType::Load8:  return "load8";
  case InstType::Load64: return "load";
  default:
    abort();
  }
}
bool InstType_has_operand(InstType type){
  switch(type){
  case InstType::Push:
  case InstType::Swap:
  case InstType::Jmp:
  case InstType::Jmp_if:    
  case InstType::Dup:
  case InstType::Noop:
  case InstType::Call:
  case InstType::Native:
  case InstType::Cmp:
    return 1;
  case InstType::Halt:
  case InstType::Addi:   case InstType::Addf:
  case InstType::Minusi: case InstType::Minusf:
  case InstType::Multi:  case InstType::Multf:
  case InstType::Divi:   case InstType::Divf:
  case InstType::Ret:
  case InstType::Load8:  case InstType::Load64:
  case InstType::Store8: case InstType::Store64:
  case InstType::Not:
  case InstType::Get_addr:
    return 0;
  default:
    abort();
  }
}

#define Make_inst(Type, Operand) (*(new Inst{.type=(Type), .operand=(Operand)}))
#define Inst_push(operand) Make_inst(InstType::Push, operand)
#define Inst_plus Make_inst(InstType::Plus, 0)
#define Inst_div Make_inst(InstType::Div, 0)
#define Inst_dup(v) Make_inst(InstType::Dup, (v))
#define Inst_jmp(addr) Make_inst(InstType::Jmp, addr)
CPUErrors Machine_alloc(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  mn->stack[mn->stack_size-1].as.ptr = malloc(mn->stack[mn->stack_size-1].as.i64);
  return CPUErrors::OK;
}
CPUErrors Machine_free(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  free(mn->stack[mn->stack_size - 1].as.ptr);
  mn->stack_size--;
  return CPUErrors::OK;
}
CPUErrors Machine_print_i64(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  printf("%li\n", mn->stack[mn->stack_size - 1].as.i64);
  mn->stack_size--;
  return CPUErrors::OK;
  
}
CPUErrors Machine_print_f64(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  printf("%lf\n", mn->stack[mn->stack_size - 1].as.f64);
  mn->stack_size--;
  return CPUErrors::OK;
}
CPUErrors Machine_print_ptr(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  printf("%p\n", mn->stack[mn->stack_size - 1].as.ptr);
  mn->stack_size--;
  return CPUErrors::OK;
}
CPUErrors Machine_exit(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  exit((int)mn->stack[mn->stack_size--].as.i64);
  mn->halt = 1;
  return CPUErrors::OK;
}
CPUErrors Machine_print_memory(Machine* mn){
  if( mn->stack_size < 2)
    return CPUErrors::StackUnderflow;

  Memory_Addr addr   = mn->stack[mn->stack_size - 2].as.i64;
  int64_t     count  = mn->stack[mn->stack_size - 1].as.i64;
  if(addr  >= MN_MEMORY_CAPACITY){
    return CPUErrors::IlegalMemoryAccess;
  }
  if(addr + count < addr){
    return CPUErrors::IlegalMemoryAccess;
  }
  if(addr + count >= MN_MEMORY_CAPACITY){
    return CPUErrors::IlegalMemoryAccess;
  }
  
  for( int64_t i=addr; i < count; i++){
    printf("%02x ", mn->memory[addr + i]);    
  }
  printf("\n");
  mn->stack_size -= 2;
  return CPUErrors::OK;
}
CPUErrors Machine_write(Machine* mn){
  if( mn->stack_size < 2)
    return CPUErrors::StackUnderflow;

  Memory_Addr str_addr    = mn->stack[mn->stack_size - 3].as.i64;
  int64_t     str_len     = mn->stack[mn->stack_size - 2].as.i64;
  Memory_Addr file_stream = mn->stack[mn->stack_size - 1].as.i64;

  if(str_addr  >= MN_MEMORY_CAPACITY){
    return CPUErrors::IlegalMemoryAccess;
  }
  if(str_addr + str_len < str_len){
    return CPUErrors::IlegalMemoryAccess;
  }
  if(str_addr + str_len >= MN_MEMORY_CAPACITY){
    return CPUErrors::IlegalMemoryAccess;
  }   

  //write(int, const void*, size_t);
  size_t data_addr = global_strings.strs[str_addr].str_loc;
  write((int)file_stream,
	(const void*)(mn->buffer.data + data_addr),
	str_len);
  
  mn->stack_size -= 3;
  return CPUErrors::OK;
}
CPUErrors Machine_print_stack(Machine* mn){
  if( mn->stack_size < 1){
    return CPUErrors::StackUnderflow;
  }
  printf("stack:\n");
  for( size_t i=0; i < mn->stack_size; i++){
    dump_word(mn->stack[i]);
    printf("\n");
  }
  mn->stack_size -= 2;
  return CPUErrors::OK;
  
}
void Machine_push_native(Machine* mn, Machine_native native, const char* native_name){
  assert(mn->natives_size < MACHINE_NATIVE_ID_CAPACITY);  
  mn->natives[mn->natives_size] = native;
  natives_record.records[natives_record.records_size].name = Internstr(native_name);
  natives_record.records[natives_record.records_size].loc  = mn->natives_size;  
  natives_record.records_size++;
  mn->natives_size++;  
}


void Machine_push_all_natives(Machine* mn){
  Machine_push_native(mn, Machine_alloc,	"alloc");
  Machine_push_native(mn, Machine_free,		"free");
  Machine_push_native(mn, Machine_exit,		"exit");
  Machine_push_native(mn, Machine_write,        "write");
  Machine_push_native(mn, Machine_print_i64,	"print_i64");
  Machine_push_native(mn, Machine_print_f64,	"print_f64");
  Machine_push_native(mn, Machine_print_memory, "print_memory");
  Machine_push_native(mn, Machine_print_stack,  "print_stack");
  Machine_push_native(mn, Machine_print_ptr,    "print_ptr");
}
CPUErrors Machine_execute(Machine* mn){
  if( mn->ip > mn->program_size) {
    printf("ilegal access\n");
    printf("\tip = %li\n", mn->ip);
    printf("\tprogram_size = %zu\n", mn->program_size);
    return CPUErrors::IlegalInstructionAccess;
  }
  
  Inst inst = mn->program[mn->ip];
  switch(inst.type){
  case InstType::Noop:
    mn->ip++;
    break;
  case InstType::Get_addr: {
    if( mn->stack_size >= MACHINE_STACK_CAPACITY )
      return CPUErrors::StackOverflow;
    *(void**)&mn->stack[mn->stack_size - 1].as.i64 = &mn->stack[mn->stack_size - 1].as.ptr;
    mn->ip++;
  } break;    
  case InstType::Not:
    mn->stack[mn->stack_size - 1].as.i64 = !mn->stack[mn->stack_size - 1].as.i64;
    mn->ip++;
    break;
  case InstType::Halt:
    mn->halt = 1;
    mn->ip++;
    break;
  case InstType::Push:
    if( mn->stack_size >= MACHINE_STACK_CAPACITY )
      return CPUErrors::StackOverflow;
    mn->stack[mn->stack_size++] = inst.operand;
    mn->ip++;
    break;
  case InstType::Swap: {
    if( mn->stack_size < 2){
      return CPUErrors::StackUnderflow;
    }
    const Word tmp = mn->stack[mn->stack_size - 1];
    mn->stack[mn->stack_size - 1] = mn->stack[mn->stack_size - 2];
    mn->stack[mn->stack_size - 2] = tmp;
    mn->ip++;
    
  } break;
  case InstType::Dup:{
    if( mn->stack_size - inst.operand.as.i64 <= 0){
      return CPUErrors::StackUnderflow;
    }
    assert( inst.operand.as.i64 >= 0);
    
    mn->stack[mn->stack_size] =
      mn->stack[mn->stack_size - 1 - inst.operand.as.i64];
    mn->stack_size++;
    mn->ip++;
  } break;
    
#define InstType_bin(__type__, __OP__)					\
    {									\
      if( mn->stack_size < 2)						\
	return CPUErrors::StackUnderflow;				\
      mn->stack[mn->stack_size - 2].as.__type__ __OP__ mn->stack[mn->stack_size - 1].as.__type__; \
      mn->stack_size--;							\
      mn->ip++;							\
    }    
  case InstType::Minusi:	InstType_bin(i64, -=); break;
  case InstType::Addi:		InstType_bin(i64, +=); break;    
  case InstType::Multi:		InstType_bin(i64, *=); break;
  case InstType::Divi: {
    if( mn->stack[mn->stack_size - 2].as.i64 == 0)
      return CPUErrors::DivisionByZero;
    InstType_bin(i64, /=);
    break;
  }
  case InstType::Minusf:	InstType_bin(f64, -=); break;
  case InstType::Addf:		InstType_bin(f64, +=); break;    
  case InstType::Multf:		InstType_bin(f64, *=); break;
  case InstType::Divf: {
    if( mn->stack[mn->stack_size - 2].as.f64 == 0.f)
      return CPUErrors::DivisionByZero;
    InstType_bin(f64, /=);
    break;
  }
      
#undef InstType_bin
  case InstType::Cmp: {
    if( mn->stack_size < 2){
      return CPUErrors::StackUnderflow;
    }

    
     Word a = mn->stack[mn->stack_size - 2];
     Word b = mn->stack[mn->stack_size - 1];

     bool res;
     switch(inst.operand.as.i64){
     case CFLAG_LT:
       res= (int)(a.as.i64 < b.as.i64);
       break;
     case CFLAG_LTE:
       res= (int)(a.as.i64 <= b.as.i64);
       break;
     case CFLAG_EQ:
       res= (int)(a.as.i64 == b.as.i64);
       break;
     case CFLAG_NEQ:
       res= (int)(a.as.i64 != b.as.i64);
       break;
     case CFLAG_GT:
       res= (int)(a.as.i64 > b.as.i64);
       break;
     case CFLAG_GTE:
       res= (int)(a.as.i64 >= b.as.i64);
       break;
     default:
       printf("ERROR: undefined behavior of cmp operand.\n");
       abort();
     }
     mn->stack_size -= 2;
     mn->stack[mn->stack_size++].as.i64 = (int)res;
     mn->ip++;
  } break;    
  case InstType::Jmp:
    mn->ip = inst.operand.as.i64;
    break;
  case InstType::Jmp_if: {
    if(mn->stack_size < 1){
      return CPUErrors::StackUnderflow;
    }
    Word cond = mn->stack[mn->stack_size - 1];
    mn->stack_size--;
    if( cond.as.i64 == 1 )
      mn->ip = inst.operand.as.i64;
    else
      mn->ip++;

  } break;
  case InstType::Call:
    if( mn->return_stack_size >= MACHINE_STACK_CAPACITY )
      return CPUErrors::StackOverflow;
    mn->return_stack[mn->return_stack_size++].as.i64 = mn->ip + 1;
    mn->ip = inst.operand.as.i64;
    break;
  case InstType::Ret:
    if(mn->return_stack_size < 1){
      return CPUErrors::StackUnderflow;
    }
    mn->ip = mn->return_stack[--mn->return_stack_size].as.i64;

    break;
  case InstType::Native: {
      if( inst.operand.as.i64 >= (int)mn->natives_size){
	return CPUErrors::IlegalOperand;
      }
      CPUErrors err = mn->natives[inst.operand.as.i64](mn);
      mn->ip++;
      return err;
  } break;
  case InstType::Load8: {
    if( mn->stack_size < 1)
      return CPUErrors::StackUnderflow;
    const Memory_Addr addr = mn->stack[mn->stack_size - 1].as.i64;
    if( addr >= MN_MEMORY_CAPACITY ) {
      return CPUErrors::IlegalMemoryAccess;
    }
    mn->stack[mn->stack_size - 1].as.i64 = *(int8_t*)&mn->memory[addr];    
    mn->ip++;
  } break;
  case InstType::Load64: {
    if( mn->stack_size < 1)
      return CPUErrors::StackUnderflow;
    const Memory_Addr addr = mn->stack[mn->stack_size - 1].as.i64;
    if( addr + 7 >= MN_MEMORY_CAPACITY ) {
      return CPUErrors::IlegalMemoryAccess;
    }
    mn->stack[mn->stack_size - 1].as.i64 = *(int64_t*)&mn->memory[addr];
    mn->ip++;
  } break;    
  case InstType::Store8: {
    if( mn->stack_size < 1)
      return CPUErrors::StackUnderflow;
    const Memory_Addr addr = mn->stack[mn->stack_size - 2].as.i64;
    if( addr >= MN_MEMORY_CAPACITY ) {
      return CPUErrors::IlegalMemoryAccess;
    }
    mn->memory[addr] = (int8_t)mn->stack[mn->stack_size - 1].as.i64;
    mn->stack_size -= 2;
    mn->ip++;
  } break;    
  case InstType::Store64: {
    if( mn->stack_size < 2)
      return CPUErrors::StackUnderflow;
    const Memory_Addr addr = mn->stack[mn->stack_size - 2].as.i64;
    if( addr >= MN_MEMORY_CAPACITY ) {
      return CPUErrors::IlegalMemoryAccess;
    }
    *(int64_t*)&mn->memory[addr] = *(int64_t*)&mn->stack[mn->stack_size - 1].as.i64;
    mn->stack_size -= 2;
    mn->ip++;
  } break;        
  default:
    return CPUErrors::IlegalInst;      
  }
  return CPUErrors::OK;
}
void Machine_dump(Machine* mn){
  printf("Stack:[\n");
  if( mn->stack_size == 0){
    printf("\tEmpty\n");
  }
  else {
    for(size_t i=0; i < mn->stack_size; ++i){
      dump_word(mn->stack[i]);
      printf("\n");
    }
  }
  printf("]\n");
}
void Machine_abort(CPUErrors err){
  fprintf(stderr,
	  "CPU error identified :: %s\n", CPUErrors_Cstr(err));
  exit(1);
}


void Machine_run(Machine* mn, size_t MAX_INST=0, bool debug=false){
  MAX_INST = MAX_INST != 0? MAX_INST: ARRAY_SIZE(mn->program);
  mn->halt             = 0;
  size_t inst_count    = 0;
  while(!mn->halt){
    if(inst_count++ >= MAX_INST) break;
    CPUErrors err = Machine_execute(mn);
    if(err != CPUErrors::OK)
      Machine_abort(err);
    if(debug) {

      printf("-------------------------------\n");
      Machine_dump(mn);
      printf("-------------------------------\n");
      sleep(1);
    }
    //if(mn->halt) break;
  }
}
void Machine_save_to_file(Machine* mn, const char* fp="a.gfbm"){
  FILE *fd = fopen(fp, "wb");
  assert(fd);
  fwrite(mn->program,
	 sizeof(mn->program[0]),
	 mn->program_size,
	 fd);
  if(ferror(fd)){
    fprintf(stderr,
	    "ERROR: could not open the file `%s` : %s\n",
	    fp, strerror(errno));
    abort();
  }
  fclose(fd);
}
void Machine_load_program_from_file(Machine* mn, const char* fp){
  FILE* fd = fopen(fp, "rb");
  assert(fd);
  if(fseek(fd, 0, SEEK_END) < 0){
    fprintf(stderr,
	    "ERROR: could not read the file: %s.\n",
	    fp);
    abort();
  }
  long f_size = ftell(fd);
  if( f_size < 0){
    fprintf(stderr,
	    "ERROR: could not read the file: %s.\n",
	    fp);
    abort();
  }
  assert(f_size % sizeof(mn->program[0]) == 0);
  assert((size_t)f_size <= MACHINE_PROGRAM_CAPACITY * sizeof(mn->program[0]));
  if (fseek(fd, 0, SEEK_SET) < 0 ){
    fprintf(stderr,
	    "ERROR: could not read the file: %s.\n",
	    fp);
    abort();
  }
  mn->program_size = fread(mn->program,
			   sizeof(mn->program[0]),
			   (f_size / sizeof(mn->program[0])),
			   fd);
  if(ferror(fd)){
    fprintf(stderr,
	    "ERROR: could not read the file: %s.\n",
	    fp);
    abort();
  }
  fclose(fd);  
}

void Machine_translate_line_into_inst(Machine* mn){
  if(expect_name("push")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Push,
      .operand = operand
    };    
  } else if (expect_name("dup")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Dup,
      .operand = operand
    };
  } else if (expect_name("minusi")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Minusi,
      .operand = Word_i64(0)
    };
  } else if (expect_name("addi")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Addi,
      .operand = Word_i64(0)
    };
  } else if (expect_name("addf")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Addf,
      .operand = Word_i64(0)
    };
  } else if (expect_name("multi")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Multi,
      .operand = Word_i64(0)
    };
  } else if (expect_name("multf")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Multf,
      .operand = Word_i64(0)
    };
  } else if (expect_name("divi")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Divi,
      .operand = Word_i64(0)
    };
  } else if (expect_name("divf")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Divi,
      .operand = Word_i64(0)
    };
  } else if (expect_name("cmp")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Cmp,
      .operand = operand
    };    
  } else if (expect_name("jmp")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Jmp,
      .operand = operand
    };    
  } else if (expect_name("jmp_if")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Jmp_if,
      .operand = operand
    };    
  } else if (expect_name("halt")){    
    //const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Halt,
      .operand = Word_i64(0)
    };
  } else if (expect_name("swap")){
    const Word operand = Word_by_token(mn, consume());
    mn->program[mn->program_size++] = {
      .type    = InstType::Swap,
      .operand = operand
    };
  } else if (expect_name("ret")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Ret,
      .operand = Word_i64((int)mn->program_size)
    };
  } else if (expect_name("call")){
    const Word operand = Word_by_token(mn, consume());
    mn->program[mn->program_size++] = {
      .type    = InstType::Call,
      .operand = operand
    };
  } else if (expect_name("native")){
    Token tk = consume();
    const Word operand = Word_by_token(mn, tk);
    if(operand.as.i64 < 0){
      printf("could not find the native id for: %s.\n", tk.name);
      exit(1);
    }
    mn->program[mn->program_size++] = {
      .type    = InstType::Native,
      .operand = operand
    };
  }
  else if (expect_name("load8")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Load8,
      .operand = Word_i64(0)
    };
  }
  else if (expect_name("load")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Load64,
      .operand = Word_i64(0)
    };
  }
  else if (expect_name("store8")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Store8,
      .operand = Word_i64(0)
    };
  }
  else if (expect_name("store")){
    mn->program[mn->program_size++] = {
      .type    = InstType::Store64,
      .operand = Word_i64(0)
    };
  }
  else if (expect_name("push_str")){
    const Word operand = Word_by_token(mn, consume());    
    mn->program[mn->program_size++] = {
      .type    = InstType::Push,
      .operand = operand
    };
    size_t str_len = global_strings.strs[operand.as.i64].str_len;    
    mn->program[mn->program_size++] = {
      .type    = InstType::Push,
      .operand = Word_i64((int)(str_len))
    };
  }
  
  else {
    Token unknown = consume();
    if(unknown.kind == TOKEN_NAME){
      if(expect_token(TOKEN_DOUBLE_DOT)){
	Label_Table_push(&label_table, {
	    .name = Internstr(unknown.name),
	    .addr = Word_i64((int)mn->program_size)
	  });
      }
      else {
	printf("ERROR: unexpected instructions: %s\n", unknown.name);
	exit(1);
      }
    } else if (unknown.kind == TOKEN_HASHTAG){
      if(expect_name("entry")){
	if(!mn->has_entry){
	  mn->entry_point = (int)mn->program_size;
	  mn->has_entry = true;
	} else {
	  printf("ERROR: the byte code can only have one entry point.\n");
	  exit(1);
	}
      } else if (expect_name("include")){
	assert(token.kind == TOKEN_STRING);
	const char* lstr = stream;
	Machine_slurp_file_node(mn, consume().name);
	init_stream(lstr);
      } else if (expect_name("define")){
	Token tk = consume();
	assert(tk.kind == TOKEN_NAME);
	Word value = Word_by_token(mn, consume());
	Global_Def_push(&global_def, tk.name, value);
      } else if (expect_name("string")){
	Token tk = consume();
	assert(tk.kind == TOKEN_NAME);
	const char* str = consume().STRING;
	Global_string_push(mn, tk.name, str);	
      }
      else {
	printf("ERROR: unexpected pre-processor instructions: %s\n",
	       token.name);
      }
    }
    else{
      printf("ERROR: unexpected instructions: %s\n", token.name);
      exit(1);
    }
  } 
  //Token unknown = consume();
  // TODO:
  // check for:
  // [1] pre-processor instructions
  // [2] labels
  
}

void Machine_parse_source_code(Machine* mn, const char* src){  
  init_stream(src);
  while(!is_token(TOKEN_EOF)){
    Machine_translate_line_into_inst(mn);    
  }
}
void Machine_slurp_file_node(Machine* mn, const char* fp){
  if(rec_level++ >= GFM_MAX_INCLUDE_LEVEL){
    printf("ERROR: Auto recursive include hits maximum recursive level: %i\n", GFM_MAX_INCLUDE_LEVEL);
    printf("NOTE: include file path: %s\n", fp);
    exit(1);
  }
  const char* file_content = get_file_text(fp);
  Machine_parse_source_code(mn, file_content);
}
void Machine_slurp_code_from_file(Machine* mn, const char* fp){
  rec_level++;
  Machine_slurp_file_node(mn, fp);
  for(size_t i=0; i< label_table.unsolved_addr_size; i++){
    Word Addr_loc =
      Label_Table_find(&label_table,
		       label_table.unsolved_addr[i].name);
    
    printf("Addr_loc --> %li\n", Addr_loc.as.i64);
    if(Addr_loc.as.i64 < 0){
      printf("ERROR: unsolved label: %s.\n",
	     label_table.unsolved_addr[i].name.str);
    }
    mn->program[label_table.unsolved_addr[i].addr.as.i64].operand = Addr_loc;
  }
  if(!mn->has_entry){
    Word main_loc = Label_Table_find(&label_table, Internstr("main"));
    if(main_loc.as.i64 < 0){
      printf("ERROR: could not find the main label, please define it as the entry point.\n");
      exit(1);
    }
    mn->program[0] = {
      .type    = InstType::Jmp,
      .operand = main_loc
    };
  }
}

void Machine_dump_program(FILE* stream, Machine* mn){
  for(size_t i=0; i< mn->program_size; ++i){  
    Inst inst = mn->program[i];
    fprintf(stream, "%s ", InstType_Cstr(inst.type));
    if(InstType_has_operand(inst.type))
      dumpf_word_i64(stream, inst.operand);
    fprintf(stream, "\t\t// inst: %zu \n", i);
  }
}
void Machine_decompile_to_file(Machine* mn, const char* outfp){
  FILE* fd = fopen(outfp, "wb");
  assert(fd);
  Machine_dump_program(fd, mn);
}


#endif /* __machine__ */

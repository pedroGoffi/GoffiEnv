#ifndef CORE_INSTRUCTIONS
#define CORE_INSTRUCTIONS 
#include <stdint.h>
#include <string>

typedef enum{
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
  OP_PLUS,
  OP_MINUS,
  OP_DIVMOD,
  OP_IDIVMOD,
  OP_EQUALS,
  OP_NOT_EQUALS,
  OP_MULT,
  OP_GTHAN,
  OP_LTHAN,    
  OP_MEM,
  ARGC,
  ARGV,
  DUMP,
  DUP,
  DROP, 
  OVER, // a b -> a b a 
  SWAP, // a b -> b a
  ROT,// a b c -> c b a
  PUSH_PTR,
  //  -----
  SHIFT_LEFT,
  SHIFT_RIGHT,
  OR,
  AND,
  NOT,
  CAST,
  // -----


  OP_IF,
  OP_ELSE,
  OP_WHILE,
  OP_DO,
  OP_END,


  OP_SYSCALL,


  OP_LOADBYTE,
  OP_STOREBYTE,

  NUM_OF_OPERANDS        
} Op_type;

typedef struct VR{
  Op_type  op;
  uint64_t    operand;
  std::string op_string="";
  uint64_t	scape_count=0;
}VR;
#endif /* ifndef CORE_INSTRUCTIONS */

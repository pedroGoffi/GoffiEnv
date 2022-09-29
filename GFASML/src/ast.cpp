#ifndef   GFASM_AST
#define   GFASM_AST
#include <iostream>
#include "../../common/utils.cpp"
typedef struct Module Module;
typedef struct file_node file_node;
typedef struct Node_procedure Node_procedure;
typedef struct Procedure_block Procedure_block;
typedef struct Instruction Instruction;
typedef struct Instruction_expr Instruction_expr;

enum Expr_kind{
  expr_int
};
struct Instruction_expr{
  Expr_kind expr_kind;
  union{
    int INT;
  } as;
};
enum Instruction_kind{
  PUSH,
  NOOP
};
struct Instruction{
  Instruction_kind instruction_kind;
  Instruction_expr instruction_expr;
};

struct Procedure_block{
  size_t        stack_allocation_size;
  Instruction** instructions;
};
struct Node_procedure{
  const char* name;
  Procedure_block* procedure_block;
};

enum File_node_kind{
  node_procedure
};
struct File_node{
  File_node_kind kind;
  union{
    Node_procedure* node_procedure;
  };
};
struct Module{
  // TODO: FILE_CONTEXT
  File_node* file_node;
};
#endif /* GFASM_AST */


#ifndef __instruction
#define __instruction


enum operation_kind{
  noop,
  push,
  add_INT,
  dump_INT,
  dump_STRING,
  TODO_cmp_gt,
  cmp_lt,
  TODO_cmp_eq,
  TODO_jmp_if_zero,
  jmp,
  jmp_if,
  _abort,
  halt
};

union Value{
  int         INT;
  bool        BOOL;
  const char* STRING;
};
struct operation{
  operation_kind kind;
  Value          as;
};
#endif /* __instruction */

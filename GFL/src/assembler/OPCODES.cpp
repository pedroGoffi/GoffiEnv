#ifndef __OPCODES__
#define __OPCODES__
#include "../ast.cpp"
enum OPCODE_kind{
  NOP,
  PUSH,
  LEA
};
const char* OPCODE_kind_cstr(OPCODE_kind kind){
  switch(kind){
#define CASE(k) case k: return (#k);
    CASE(NOP);
    CASE(PUSH);
    CASE(LEA);
#undef CASE
  }
  abort();
}
struct OPCODE{
  OPCODE_kind kind;
  const char* name;
  size_t      len;
};

OPCODE OPCODES[] = {
#define OP(k, l) (OPCODE{.kind=(OPCODE_kind::k), .name = OPCODE_kind_cstr(k), .len=(l)})
  OP(NOP, 0),
  OP(PUSH, 1),
  OP(LEA, 1)
#undef OP
};
#endif /*__OPCODES__*/

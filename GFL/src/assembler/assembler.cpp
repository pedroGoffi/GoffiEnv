#ifndef __assembler__
#define __assembler__
#include "../ast.cpp"
#include "./OPCODES.cpp"
#include "./OPERAND.cpp"
/*
  make a notion of opcode and operand
  <opcode> <operand>, <operand>
  <opcode> = {
    opkind 
    oplen
  }
  <operand> = {
    operandKind
    union operand_reg, operand_int, ...
  }
  mov QWORD [rsp - 8], 10
*/

void assembler_test(){
  printf("UNIMPLEMENTED %s.\n", OPCODE_kind_cstr(PUSH));
  auto* r = OPERAND_int(10);
  printf("sex? %s\n", OPERAND_kind_cstr(r->kind));
  for(auto& x: OPCODES){
    printf("opcode %s of len %zu\n",
	   x.name, x.len);
  }
}
#endif /*__assembler__*/

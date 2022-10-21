#include <iostream>
#include "src/gfsl.cpp"

int main() {
  printf("macro inst-type %zu end\n", offsetof(Inst, type));
  printf("macro inst-operand %zu end\n", offsetof(Inst, operand));

  printf("sizeof(struct inst) = %zu\n", sizeof(struct Inst));
  
  //Inst    program[GFSL_PROGRAM_CAPACITY];
  //size_t  program_size;
  //
  
  //
  

}

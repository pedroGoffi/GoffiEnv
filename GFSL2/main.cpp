#include <iostream>
#include <cassert>
#include "./src/gfsl.cpp"
#include "./src/typechecker.cpp"

int main(int argc, char** argv){
  assert(argc == 2);
  const char* const program   = argv[0];
  const char*       input_fp  = argv[1];
  const char*       stdout_fp = "out.asm";
  // TODO: check CI
  gfsl_vr vr = {};
  vr.program_size = 0;  
  gfsl_from_file(&vr, input_fp);
  gfsl_cross_reference(&vr);
  // typecheck_program(&vr);
  gfsl_compile_program(&vr, stdout_fp);
  compile_asm_to_binary(stdout_fp);
}

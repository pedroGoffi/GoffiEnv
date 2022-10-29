#include <iostream>
#include <cassert>
#include "./src/gfsl.cpp"
#include "./src/typechecker.cpp"
void compile_asm_to_binary(const char* fp){
  char buffer[258];
#define CMD(...)				\
  sprintf(buffer, __VA_ARGS__);			\
  printf("[CMD]: %s\n", buffer);		\
  system(buffer);
  
  const char* trash = "__TRASH__.o";
  CMD("nasm -felf64 %s -o %s", fp, trash);
  CMD("ld %s -o %s.bin", trash, fp);
  //CMD("rm %s", trash);
#undef CMD
}
int main(int argc, char** argv){
  assert(argc == 2);
  const char* const program   = argv[0];
  const char*       input_fp  = argv[1];
  const char*       stdout_fp = "out.asm";
  // TODO: check CI
  gfsl_vr vr = {};
  vr.program_size = 0;  
  gfsl_from_file(&vr, input_fp);
  gfsl_compile_program(&vr, stdout_fp);
  typecheck_program(&vr);
  compile_asm_to_binary(stdout_fp);
}

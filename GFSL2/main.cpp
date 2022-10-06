#include <iostream>
#include <cassert>
#include "./src/gfsl.cpp"

int main(int argc, char** argv){
  assert(argc == 2);
  const char* const program  = argv[0];
  const char*       input_fp = argv[1];
  // TODO: check CI
  gfsl_vr vr = {};
  vr.program_size = 0;  
  gfsl_from_file(&vr, input_fp);
  gfsl_simulate_program(&vr);
}

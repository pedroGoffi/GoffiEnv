#ifndef __c_compiler
#define __c_compiler
#include "./ast.cpp"
#include "./compiler.cpp"

void assembly_c_ast(Decl** ast, const char* outfp){
  fprintf(stderr,
	  "ERROR: gfl can not transpilte to c code yet\n");
  exit(1);
}
#endif /* __c_compiler */

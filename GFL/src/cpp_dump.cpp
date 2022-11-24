#ifndef __cpp_dump
#define __cpp_dump
#include "./ast.cpp"
FILE* fd = NULL;

void gen_cpp_file_from_ast(AST_ROOT ast, const char* output_fp)
{
  (void) ast;
  (void) output_fp;
  printf("ERROR: void gen_cpp_file_from_ast(AST_ROOT ast, const char* output_fp) is not implemented yet.\n");
  exit(1);
}
#endif /* __cpp_dump */

#ifndef _GFI_CPP
#define _GFI_CPP
#include <iostream>
#include <cassert>
#include "./backend/lexer/lexer.cpp"
#include "./backend/parser/parser.cpp"
#include "./backend/core/visitor.cpp"

int gfi_main(int argc, char** argv){
  const char* program  = shift(&argc, &argv);
  const char* input_fp = shift(&argc, &argv);
  assert(argc == 0);
  const char* str_code = lexer::get_file_text(input_fp);
  std::vector<Ast::Node*> ast = parse::vec_from_str(str_code);
  Visitor::run_code(ast);
  return 0;
}
#endif /*_GFI_CPP*/

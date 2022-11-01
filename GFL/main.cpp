// TODO: make cached types in ast
#ifndef __main
#define __main
#include <iostream>
#include <string>
#define LEXER_HAS_LOCATION        
#include "../common/utils.cpp"
#include "./src/parser.cpp"
#include "./src/ordering.cpp"
#include "./src/cpp_dump.cpp"
#include "./src/compiler.cpp"
#include "./src/assembler.cpp"
#include "../GFSL2/src/gfsl.cpp"
#include "../GFSL2/src/typechecker.cpp"

enum CO_OPT{
  NONE,
  DUMP_AST,  
  GENERATE_C_CODE,
  GENERATE_ASM_CODE,
  SIMULATE_GFM,
};

int main(int argc, char** argv){
  const char*  const program = shift(&argc, &argv);
  const char*  input_fp  = argv[0];
  const char*  output_fp = "out.asm";
  enum CO_OPT compiler_mode = CO_OPT::SIMULATE_GFM;
  bool debug_mode = false;
  // TODO: parse different args
  while(argc){    
    const char* flag = shift(&argc, &argv);
#define is_str(x, y) (strcmp(x, y) == 0)
    if(is_str(flag, "-h")){
      usage(stdout, program);
      return 0;
    }
    else if(is_str(flag, "-o")){
      if(argc < 1){
	usage(stderr, program);
	fprintf(stderr, "error: expected output file path fat `-o` flag\n");
	return 1;
      }
      output_fp = shift(&argc, &argv);
    } else if (is_str(flag, "--debug")){
      debug_mode = true;
    } else if (is_str(flag, "-gen:c")){
      compiler_mode = CO_OPT::GENERATE_C_CODE;
    } 
    else {
      input_fp = flag;
    }
#undef is_str
  }  
  const char* entry_content = get_file_text(input_fp);
  if(!entry_content){
    usage(stderr, program);
    printf("error: could not open the file: `%s`\n", input_fp);
    return 1;
  }
  
  AST_ROOT first_ast   = parser_run_code(entry_content);
  AST_ROOT ordered_ast = order_ast(first_ast);
  assembly_ast_into_gfsl(ordered_ast);
  
  // some clean-up
  buf__free(first_ast);

  print_ast(ordered_ast);

  gfsl_vr vr = {};
  gfsl_cross_reference(&vr);
  //TODO: in future typecheck_program(&vr);
  gfsl_compile_program(&vr, output_fp);
  compile_asm_to_binary(output_fp);
  (void) compiler_mode;
  (void) debug_mode;
  return 0;   
}
#endif /* __main */

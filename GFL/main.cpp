#ifndef __main
#define __main
#include <iostream>
#include <string>
#define LEXER_HAS_LOCATION        
#include "../common/utils.cpp"
#include "./src/parser.cpp"
#include "./src/resolve.cpp"


enum CO_OPT{
  NONE,
  DUMP_AST,  
  GENERATE_C_CODE,
  GENERATE_ASM_CODE,
};

auto main(int argc, char** argv) -> int {
  const char*  const program = shift(&argc, &argv);
  const char*  input_fp  = argv[0];
  const char*  output_fp = "out.cpp";
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

  AST_ROOT ast = parser_run_code(entry_content);
  // resolve will type check this stuff and do some automatic stuff
  resolve_ast(ast);
  

  print_ast(ast);
  printf("------------------\n");
  //gen_cpp_file_from_ast(ast, output_fp);

  
  return 0;
  
}
#endif /* __main */

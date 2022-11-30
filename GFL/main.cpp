// TODO: make cached types in ast
#ifndef __main
#define __main
#include <iostream>
#include <string>
const char*  program = NULL;
#define LEXER_HAS_LOCATION        
#include "../common/utils.cpp"
#include "./src/parser.cpp"
#include "./src/ordering.cpp"
#include "./src/compiler.cpp"
#include "./src/ASM_x86_64_compiler.cpp"
#include "./src/C_compiler.cpp"
#include "./src/3BC_compiler.cpp"

enum CO_OPT{
  NONE,
  DUMP_AST,  
  GENERATE_C_CODE,
  GENERATE_ASM_CODE,
  GENERATE_3BC_CODE
  // UNIMPLEMENTED: SIMULATE_GFM,
};
void CompileAsmToBinary(const char* outfp){
  char buff[258];
#define CMD(...)				\
  sprintf(buff, __VA_ARGS__);			\
  printf("[CMD]: %s\n", buff);			\
  system(buff);

  CMD("nasm -felf64 %s -o __TRASH__.o", outfp);
  CMD("ld __TRASH__.o -o %s.bin", outfp);
  CMD("rm __TRASH__.o");
#undef CMD
}

int main(int argc, char** argv){
  program = shift(&argc, &argv);
  const char*  input_fp  = argv[0];
  const char*  output_fp = "out.asm";
  enum CO_OPT  compiler_mode = CO_OPT::GENERATE_ASM_CODE;
  bool debug_mode = false;
  // TODO: parse different args
  while(argc){    
    const char* flag = shift(&argc, &argv);

    if(STR_CMP(flag, "-h")){
      usage(stdout, program);
      return 0;
    }
    else if(STR_CMP(flag, "-o")){
      if(argc < 1){
	usage(stderr, program);
	fprintf(stderr, "error: expected output file path fat `-o` flag\n");
	return 1;
      }
      output_fp = shift(&argc, &argv);

    } else if (STR_CMP(flag, "--debug")){
      debug_mode = true;
    } else if (STR_CMP(flag, "-gen:c")){
      compiler_mode = CO_OPT::GENERATE_C_CODE;
    } else if (STR_CMP(flag, "-gen:3bc")){
      compiler_mode = CO_OPT::GENERATE_3BC_CODE;
    }
    else {
      input_fp = flag;
    }

  }  
  
  
  // TODO: order_ast without bug
  Decl** ast = order_ast(parse_from_file(input_fp));
  switch(compiler_mode){
  case NONE:
  case DUMP_AST:
    print_ast(ast);
    return 0;
  case GENERATE_C_CODE:
    assembly_c_ast(ast, output_fp);
    return 0;
    
    
  case GENERATE_ASM_CODE:
    assembly_asmx86_64_ast(ast, output_fp);
    CompileAsmToBinary(output_fp);
    return 0;
  case GENERATE_3BC_CODE:
    assembly_3bc_ast(ast, output_fp);
    return 0;
  default:
    usage(stderr, program);
    fprintf(stderr,
	    "ERROR: gfl compiler_mode not specified.\n");
    exit(1);
    break;
  }
  // code must terminate inside the Switch
  // never come here down
  // if it does, its a bug
  fprintf(stderr,
	  "[INTERNAL-ERROR]: RIP reach forbidden code section.\n");
  return 1;   
}
#endif /* __main */

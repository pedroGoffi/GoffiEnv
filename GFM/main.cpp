#include <iostream>
#include "./src/machine.cpp"
#include "./src/errors.cpp"
#include "../common/utils.cpp"

#if 1
#define STD_AUTO_INST_LIMIT ARRAY_SIZE(mn.program)
#define STD_OUT_FP     "a.gfm"
enum Compiler_mode{
  undefined = 0,
  run_bytecode,
  compile_into_bytecode,
  compile_into_asmx86_64,
  translate_bytecode_to_ascii,
  decompiler
};
int main(int argc, char** argv){
  const char*  const program	= shift(&argc, &argv);
  const char*  input_fp		= nullptr;
  const char*  output_fp	= STD_OUT_FP;
  size_t inst_limit             = 0;
  auto compiler_mode            = Compiler_mode::undefined;
  bool debug_mode               = false;
  // TODO: parse different args
  while(argc){
    const char* flag = shift(&argc, &argv);

    if(strcmp(flag, "-h") == 0){
      usage(stdout, program);
      return 0;
    }     else if(strcmp(flag, "-o") == 0){
      if(argc < 1){
	usage(stderr, program);
	fprintf(stderr, "error: expected output file path fat `-o` flag\n");
	return 1;
      }
      output_fp = shift(&argc, &argv);
    } else if(strcmp(flag, "-c") == 0){
      compiler_mode = Compiler_mode::compile_into_bytecode;
    } else if(strcmp(flag, "-l") == 0){
      if(argc < 1){
	usage(stderr, program);
	fprintf(stderr, "error: expected int for the limit after `-l` flag\n");
	return 1;
      }
      inst_limit = atoi(shift(&argc, &argv));
    } else if (strcmp(flag, "--debug") == 0){
      debug_mode = true;
    } else if (strcmp(flag, "-b") == 0){
      compiler_mode = Compiler_mode::run_bytecode;
    } else if (strcmp(flag, "-t") == 0){
      compiler_mode = Compiler_mode::translate_bytecode_to_ascii;
    } else if (strcmp(flag, "-d") == 0){
      compiler_mode = Compiler_mode::decompiler;
    } else if (strcmp(flag, "--asm") == 0){
      compiler_mode = Compiler_mode::compile_into_asmx86_64;
    } else {
      input_fp = flag;
    }
  }
  if( input_fp == nullptr){
    usage(stderr, program);
    fprintf(stderr, "error: expected any input file path but got nothing.\n");
    return 1;
  }

  Machine mn      = {};
  mn.program_size = 1;
  mn.ip           = 0;
  Machine_push_all_natives(&mn);
  switch(compiler_mode){
  case Compiler_mode::compile_into_bytecode:
    Machine_slurp_code_from_file(&mn, input_fp);
    Machine_save_to_file(&mn, output_fp);
    return 0;
    break;
  case Compiler_mode::translate_bytecode_to_ascii:
    Machine_load_program_from_file(&mn, input_fp);
    Machine_dump_program(stdout, &mn);
    return 0;
    break;
  case Compiler_mode::decompiler: 
    Machine_load_program_from_file(&mn, input_fp);
    Machine_decompile_to_file(&mn, output_fp);
    return 0;
    break;
    
  case Compiler_mode::run_bytecode:
    Machine_load_program_from_file(&mn, input_fp);
    break;
  case Compiler_mode::compile_into_asmx86_64: {
    Machine_slurp_code_from_file(&mn, input_fp);
    Machine_compile_to_asmx86_64(&mn, output_fp);
    char buffer[256];
#define CMD(...)						\
    sprintf(buffer, __VA_ARGS__);				\
    printf("CMD: %s\n", buffer);					\
    system(buffer);
    //
    //CMD("rm %s", output_fp);
    CMD("nasm -felf64 %s -o a.o", output_fp);
    CMD("ld a.o -o %s", "g.bin");
    CMD("rm a.o");
#undef CMD
    return 0;
  } break;
  case Compiler_mode::undefined: 
  default:
    Machine_slurp_code_from_file(&mn, input_fp);
  }
  if( !inst_limit ) {
    inst_limit = STD_AUTO_INST_LIMIT + 1;
  }
      
  Machine_run(&mn, inst_limit, debug_mode);
}
#else
int main(){
  Buffer buffer = {};
  Buffer_write_cstr(&buffer, "hello");
  Buffer_write_cstr(&buffer, "world");
  Buffer_free(&buffer);
  
}
#endif

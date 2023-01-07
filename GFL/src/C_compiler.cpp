#ifndef __c_compiler
#define __c_compiler
#include "./ast.cpp"
#include "./compiler.cpp"



void c_decl(Decl* decl){
  switch(decl->kind){
  case DECL_PROC: {
    Proc proc = decl->as.procDecl;
    // TODO: print_type(proc.ret_type)
    print(" %s", proc.name);
    {
      print("(");
      size_t last_param = (size_t)buf__len(proc.args); 
      for(size_t i=0; i< last_param; ++i){
	Var* param = proc.args->vars[i];
	// c_typefield(param->type_field)
	print(i != last_param? ", ": "");	
      }
      print(")");
    }
    //c_block(proc.block);
    break;
  }
  default:
    fatal("Unexpected c_decl.\n");
    exit(1);
  }
}
void assembly_c_ast(Decl** ast, const char* outfp){
  output_file = fopen(outfp, "wb");
  assert(output_file);
  for(Decl* node = *ast; node != *buf__end(ast); node++){
    c_decl(node);
  }

  fclose(output_file);
}
#endif /* __c_compiler */

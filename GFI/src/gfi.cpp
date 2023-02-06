#ifndef __gfi_main
#define __gfi_main
#include "../../common/utils.cpp"
#include "./lexer.cpp"
#include "./parser.cpp"

int gfi_main(int argc, char** argv){
  const char* const program = shift(&argc, &argv);
  if(argc == 0){
    usage(stderr, program);
    fprintf(stderr, "ERROR: expected file path.\n");
    exit(1);
  }
  const char*  input_fp  = argv[0];

  while(argc){    
    const char* flag = shift(&argc, &argv);

    if(!strcmp(flag, "-h")){
      usage(stdout, program);
      return 0;
    }
    else {
      input_fp = flag;
    }

  }  

  const char* code_entry_string = get_file_text(input_fp);  
  std::vector<Ast::Node*> code = parse_string(code_entry_string);

  for(size_t i=0; i < code.size(); ++i){
    Ast::Node* node = code[i];
    switch(node->kind){
    case Ast::NodeKind::NODE_NUMBER:
    case Ast::NodeKind::NODE_STRING:
    case Ast::NodeKind::NODE_BINARY:
    case Ast::NodeKind::NODE_CALL:
      continue;
      
    case Ast::NodeKind::NODE_VAR: {
      Ast::Node* vexpr = Vars::GLOBAL_FIND(node->name);
      Ast::Node* resolved_expr = Visit::node(vexpr);
      *vexpr = *resolved_expr;
      break;
    }      
    case Ast::NodeKind::NODE_PROC:
      if(!strcmp(node->name, "main")){
	Visit::block(node->block);
      }
      break;
    case Ast::NodeKind::NODE_UNDEFINED:
    default:
      printf("Error: inesperado node->kind.\n");
      exit(1);
    }
    Visit::node(node);
  }
  return 1;
}

#endif /*__gfi_main*/

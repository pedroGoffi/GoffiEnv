#ifndef __resolve
#define __resolve
#include "./ast.cpp"
struct GLOBAL_VAR {};
GLOBAL_VAR** global_vars = NULL;

void resolve_ast(AST_ROOT ast){
  error_here("TODO: resolve_ast");
  for(size_t i=0; i<buf__len(ast); ++i){
    AST_NODE node = ast[i];
    switch(node->kind){
    case DECLKIND_VAR:
      
      error_here("some var");
      break;
    default:
      error_here("some stuff");
    }
    
  }
}

#endif /* __resolve */

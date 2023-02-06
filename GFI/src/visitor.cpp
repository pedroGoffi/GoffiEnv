#ifndef __visitor
#define __visitor
#include "./builtin.cpp"
#include "./ast.cpp"
#include "./table.cpp"
#include <string>

namespace Visit{
  Ast::Node* block(std::vector<Ast::Node*> block);
  
  Ast::Node* node(Ast::Node* node){
    assert(node->kind != Ast::NODE_UNDEFINED);
    
    if(node->kind == Ast::NODE_NUMBER){
      return node;
    }
    if(node->kind == Ast::NODE_STRING){
      return node;
    }
    else if (node->kind == Ast::NODE_VAR){
      return Visit::node(node->rhs);
    }
    else if (node->kind == Ast::NODE_BINARY){
      assert(node->lhs && node->rhs);
      Ast::Node* lhs = Visit::node(node->lhs);
      Ast::Node* rhs = Visit::node(node->rhs);
      assert(lhs->kind == Ast::NODE_NUMBER and rhs->kind == Ast::NODE_NUMBER);
      lhs->token.INT += rhs->token.INT;
      return lhs;
    }
    else if (node->kind == Ast::NODE_CALL){
      if(Builtin::is_proc(node->name)){
	return Builtin::Make_call(node);
      }
      else if (Ast::Node* proc = Procs::PROCS_FIND(node->name)){
        return Visit::block(proc->block);
      }
      printf("Error: o procedimento '%s', não foi declarado neste escopo.\n",
	     node->name);
      exit(1);
    }
    else if (node->kind == Ast::NODE_PROC) {
      return Ast::Node_builtin_int(0);
    }
    printf("visitor error %i:\n",
	   (int)node->kind);
    return node;    
  }
  Ast::Node* block(std::vector<Ast::Node*> block){
    Ast::Node* ret;
    for(size_t i=0; i < block.size(); ++i){
      ret = Visit::node(block[i]);
    }
    return ret;
  }
}
#endif /*__visitor*/

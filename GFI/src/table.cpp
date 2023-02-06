#ifndef __table
#define __table
#include <vector>
#include "./ast.cpp"
#include "./visitor.cpp"

namespace Vars {
  static std::vector<Ast::Node*> GLOBAL_VARS = {};
  Ast::Node* GLOBAL_FIND(const char*);
  void GLOBAL_PUSH(Ast::Node*);


  void GLOBAL_PUSH(Ast::Node* var){
    GLOBAL_VARS.push_back(var);
  }
  Ast::Node* GLOBAL_FIND(const char* name){
    LOOK_UP_TABLE(GLOBAL_VARS, name, name, NULL);
  }
}

namespace Procs {
  static std::vector<Ast::Node*> PROCS = {};
  
  void PROCS_PUSH(Ast::Node*);
  Ast::Node* PROCS_FIND(const char*);

  void PROCS_PUSH(Ast::Node* proc){
    PROCS.push_back(proc);
  }

  Ast::Node* PROCS_FIND(const char* name){
    LOOK_UP_TABLE(PROCS, name, name, NULL);    
  }
}
#endif /*__table*/

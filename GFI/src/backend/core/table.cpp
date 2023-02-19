#ifndef __table_CPP
#define __table_CPP
#include <cstring>
#include <cassert>
#include "../ast/ast.cpp"

namespace Table {
  namespace Vars {
    std::vector<Ast::Node*> global_vars = {};
    std::vector<Ast::Node*> local_vars = {};
    
    Ast::Node* get(std::vector<Ast::Node*> var_table, const char* name){
      for(size_t i=0; i < var_table.size(); ++i){
	if(!strcmp(var_table[i]->name, name)){	  
	  return *&((var_table)[i]);
	}      
      }
      return NULL;
    }
    void push(std::vector<Ast::Node*>* var_table, Ast::Node* var){
      assert(!get(*var_table, var->name));
      var_table->push_back(var);
    }
  }
//namespace Procs {
//  struct Proc {
//    const char* name;
//    std::vector<Ast::Node*>* args;
//    std::vector<Ast::Node*>  block;      
//  };
//  std::vector<Proc*> procs = {};
//  
//  Proc* get(std::vector<Proc*> table, const char* name){
//    for(size_t i=0 ; i< table.size(); ++i){
//	if(!strcmp(table[i]->name, name)){
//	  return table[i];
//	}
//    }
//    return NULL;
//  }
//  void push(std::vector<Proc*>* table, Proc* proc){
//    assert(!Table::Procs::get(*table, proc->name));
//    table->push_back(proc);
//  }
//}
}
#endif /*__table_CPP*/

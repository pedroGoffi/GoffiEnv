#ifndef _visitor
#define _visitor
#include <vector>
#include <string>
#include "../ast/ast.cpp"
#include "./table.cpp"
typedef Ast::Node* (*gfi_builtin_call)(Ast::Node*);

using Table::Vars::get;
Ast::Node* ty_string = new Ast::Node {.name = "string"};
Ast::Node* ty_int    = new Ast::Node {.name = "int"};

namespace Visitor {
  bool on_proc = false;
  Ast::Node* run_code(std::vector<Ast::Node*> code_vec); 
  Ast::Node* run_node(Ast::Node* node);
}
namespace Builtin {
  Ast::Node* GFI_WRITE(Ast::Node* args);
  Ast::Node* GFI_TO_STRING(Ast::Node* args);
  Ast::Node* GFI_EXIT(Ast::Node* args);
  Ast::Node* GFI_INPUT(Ast::Node* args);
  

  gfi_builtin_call* is_func(const char* name);

  
  std::pair<const char*, gfi_builtin_call> builtin_table[] = {
    {"write", GFI_WRITE},
    {"str",   GFI_TO_STRING},
    {"exit",  GFI_EXIT},
    {"input", GFI_INPUT}
  };
}
Ast::Node* Builtin::GFI_EXIT(Ast::Node* node){
  (void) node;
  printf("ERR: exit is not fully implemented yet.\n");  
  exit(0);
}
Ast::Node* Builtin::GFI_TO_STRING(Ast::Node* node){ 
  std::vector<Ast::Node*> args = node->block;
  if(args.size() != 1) {
    printf("Error: builtin 'str' espera 1 argumento.\n");
    exit(1);
  }
  // TODO: filter the token to -> string
  if(node->token.kind == token::TOKEN_STRING) return node;
  
  Ast::Node* arg = Visitor::run_node(args[0]);
  const char* str = std::to_string(arg->token.INT).c_str();
  token::Token tk = {
    .kind   = token::TokenKind::TOKEN_STRING,
    .name   = "<builtin-string>",
    .STRING = str
  };
  args[0]->token = tk;
  return args[0];
}
Ast::Node* Builtin::GFI_INPUT(Ast::Node* args){
  std::string str;
  std::cin >> str;
  return Ast::Node_builtin_str(str.c_str());
}
Ast::Node* Builtin::GFI_WRITE(Ast::Node* node){  
  std::vector<Ast::Node*> args = node->block;
  for(size_t i=0 ; i < args.size(); ++i){
    Ast::Node* visited_node = Visitor::run_node(args[i]);
    printf("%s", visited_node->token.STRING);
    
  }
  return Ast::Node_builtin_int(0);
}

gfi_builtin_call* Builtin::is_func(const char* name){
  for(auto& bi_item : Builtin::builtin_table){
    if(!strcmp(bi_item.first, name)){
      return &bi_item.second;
    }
  }
  return NULL;
} 
Ast::Node* Visitor::run_node(Ast::Node* node){
  assert(node->kind != Ast::NodeKind::NODE_UNDEFINED);
  switch(node->kind){
  case Ast::NodeKind::NODE_NUMBER: 
  case Ast::NodeKind::NODE_STRING:
    return node;

  case Ast::NodeKind::NODE_BINARY:{
    Ast::Node* lhs = Visitor::run_node(node->lhs);
    Ast::Node* rhs = Visitor::run_node(node->rhs);
    switch((char)node->token.kind){
    case '+':
      lhs->token.INT += rhs->token.INT;
      break;
    case '-':
      lhs->token.INT -= rhs->token.INT;
      break;
    case '*':
      lhs->token.INT *= rhs->token.INT;
      break;
    case '/':
      lhs->token.INT /= rhs->token.INT;
      break;
    default:
      printf("Error: expressão binaria '%c', não definida.\n",
	     node->token.kind);
      exit(1);
    }    
    return lhs;
  }

  case Ast::NodeKind::NODE_DOT_GET: {
    printf("AVISO! NAO IMPLEMENTEADO DOT_GET.\n");
    assert(node->name);
    // for now only global vars
    {
      using namespace Table;
      Ast::Node* global_var = Vars::get(Vars::global_vars,

					node->name);
      
	return global_var->get_property(node->rhs->name);      
    }
    exit(1);
  };
  case Ast::NodeKind::NODE_CALL: {
    Visitor::on_proc = true;
    Ast::Node* ret;
    if( gfi_builtin_call* bi_call = Builtin::is_func(node->name)){
      
      ret = (**bi_call)(node);
    }
    //else if (auto* proc = Table::Procs::get(Table::Procs::procs, node->name)){
    //  // save last local vars
    //  // use call local vars      
    //  Ast::Node* ret = Visitor::run_code(proc->block);
    //  // restore last local vars
    //  return ret;
    //}
    else {
      printf("Error: o procedimento '%s' nao foi declarado neste escopo.\n",
	     node->name);
      exit(1);
    }
    Visitor::on_proc = false;
    return ret;
  }
  case Ast::NodeKind::NODE_PROC: {
     if(!strcmp(node->name, "main")){
      Visitor::on_proc = true;
      Ast::Node* ret = Visitor::run_code(node->block);
      Visitor::on_proc = false;
      return ret;
    }
    else {
      // for now ignore diferents procedures
      return node;
    }
  } break;
  case Ast::NodeKind::NODE_VAR: {
    assert(node->name);
    using namespace Table;
   		       
    // check if var is already declared
    // then if yes reasign to node->rhs
    
    Ast::Node* resolved_expr = Visitor::run_node(node->rhs);
    
    if(auto* var = Vars::get(Vars::global_vars, node->name)){
      // variable already exist   
      *var->rhs = *resolved_expr;      
      return var->rhs;
    }
    
    // if not then declare it 
    if(Visitor::on_proc){
      // local variables
      printf("Error: local vars is not implemented yet.\n");
      exit(1);
    }
    else {
      // global vars
      *node->rhs = *resolved_expr;
      node->set_property("type",
			 Ast::Node_name(node->type->name));         
      Vars::push(&Vars::global_vars, node);
      return node;            
    }
    printf("node VAR: %s\n", node->name);
    exit(1);

  } break;
  case Ast::NodeKind::NODE_RETURN: return Visitor::run_node(node->rhs);
  case Ast::NodeKind::NODE_NAME: {
    using namespace Table;
    if(auto* var = Vars::get(Vars::global_vars, node->name)){
      return var->rhs;  
    }
    
    printf("node name NODE_NAME : %s\n", node->name);
    return node;
  }
  default:
    printf("ERROR: run_code inesperado node->kind.\n");
    exit(1);
  }
  printf("Error: run_node .\n");
  exit(1);
}

Ast::Node* Visitor::run_code(std::vector<Ast::Node*> code_vec){
  Ast::Node* ret;
  for(size_t i=0; i < code_vec.size(); ++i){    
    Ast::Node* node = code_vec[i];
    if(node->kind == Ast::NodeKind::NODE_RETURN){
      return Visitor::run_node(node);
    }
    ret = Visitor::run_node(node);
  }
  return ret;
}


#endif /*_visitor*/

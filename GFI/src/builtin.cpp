#ifndef __builtin
#define __builtin
#include <iostream>
#include <string>
#include "./ast.cpp"

#define GFI_WRITE "write"
#define GFI_INPUT "input"
typedef int (*gfi_io_write)(const char*, ...);
typedef std::string (*gfi_io_input) ();

std::string gfi_custom_input(){
  std::string var;
  std::cin >> var;
  return var;
}

namespace Builtin {
  const char* tktstr(Token tk);
  
  gfi_io_write write = printf;
  gfi_io_input input = gfi_custom_input;
  
  static const char* builtin_names[] = {
    GFI_WRITE, GFI_INPUT
  };
  
  bool is_proc(const char* name){
    for(const char* bn: builtin_names){
      if(!strcmp(bn, name)) return true;
    }
    return false;
  }
  Ast::Node* Make_call(Ast::Node* node){

    if(!strcmp(node->name, GFI_WRITE)){
      std::vector<Ast::Node*> args = node->block;
      for(size_t i=0; i < args.size(); ++i){
	Token argtk = args[i]->token;
	const char* arg_str = argtk.STRING;
	(*Builtin::write)(arg_str);
      }
    }
    if(!strcmp(node->name, GFI_INPUT)){
      printf("ok\n");
      std::vector<Ast::Node*> args = node->block;
      for(size_t i=0; i < args.size(); ++i){
	Token argtk = args[i]->token;
	const char* arg_str = Builtin::tktstr(argtk);
	(*Builtin::write)(arg_str);
      }
      std::string str;
      std::cin >> str;

      Token str_tk = {
	.kind = TOKEN_STRING,
	.modifier = {},
	.STRING   = str.c_str()
      };
      return Ast::Node_string(str_tk);
    }
    return Ast::Node_builtin_int(0);
  }
  const char* tktstr(Token tk){
    if(tk.kind == TOKEN_STRING) return tk.STRING;
    if(tk.kind == TOKEN_INT){
      return std::to_string(tk.INT).c_str();
    }
    printf("Error: tktstr could not stringfy the token.\n");
    exit(1);
  }
}
#endif /*__builtin*/

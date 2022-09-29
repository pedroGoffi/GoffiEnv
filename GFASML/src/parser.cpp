#ifndef   GFASM_PARSER
#define   GFASM_PARSER
#include <iostream>
#include <map>
#include "./ast.cpp"
#include "./lexer.cpp"
#include "../../common/utils.cpp"

static std::map<const char*, Instruction_kind> ins_kind_map = {  
  {"noop", Instruction_kind::NOOP},
  {"push", Instruction_kind::PUSH}
};
Instruction* parse_instruction();
Procedure_block* parse_procedure_block();
Node_procedure* parse_node_procedure();
File_node* parse_file_nodes();
File_node** parse_module();


Instruction* parse_instruction(){
  Instruction* ins = new Instruction;
  assert(token.kind == TOKEN_NAME);
  if(!find_map(ins_kind_map, token.name)){
    error_here("unexpected instruction `%s` -- %zu.\n", token.name,
	       (size_t)ins_kind_map[token.name]);
  }
  ins->instruction_kind = ins_kind_map[token.name];
  return ins;
}

Procedure_block* parse_procedure_block(){
  Procedure_block* pb   = new Procedure_block;
  Instruction**    body = nullptr;
  pb->stack_allocation_size = 0;
  while(!token_is_name("end")){
    buf__push(body, parse_instruction());
  }
  pb->instructions = body;
  return pb;
}
Node_procedure* parse_node_procedure(){
  Node_procedure* np = new Node_procedure;
  assert(token.kind == TOKEN_NAME);
  np->name = consume().name;
  
  if(expect_name("do")){
    np->procedure_block = parse_procedure_block();
    expect_name("end");
  }
  else {
    error_here("expected `do` after procedure definition.\n");
  }
  return np;
}
File_node* parse_file_nodes(){
  File_node* fn = new File_node;
  if(expect_name("proc")){
    fn->kind = File_node_kind::node_procedure;
    fn->node_procedure = parse_node_procedure();
  }
  error_here("could not parse the file node.\n");
}
void init_ins_kind_map(){
  ins_kind_map["push"]  = Instruction_kind::PUSH;
}
File_node** parse_module(){ 
  File_node** fn = NULL;
  init_ins_kind_map();
  while(*stream){
    buf__push(fn, parse_file_nodes());
  }
  return fn;
}
#endif /* GFASM_PARSER */

#ifndef __parser
#define __parser
#include <iostream>
#include "./lexer.cpp"
#include "./token.cpp"
#include "./ast.cpp"
#include "./visitor.cpp"
#include "./table.cpp"
#include "./grammar.cpp"

namespace parse {
  Ast::Node* atom();   // parse numbers 
  Ast::Node* unary();  // operand?(atom)
  Ast::Node* mult();   // [* or /](unary)
  Ast::Node* binary(); // [+ or -](mult) 
  Ast::Node* expr();
  Ast::Node* func();
  std::vector<Ast::Node*> block();


  Ast::Node* atom(){    
    if (is_token(TOKEN_INT)){
      return Ast::Node_num(consume());     
    }
    else if (is_token(TOKEN_STRING)){
      return Ast::Node_string(consume());
    }
    else if(is_token(TOKEN_NAME)){
      Token tk = consume();
      // chamada de procedimento 
      if (expect_token(TOKEN_OPEN_R_PAREN)){
	std::vector<Ast::Node*> args;
	while(!is_token(TOKEN_CLOSE_R_PAREN)){
	  if(args.size() > 0){
	    MustExpect(TOKEN_COMMA);
	  }
	  Ast::Node* arg = parse::expr();
	  args.push_back(arg);
	}
	assert(expect_token(TOKEN_CLOSE_R_PAREN));
	return Ast::Node_call(tk.name, args);
      }
      // definição de procedimentos ou estruturas
      else if (expect_token(TOKEN_ACCESS_FIELD)){
	if(token_is_name(proc_kw)){
	  Ast::Node* proc = parse::func();
	  proc->name = tk.name;
	  return proc;
	}
	else {
	  printf("Erro Sintatico: definição pré-processada não compreendida, apos operador '::' para a palavra %s.\n",
		 tk.name);
	  
	  exit(1);
	}
      }
         
      else if (is_token(TOKEN_DOUBLE_DOT) or is_token(TOKEN_EQ)){
	// var def
	Ast::Node* var = new Ast::Node();
	var->kind = Ast::NODE_VAR;
	var->set_token(tk); // token name + position
	var->name = var->token.name;
	
	if(expect_token(TOKEN_DOUBLE_DOT)){
	  printf(ERR_01);
	  exit(1);
	}
    
	if(expect_token(TOKEN_EQ)){
	  var->set_rhs(parse::expr());	  
	}
	
	else if (!is_token(TOKEN_DOT_AND_COMMA) and !is_token(TOKEN_EOF)){
	  printf(ERR_02);
	  //exit(1);
	}
    
	Vars::GLOBAL_PUSH(var);
	return var;
      }
      else if (Ast::Node* var = Vars::GLOBAL_FIND(tk.name)){
	return var->rhs;
      }

      
    }    
    printf("Error sintatico ao tentar parsear uma expressao: '%s'\n", human_readable_token(token));
    exit(1);
  }
  Ast::Node* unary(){
    //if(is_token(TOKEN_LESS)){
    //  next_token();
    //  return (parse::atom());
    //}
    return parse::atom();
  }
  Ast::Node* mult(){
    Ast::Node* lhs = parse::unary();
    if (is_token(TOKEN_STAR) or is_token(TOKEN_DIV)){
      Token token = consume();
      Ast::Node* rhs = parse::unary();

      lhs = Ast::Node_bin(token, lhs, rhs);
    }
    return lhs;
  }
  Ast::Node* binary(){    
    Ast::Node* lhs  = parse::mult();    
    if (is_token(TOKEN_PLUS) or is_token(TOKEN_LESS)){
    
      Token token = consume();
      Ast::Node* rhs = parse::mult();
      
      lhs = Ast::Node_bin(token, lhs, rhs);
    }
    return lhs;
  }

  Ast::Node* expr(){
    // TODO: differend kinds of expr
    if(0){

    }
    else {
      return parse::binary();
    }
  }
  std::vector<Ast::Node*> block(){
    assert(consume().kind == TOKEN_OPEN_C_PAREN);
    std::vector<Ast::Node*> block;
    while(!is_token(TOKEN_CLOSE_C_PAREN)){
      Ast::Node* line = parse::expr();
      block.push_back(line);
    }
    assert(consume().kind == TOKEN_CLOSE_C_PAREN);
    return block;
  }
  Ast::Node* func(){
    assert(token_is_name(proc_kw));
    next_token();
    MustExpect(TOKEN_OPEN_R_PAREN);
    Ast::Node* func = new Ast::Node();
    func->kind = Ast::NODE_PROC;
    while(!is_token(TOKEN_CLOSE_R_PAREN)){
      printf(DERR_01);
      exit(1);
    }
    MustExpect(TOKEN_CLOSE_R_PAREN);
    func->block = parse::block();
    return func;
  }
}

std::vector<Ast::Node*> parse_string(const char* string)
{
  std::vector<Ast::Node*> code;  
  init_stream(string);
  while(!is_token(TOKEN_EOF)){   
    Ast::Node* node = parse::expr();
    if(node->kind == Ast::NODE_PROC)
      Procs::PROCS_PUSH(node);
    
    code.push_back(node);
  }
  return code; 
}
#endif /*__parser*/

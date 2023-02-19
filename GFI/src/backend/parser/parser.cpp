#ifndef __parser
#define __parser
#include <iostream>
#include "../lexer/lexer.cpp"
#include "../ast/ast.cpp"
#include "../../frontend/grammar.cpp"


namespace parse {  
  Ast::Node* atom();   // parse numbers 
  Ast::Node* unary();  // operand?(atom)
  Ast::Node* mult();   // [* or /](unary)
  Ast::Node* binary(); // [+ or -](mult) 
  Ast::Node* expr();
  Ast::Node* func();
  Ast::Node* var(token::Token*);
  
  std::vector<Ast::Node*> block();


  Ast::Node* atom(){    
    if (lexer::is_token(token::TOKEN_INT)){
      return Ast::Node_num(lexer::consume());     
    }
    else if (lexer::is_token(token::TOKEN_STRING)){
      return Ast::Node_string(lexer::consume());
    }
    else if(lexer::is_token(token::TOKEN_NAME)){
      token::Token tk = lexer::consume();
      // chamada de procedimento
      if(lexer::is_token(token::TOKEN_DOT)){
	Ast::Node* root = new Ast::Node{
	  .kind = Ast::NODE_DOT_GET,
	  .name = tk.name,
	  .token = tk
	};

	lexer::next_token();
	printf("get");
	root->rhs = parse::atom();
        return root;
      }
      else if (lexer::expect_token(token::TOKEN_OPEN_R_PAREN)){
	std::vector<Ast::Node*> args;
	while(!lexer::is_token(token::TOKEN_CLOSE_R_PAREN)){
	  if(args.size() > 0){
	    lexer::MustExpect(token::TOKEN_COMMA);
	  }
	  Ast::Node* arg = parse::expr();
	  args.push_back(arg);
	}
	assert(lexer::expect_token(token::TOKEN_CLOSE_R_PAREN));
	return Ast::Node_call(tk, args);
      }
      // definição de procedimentos ou estruturas
      else if (lexer::expect_token(token::TOKEN_ACCESS_FIELD)){
	if(lexer::token_is_name(proc_kw)){
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
         
      else if (lexer::is_token(token::TOKEN_DOUBLE_DOT) or lexer::is_token(token::TOKEN_EQ)){
	
	return parse::var(&tk);
      }
      return Ast::Node_name(tk.name);
    }
    else if (lexer::expect_token(token::TOKEN_OPEN_R_PAREN)){
      Ast::Node* nd = parse::expr();
      lexer::MustExpect(token::TOKEN_CLOSE_R_PAREN);
      return nd;
    }
    printf("Error sintatico ao tentar parsear uma expressao: '%s'\n", token::human_readable_token(token::token));
    exit(1);
  }
  Ast::Node* unary(){
    //if(lexer::is_token(token::TOKEN_TAKEAWAY)){
    //  lexer::next_token();
    //  return (parse::atom());
    //}
    return parse::atom();
  }
  bool is_mult(){
    return
      lexer::is_token(token::TOKEN_STAR) or
      lexer::is_token(token::TOKEN_DIV);
  }
  Ast::Node* mult(){
    Ast::Node* lhs = parse::unary();
    while(parse::is_mult()){
      token::Token token = lexer::consume();
      Ast::Node* rhs = parse::unary();

      lhs = Ast::Node_bin(token, lhs, rhs);
    }
    return lhs;
  }
  bool is_binary() {
    return
      lexer::is_token(token::TOKEN_PLUS) or
      lexer::is_token(token::TOKEN_TAKEAWAY);
  }
  Ast::Node* binary(){    
    Ast::Node* lhs  = parse::mult();    
    while (parse::is_binary()){
      token::Token token = lexer::consume();
      Ast::Node* rhs = parse::mult();
      
      lhs = Ast::Node_bin(token, lhs, rhs);
    }
    return lhs;
  }

  Ast::Node* expr(){
    // TODO: differend kinds of expr
    if(lexer::token_is_name("return")){
      lexer::next_token();
      Ast::Node* nd = parse::expr();
      return Ast::Node_ret(nd);
    }
    else {
      return parse::binary();
    }
  }
  std::vector<Ast::Node*> block(){
    assert(lexer::consume().kind == token::TOKEN_OPEN_C_PAREN);
    std::vector<Ast::Node*> block;
    while(!lexer::is_token(token::TOKEN_CLOSE_C_PAREN)){
      Ast::Node* line = parse::expr();
      block.push_back(line);
    }
    assert(lexer::consume().kind == token::TOKEN_CLOSE_C_PAREN);
    return block;
  }
  Ast::Node* func(){
    assert(lexer::token_is_name(proc_kw));
    lexer::next_token();
    lexer::MustExpect(token::TOKEN_OPEN_R_PAREN);
    Ast::Node* func = new Ast::Node();
    func->kind = Ast::NODE_PROC;
    while(!lexer::is_token(token::TOKEN_CLOSE_R_PAREN)){
      Ast::Node* arg = parse::var(NULL);
      func->args.push_back(arg);
    }
    lexer::MustExpect(token::TOKEN_CLOSE_R_PAREN);
    func->block = parse::block();
    return func;
  }
  Ast::Node* var(token::Token* var_tk = NULL){
    Ast::Node* var = new Ast::Node();	
    var->kind = Ast::NODE_VAR;
    token::Token tk =  (var_tk)? *var_tk : lexer::consume();
    var->set_token(tk); // token name + position
    var->name = var->token.name;

    Ast::Node* ty = Ast::Type_undefined(tk);
    
    if(lexer::expect_token(token::TOKEN_DOUBLE_DOT)){   
      ty = Ast::Type_name(token::token.name);
      lexer::next_token();
	
    }
    var->type = new Ast::Node;
    *var->type = *ty;

    if(lexer::expect_token(token::TOKEN_EQ)){
      var->set_rhs(parse::expr());	  
    }
    
    return var;
  }
  std::vector<Ast::Node*> vec_from_str(const char* string){
    std::vector<Ast::Node*> code;  
    lexer::init_stream(string);
    while(!lexer::is_token(token::TOKEN_EOF)){   
      Ast::Node* node = parse::expr();
      code.push_back(node);
    }
    return code; 
  }
}


#endif /*__parser*/

#ifndef __ast
#define __ast
#include <vector>
#include <map>
#include "../lexer/token.cpp"

namespace Ast {  
  enum NodeKind{
    NODE_UNDEFINED,
    NODE_TYPE,
    NODE_DOT_GET,
    NODE_NUMBER,
    NODE_STRING,
    NODE_NAME,
    NODE_RETURN,
    NODE_BINARY,
    NODE_VAR,
    NODE_CALL,
    NODE_PROC
  };
  class Node{
  public:
    NodeKind    kind;
    const char* name;
    token::Token       token;
    Node*       type;
    Node*       lhs;
    Node*       rhs;
    std::map<const char*, Ast::Node*> properties;
    std::vector<Node*> block;
    std::vector<Node*> args;
    
    
    void set_token(token::Token token);
    void set_kind(NodeKind);
    void set_lhs(Node* lhs);    
    void set_rhs(Node* rhs);
    void set_binary(token::Token, Node*, Node*);
    // User Utils
    const char* human_kind();
    Node* get_property(const char*);
    void  set_property(const char*, Node*);
  };

  void Node::set_kind(NodeKind kind){
    this->kind = kind;
  }
  void Node::set_token(token::Token token){
    this->token = token;
  }
  void Node::set_lhs(Node* lhs){
    this->lhs = lhs;
  }
  void Node::set_rhs(Node* rhs){
    this->rhs = rhs;
  }
  void Node::set_binary(token::Token token, Node* lhs, Node* rhs){
    this->token = token;
    this->kind  = NODE_BINARY;
    this->lhs   = lhs;
    this->rhs   = rhs;
  }
  Node* Node::get_property(const char* str){
    if(find_map(this->properties, str))
      return this->properties[str];
    return NULL;
  }
  void  Node::set_property(const char* str, Node* node){
    this->properties[str] = node;
  }
  const char* Node::human_kind(){
    switch(this->kind){
    case NODE_NUMBER:   return "Number";
    case NODE_STRING:	return "String";
    case NODE_BINARY:	return "Binary operation";
    case NODE_VAR:	return "Variable";
    case NODE_CALL:	return "Call";
    case NODE_PROC:	return "Procedure";
    default: return "Undefined";
    }
  }
  Node* Node_ret(Node* node){
    Node* nd	= new Node();
    nd->kind	= NODE_RETURN;
    nd->name	= "<builtin-return>";
    nd->rhs	= node;
    return nd;          
  }
  Node* Node_builtin_int(int val){
    Node* nd = new Node();;
    nd->kind = NODE_NUMBER;
    nd->token = {
      .kind     = token::TOKEN_INT,
      .name     = "<builtin-int>",
      .INT      = val
    };
    return nd;
  }
  Node* Node_builtin_str(const char* str){
    Node* nd = new Node();
    nd->kind = NODE_STRING;
    nd->token = token::Token{
      .kind     = token::TOKEN_STRING,
      .name     = "<builtin-string>",
      .STRING   = str
    };
    return nd;
  }
  Node* Node_name(const char* name){
    Node* nd  = new Node();
    nd->kind  = NODE_NAME;
    nd->name  = name;
    return nd;
  }
  Node* Node_num(token::Token token){
    Node* nd  = new Node();
    nd->kind  = NODE_NUMBER;
    nd->token = token;
    return nd;
  }
  Node* Node_bin(token::Token token, Node* lhs, Node* rhs){
    Node* nd = new Node();
    nd->set_binary(token, lhs, rhs);
    return nd;
  }
  Node* Node_string(token::Token token){
    Node* nd = new Node();
    nd->kind = NODE_STRING;
    nd->set_token(token);
    return nd;
  }
  Node* Node_call(token::Token tk, std::vector<Node*> args){
    Node* nd = new Node();
    nd->kind = NODE_CALL;
    nd->token = tk;
    nd->name = tk.name;
    nd->block = args;
    return nd;
  }
  Node* Type_undefined(token::Token tk){
    Node* nd	= new Node();
    nd->kind	= NODE_TYPE;
    nd->token	= tk;
    return nd;
  }
  Node* Type_name(const char* nm){
    Node* nd	= new Node();
    nd->kind	= NODE_TYPE;
    nd->name    = nm;
    nd->token	= token::Token {
      .kind = token::TOKEN_NAME,
      .name = nm,
      .INT  = 0
    };
    
    return nd;
  }
}
#endif /*__ast*/

  

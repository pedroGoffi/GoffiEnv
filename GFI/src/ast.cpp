#ifndef __ast
#define __ast
#include "./token.cpp"
#include <vector>

namespace Ast {
  enum NodeKind{
    NODE_UNDEFINED,
    NODE_NUMBER,
    NODE_STRING,
    NODE_BINARY,
    NODE_VAR,
    NODE_CALL,
    NODE_PROC
  };
  class Node{
  public:
    NodeKind    kind;
    const char* name;
    Token       token;  
    Node*       lhs;
    Node*       rhs;
    std::vector<Node*> block;
    
    Node(NodeKind);
    void set_token(Token token);
    void set_kind(NodeKind);
    void set_lhs(Node* lhs);    
    void set_rhs(Node* rhs);
    void set_binary(Token, Node*, Node*);
    // User Utils
    const char* human_kind();
    
  };

  Node::Node(NodeKind kind = NODE_UNDEFINED){
    this->set_kind(kind);
  }
  void Node::set_kind(NodeKind){
    this->kind = kind;
  }
  void Node::set_token(Token token){
    this->token = token;
  }
  void Node::set_lhs(Node* lhs){
    this->lhs = lhs;
  }
  void Node::set_rhs(Node* rhs){
    this->rhs = rhs;
  }
  void Node::set_binary(Token token, Node* lhs, Node* rhs){
    this->token = token;
    this->kind  = NODE_BINARY;
    this->lhs   = lhs;
    this->rhs   = rhs;
  }
  const char* Node::human_kind(){
    switch(this->kind){
    case NodeKind::NODE_NUMBER: return "Number";
    case NodeKind::NODE_STRING:	return "String";
    case NodeKind::NODE_BINARY:	return "Binary operation";
    case NodeKind::NODE_VAR:	return "Variable";
    case NodeKind::NODE_CALL:	return "Call";
    case NodeKind::NODE_PROC:	return "Procedure";
    default: return "Undefined";
    }
  }
  Node* Node_builtin_int(int val){
    Node* nd = new Node();;
    nd->kind = NODE_NUMBER;
    nd->token = {
      .kind     = TOKEN_INT,
      .modifier = {},
      .name     = "<builtin-int>",
      .INT      = val
    };
    return nd;
  }
  Node* Node_num(Token token){
    Node* nd  = new Node();
    nd->kind  = NODE_NUMBER;
    nd->token = token;
    return nd;
  }
  Node* Node_bin(Token token, Node* lhs, Node* rhs){
    Node* nd = new Node();
    nd->set_binary(token, lhs, rhs);
    return nd;
  }
  Node* Node_string(Token token){
    Node* nd = new Node();
    nd->kind = NODE_STRING;
    nd->set_token(token);
    return nd;
  }
  Node* Node_call(const char* call_name, std::vector<Node*> args){
    Node* nd = new Node();
    nd->kind = NODE_CALL;
    nd->name = call_name;
    nd->block = args;
    return nd;
  }
}
#endif /*__ast*/
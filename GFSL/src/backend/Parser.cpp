
#ifndef PARSER
#define PARSER value

#include <string>
#include <iostream>
#include <vector>
#include "./Lexer.cpp"
#include "./goffi.cpp"
#include "../frontend/Tools.cpp"
#include <typeinfo>
#define stoi64(x) (static_cast<uint64>(std::stoull(x)))

std::string current_proc;

typedef enum{
  ID_NOOP,
  ID_DUMP_STACK,
  ID_STRUCT_ACCESS,
  ID_PUSH_LOCAL_MEM,
  ID_ALLOCATE_LOCAL_MEM,
  ID_PROC_ENTRY,
  ID_PROC_RETURN,
  ID_SKIP_PROC,
  ID_PROC_LEAVE,
  ID_CALL_PROC,
  ID_INTRISIC_DUMP,
  ID_INTRISIC_DUP,
  ID_INTRISIC_EQUALS,
  ID_INTRISIC_NOT_EQ,
  ID_INTRISIC_SWAP,
  ID_INTRISIC_OVER,
  ID_INTRISIC_ROT,
  ID_INTRISIC_DROP,
  ID_INTRISIC_DIVMOD,
  ID_INTRISIC_IDIVMOD,
  ID_INTRISIC_SHIFT_LEFT,
  ID_INTRISIC_SHIFT_RIGHT,
  ID_INTRISIC_OR,

  ID_INTRISIC_AND,
  ID_INTRISIC_NOT,
  ID_INTRISIC_ARGV,
  ID_INTRISIC_ARGC,


  ID_WHILE,
  ID_IF,
  ID_DO,
  ID_ELIF,
  ID_ELSE,
  ID_END,
  ID_MEM,
  ID_HERE,
  ID_SYSCALL_0,
  ID_SYSCALL_1,
  ID_SYSCALL_2,
  ID_SYSCALL_3,
  ID_SYSCALL_4,
  ID_SYSCALL_5,
  ID_SYSCALL_6,

  ID_LOAD8,
  ID_LOAD64,
  ID_STORE8,
  ID_STORE64,
  ID_STRING,
  ID_CAST,
  ID_STATIC_MEM_USE
}Identifiers;
typedef enum{
  B_PLUS,
  B_MINUS,
  B_CMP_GT,
  B_CMP_LT,
  B_MULT,
  B_EQ,
} BinOpId;
namespace Parser{
  void parse(std::vector<VR> output, std::vector<Token> &tokens);
  BinOpId parseBinOp(std::vector<Token>::iterator& boid){
    if	(boid->head.atomName == "+")		return B_PLUS;
    else if (boid->head.atomName == "-")	return B_MINUS;
    else if (boid->head.atomName == "<")	return B_CMP_LT;
    else if (boid->head.atomName == ">")	return B_CMP_GT;
    else if (boid->head.atomName == "*")	return B_MULT;
    else if (boid->head.atomName == "=")	return B_EQ;


    fprintf(stderr, "%lu:%lu: Error: Unreachable binary operand (%s)\n", 
            boid->head.atomIndexLine,
            boid->head.atomIndex,
            boid->head.atomName.c_str()
	    );
    exit(1);
  }
  Identifiers parseIdentifier(std::vector<Token>::iterator& id){    
    if (id->head.atomName == "dump")			return ID_INTRISIC_DUMP;
    else if (id->head.atomName == "???") return ID_DUMP_STACK;
    else if (id->head.atomName == "___SKIP_PROC")	return ID_SKIP_PROC;
    else if(id->head.atomName == "__PROC_ENTRY")	return ID_PROC_ENTRY;
    else if(id->head.atomName == "__PROC_RETURN")       return ID_PROC_RETURN;
    else if(id->head.atomName == "__PROC_LEAVE")	return ID_PROC_LEAVE;
    else if(id->head.atomName == "__ALOCATE__")         return ID_ALLOCATE_LOCAL_MEM;
    
    else if (id->head.atomName == "=")			return ID_INTRISIC_EQUALS; 
    else if (id->head.atomName == "dup")		return ID_INTRISIC_DUP;
    else if (id->head.atomName == "swap")		return ID_INTRISIC_SWAP;
    else if (id->head.atomName == "over")		return ID_INTRISIC_OVER;
    else if (id->head.atomName == "rot")		return ID_INTRISIC_ROT;
    else if (id->head.atomName == "drop")		return ID_INTRISIC_DROP;
    else if (id->head.atomName == "divmod")		return ID_INTRISIC_DIVMOD;

    else if (id->head.atomName == "idivmod")		return ID_INTRISIC_IDIVMOD;

    else if (id->head.atomName == ">>")			return ID_INTRISIC_SHIFT_RIGHT;
    else if (id->head.atomName == "<<")			return ID_INTRISIC_SHIFT_LEFT;

    else if (id->head.atomName == "and")		return ID_INTRISIC_AND;
    else if (id->head.atomName == "or")			return ID_INTRISIC_OR;
    else if (id->head.atomName == "not")		return ID_INTRISIC_NOT;

    else if (id->head.atomName == "__argv")		return ID_INTRISIC_ARGV;
    else if (id->head.atomName == "__argc")		return ID_INTRISIC_ARGC;


    else if (id->head.atomName == "store8")		return ID_STORE8;
    else if (id->head.atomName == "store64")		return ID_STORE64;

    else if (id->head.atomName == "load8")		return ID_LOAD8;
    else if (id->head.atomName == "load64")		return ID_LOAD64;
    else if (id->head.atomName == "mem")		return ID_MEM;
    else if (id->head.atomName == "if")			return ID_IF;
    else if (id->head.atomName == "while")		return ID_WHILE;
    else if (id->head.atomName == "do")			return ID_DO;
    else if (id->head.atomName == "elif")		return ID_ELIF;
    else if (id->head.atomName == "else")		return ID_ELSE;

    else if (id->head.atomName == "end")		return ID_END;
    else if (id->head.atomName == "!=")			return ID_INTRISIC_NOT_EQ;
    else if (id->head.atomName == "here")		return ID_HERE;

    else if (id->head.atomName == "cast(ptr)")		return ID_CAST;
    else if (id->head.atomName == "cast(int)")		return ID_CAST;
    else if (id->head.atomName == "cast(bool)")		return ID_CAST;
	

    else if (id->head.atomName == "syscall0")		return ID_SYSCALL_0;
    else if (id->head.atomName == "syscall1")		return ID_SYSCALL_1;
    else if (id->head.atomName == "syscall2")		return ID_SYSCALL_2;
    else if (id->head.atomName == "syscall3")		return ID_SYSCALL_3;
    else if (id->head.atomName == "syscall4")		return ID_SYSCALL_4;
    else if (id->head.atomName == "syscall5")		return ID_SYSCALL_5;
    else if (id->head.atomName == "syscall6")		return ID_SYSCALL_6;


    else{

      if(id->head.atomName[0] == '"'){
	id->head.atomName = id->head.atomName.substr(1, id->head.atomName.length() - 2);
	if( id->head.atomName.length() == 0 ){
	  return ID_NOOP;
	}
	return ID_STRING;
      }
      else if( static_addresses.find(id->head.atomName) != static_addresses.end() ){
	return ID_STATIC_MEM_USE;
      }
      else if( proc_definitions.find(id->head.atomName) != proc_definitions.end()){
	return ID_CALL_PROC;
      }
      else if( local_memories.find(id->head.atomName) != local_memories.end()){
	return ID_PUSH_LOCAL_MEM;
      }
      else if( struct_def.find(id->head.atomName) != struct_def.end()){
	return ID_STRUCT_ACCESS;
      }
      else{
	fprintf(stderr, "%lu:%lu: Error: Unreachable identifier at `%s` in the Parsing stage.\n", 
		id->head.atomIndexLine,
		id->head.atomIndex,
		id->head.atomName.c_str()
		);
	exit(1);
      }
    }
  }

  std::vector<std::pair<VR, Token>> parse(std::vector<Token> &tokens){
    std::vector<Token>::iterator Node = tokens.begin();
    std::vector<VR> output;
    while( Node != tokens.end()){
      switch(Node->type){ 
      case NAME:{                                        
	Identifiers id = Parser::parseIdentifier(Node);
	switch(id){
	case ID_STRUCT_ACCESS: {
	  ++Node;
	  std::string access_type = Node->head.atomName;
	  if( access_type != ":" && access_type != "->"){
	    fprintf(stderr, "%lu:%lu: Error: you can only access struct with `:` or `->`, found `%s`\n",
		    Node->head.atomIndexLine,
		    Node->head.atomIndex,
		    Node->head.atomName.c_str()
		    );
	    exit(1);	    
	  }
	  printf("ERROR: structs are not implemented yet\n");
	  exit(1);
	  break;
	}
	case ID_DUMP_STACK:
	  output.push_back(VR{
	      DUMP_STACK,
	      0,
	      "",
	      0
	    });
	  ++Node;
	  break;
	case ID_NOOP: 
	  ++Node;
	  break;
	case ID_ALLOCATE_LOCAL_MEM:
	{
	  ++Node;

	  output.push_back(VR{
	      ALLOC_LOCAL_MEM,
	      0,
	      Node->head.atomName,
	      0	      
	    });


	  ++Node;
	  break;
	} 
	case ID_PUSH_LOCAL_MEM:
	{
	  output.push_back(VR{PUSH_LOCAL_MEM, 0, Node->head.atomName, 0});
	  ++Node;
	  break;
	}
	case ID_CALL_PROC:
	  output.push_back(VR{ CALL_PROC, 0, Node->head.atomName, 0});
	  ++Node;
	  break;
	case ID_SKIP_PROC:
	{
	  output.push_back(VR{ PROC_SKIP, Node->head.atomLinkedIndex, Node->head.atomName, 0});
	  ++Node;
	  break;
	}
	case ID_PROC_RETURN:
	{
	  output.push_back(VR{ PROC_RETURN, Node->head.atomLinkedIndex, Node->head.atomName, 0});
	  ++Node;
	  break;
	}
	case ID_PROC_LEAVE:
	{
	  output.push_back(VR{ PROC_LEAVE, Node->head.atomLinkedIndex, Node->head.atomName, 0});
	  ++Node;
	  break;
	}	
	case ID_PROC_ENTRY:
	{
	  output.push_back(VR{ PROC_ENTRY, (++Node)->head.atomLinkedIndex, Node->head.atomName, 0});
	  current_proc = Node->head.atomName;
	  ++Node; 
	  break;
	}
	case ID_CAST:
	  {
	    output.push_back(VR{ CAST, 0, Node->head.atomName, 0 });
	    ++Node;
	    break;
	  }
	case ID_HERE:
	  {
	    std::string pos =std::to_string(Node->head.atomIndexLine)+":"+std::to_string(Node->head.atomIndex);
	    Words[(uint64_t)words_count] =  string_to_hex(pos);

	    output.push_back(VR{ PUSH_STR, (uint64_t)words_count, pos });
	    ++words_count;
	    ++Node;
	  } break;
	case ID_STRING:
	  {
	    if( Node->head.atomName.length() > 0 ){
	      std::string res;
	      uint64_t scape_count{0};
	      for(auto x = Node->head.atomName.begin(); x != Node->head.atomName.end(); ++x){

		if( *x == '\\' ){
		  ++x;
		  switch(*x){
		  case 'n': 
		    res += '\n';
		    scape_count++;
		    break;
		  }
		} else{
		  res  += *x;
		}
	      }
	      Words[(uint64_t)words_count] = string_to_hex(res);
	      output.push_back(VR{ PUSH_STR, (uint64_t)words_count, res, scape_count});
	      ++words_count;
	    }
	    ++Node;
	  } break;
	case ID_STATIC_MEM_USE:
	  {
	    output.push_back(VR{PUSH_PTR, 0, Node->head.atomName});
	    ++Node;
	  } break;
	case ID_STORE8:output.push_back(VR{OP_STOREBYTE, 8}); ++Node;break;
	case ID_STORE64:output.push_back(VR{OP_STOREBYTE, 64}); ++Node;break;
                        

	case ID_LOAD8:output.push_back(VR{OP_LOADBYTE,    8}); ++Node; break;
	case ID_LOAD64:output.push_back(VR{OP_LOADBYTE,    64}); ++Node; break;
                            
	case ID_MEM:
	  output.push_back(VR{OP_MEM,   0});
	  ++Node;
	  break;
	case ID_WHILE:
	  output.push_back(VR{OP_WHILE, 0});
	  ++Node;
	  break;
	case ID_INTRISIC_DUP:
	  output.push_back(VR{DUP, 0});
	  ++Node;
	  break;
	case ID_INTRISIC_SWAP:
	  output.push_back(VR{SWAP,	0});
	  ++Node;
	  break;
	case ID_INTRISIC_OVER:
	  output.push_back(VR{OVER,	0});
	  ++Node;
	  break;
	case ID_INTRISIC_ROT:
	  output.push_back(VR{ROT,	0});
	  ++Node;
	  break;
	case ID_INTRISIC_DROP:
	  output.push_back(VR{DROP,	0});
	  ++Node;
	  break;
	case ID_INTRISIC_DIVMOD:
	  output.push_back(VR{OP_DIVMOD, 0});
	  ++Node;
	  break;
	case ID_INTRISIC_IDIVMOD:
	  output.push_back(VR{OP_IDIVMOD, 0});
	  ++Node;
	  break;
	case ID_INTRISIC_SHIFT_LEFT: 
	  output.push_back(VR{SHIFT_LEFT, 0});
	  ++Node; break;
	case ID_INTRISIC_SHIFT_RIGHT: output.push_back(VR{SHIFT_RIGHT, 0});++Node; break;

	case ID_INTRISIC_AND: output.push_back(VR{AND, 0});++Node; break;
	case ID_INTRISIC_OR: output.push_back(VR{OR, 0});++Node; break;

	case ID_INTRISIC_NOT: output.push_back(VR{NOT, 0}); ++Node; break;
	case ID_INTRISIC_ARGV: output.push_back(VR{ARGV, 0}); ++Node; break;
	case ID_INTRISIC_ARGC: output.push_back(VR{ARGC, 0}); ++Node; break;
	case ID_END:
	  output.push_back(VR{OP_END, static_cast<uint64_t>(Node->head.atomLinkedIndex)});
	  ++Node;
	  break;
	case ID_INTRISIC_NOT_EQ:
	  output.push_back(VR{OP_NOT_EQUALS,	0});
	  ++Node;
	  break;

	case ID_SYSCALL_0: output.push_back(VR{OP_SYSCALL, 0}); ++Node; break;
	case ID_SYSCALL_1: output.push_back(VR{OP_SYSCALL, 1}); ++Node; break;
	case ID_SYSCALL_2: output.push_back(VR{OP_SYSCALL, 2}); ++Node; break;
	case ID_SYSCALL_3: output.push_back(VR{OP_SYSCALL, 3}); ++Node; break;
	case ID_SYSCALL_4: output.push_back(VR{OP_SYSCALL, 4}); ++Node; break;
	case ID_SYSCALL_5: output.push_back(VR{OP_SYSCALL, 5}); ++Node;  break;
	case ID_SYSCALL_6: output.push_back(VR{OP_SYSCALL, 6}); ++Node; break;

	case ID_DO:
	  output.push_back(VR{OP_DO,  static_cast<uint64_t>(Node->head.atomLinkedIndex)});
	  ++Node;
	  break;
	case ID_ELSE:
	  output.push_back(VR{OP_ELSE,  static_cast<uint64_t>(Node->head.atomLinkedIndex)});
	  ++Node;
	  break;
	case ID_ELIF:
	  printf("keywords `else` or `elif` Not implemented yet\n");
	  fprintf(stderr, "%lu:%lu: Error: Internal keyword `elif` not implemented yet\n",
		  Node->head.atomIndexLine,
		  Node->head.atomIndex
		  );
	  exit(1);
	  ++Node;
	  break;
	case ID_IF:{
	  output.push_back(VR{OP_IF, 0});
	  ++Node;
	  break;
	}

	case ID_INTRISIC_DUMP:
	  output.push_back(VR{DUMP, 0});
	  ++Node;
	  break;
                        
	case ID_INTRISIC_EQUALS: 
	  output.push_back(VR{OP_EQUALS,  0});
	  ++Node;
	  break;
	}
	break;
      }

      case NUMBER:
	{ 
	  output.push_back(VR{PUSH_INT, string_to_uint64(Node->head.atomName)});
	  ++Node;
	  break;
	}
      case BINARY_OPERAND:
	switch(Parser::parseBinOp(Node)){
	case B_EQ:
	  output.push_back(VR{OP_EQUALS,  0});
	  break;
	case B_MULT:
	  output.push_back(VR{OP_MULT,    0, ""});
	  break;

	case B_PLUS:
	  output.push_back(VR{OP_PLUS,    0, ""});
	  break;
	case B_MINUS:
	  output.push_back(VR{OP_MINUS,   0, ""});
	  break;
	case B_CMP_LT:
	  output.push_back(VR{OP_LTHAN,   0, ""});
	  break;
	case B_CMP_GT:
	  output.push_back(VR{OP_GTHAN,   0, ""});
	  break;
	}
	++Node;
	break;
                
      }
    }
    // make the pair
    std::vector<std::pair<VR, Token>> FINAL;
    size_t i = 0;
    for(auto& ins: output){
      FINAL.push_back(std::pair<VR, Token>(ins, tokens[i]));
      ++i;
    }
    return FINAL;
  }
}
#endif /* ifndef PARSER */

/*  NOTE:
 *      A tokenList is a vector of Atoms
 *      tokenList ==>  vec 
 *                      |   
 *                    Atom
 *                  /   |  \
 *              name  index type
 */


#ifndef LEXER
#define LEXER value

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include <map>
#include "./Core/Instructions.cpp"
#include "../frontend/StringView.cpp"

/*  NOTE:
 *      A atom is a identifier for each word
 *      name is the name itself
 *      type is str/int types likely
 *      and index is the positio in the text
 */

class Atom{
public:
    std::string atomName;
    std::string atomType;
    uint64_t      atomIndex;
    uint64_t      atomIndexLine;
    uint64_t      atomLinkedIndex;
};
typedef enum Token_Type{
    NUMBER,
    BINARY_OPERAND,
    NAME
} Token_Type;
typedef struct Token{
    Token_Type	      type;
    Atom	      head;
} Token;

static std::map<std::string, std::vector<Token>> Macros;
static std::map<uint64_t , std::string> Words;
static std::map<std::string, uint64_t> static_addresses;
static std::map<std::string, int> proc_definitions;
static std::map<std::string, std::pair<std::vector<Token>, std::vector<Token>>> proc_args_context;
static std::map<std::string, std::vector<std::pair<std::string, std::string>>> struct_def;

static std::string proc_context;
// NOTE:
//   local_memories[0] == mem_name
//   local_memories[1] == proc_context, mem_size

static std::map<std::string, std::pair<std::string, int>> local_memories;
static std::map<std::string, int> proc_need_allocate;
int proc_count{0};
static int words_count{0};
static int static_mem_capacity{0};
static uint64_t proc_local_vars{0};
namespace Lexer{
    std::string Token_Type_To_String(Token_Type tk){
	switch(tk){
	    case NAME:
		return "Identifier";
		break;
	    case BINARY_OPERAND: 
		return "BINARY_OPERAND";
		break;
	    case NUMBER:
		return "Number";
		break;
	    default:
		assert(false && "Unreachable");
	}
    }
    std::string Tokenize(std::string& word, Token_Type &op){
      if( is_number(word)){
	op = NUMBER;
	return ("NUMERICAL_TYPE");
      } else if (word == "+"){
	op = BINARY_OPERAND;
	return ("OP_PLUS");
      } else if (word == "-"){
	op = BINARY_OPERAND;
	return ("OP_MINUS");
      } else if (word == "*"){
	op = BINARY_OPERAND;
	return ("OP_MULT");
      } else if (word == "="){
	op = BINARY_OPERAND;
	return ("OP_EQ");
      } else if (word == "<"){
	op = BINARY_OPERAND;
	return ("OP_CMP_LT");
      } else if (word == ">"){
	op = BINARY_OPERAND;
	return ("OP_CMP_GT");
      } else{
	op = NAME;
	return ("NAME_TYPE");
      }
    }
  auto parse_proc_args(
		       std::vector<Token>::iterator& i)
    -> std::pair<std::vector<Token>, std::vector<Token>>
  {
    std::vector<Token> args;
    std::vector<Token> ret;
    
    bool is_arg = true;
    while(i->head.atomName !="do")
    {
      if( i->head.atomName == "->")
	{
	  is_arg = false;
	  ++i;
	}
      if( is_arg )
	{
	  args.push_back(Token{
	      .type = i->type,
	      .head = i->head
	    });
	}
      else
	{
	  ret.push_back(Token{
	      .type = i->type,
	      .head = i->head
	    });
	}

      ++i;
    }
    return std::pair<std::vector<Token>, std::vector<Token>>(args, ret);    
  }
  void extend_macro(std::vector<Token>& result, Token &it){
      if (Macros.find(it.head.atomName) != Macros.end()){
	for(Token& tk: Macros[it.head.atomName]){
	  if( Macros.find(tk.head.atomName) != Macros.end()){
	    extend_macro(result, tk);
	  }
	  else
	    result.push_back(tk);
	}
      }
      else
	result.push_back(it);
    }
      std::vector<uint64_t> compiler_time_eval(std::vector<Token> vec){        
        std::vector<uint64_t> mem_vec;
	std::vector<Token>::iterator i = vec.begin();
        while(i != vec.end()){
          if (i->type == BINARY_OPERAND){
            if( i->head.atomName == "+" ){
              auto a = mem_vec.back(); mem_vec.pop_back();auto b = mem_vec.back(); mem_vec.pop_back();
              mem_vec.push_back(a + b);
            }
            else if( i->head.atomName == "-" ){
              auto a = mem_vec.back(); mem_vec.pop_back();auto b = mem_vec.back(); mem_vec.pop_back();
              mem_vec.push_back(b - a);
                                                                                                       
            }
            else if( i->head.atomName == "*" ){
              auto a = mem_vec.back(); mem_vec.pop_back();auto b = mem_vec.back(); mem_vec.pop_back();
              mem_vec.push_back(b*a);
            }
            else if( i->head.atomName == "/" ){
              auto a = mem_vec.back(); mem_vec.pop_back();auto b = mem_vec.back(); mem_vec.pop_back();
              mem_vec.push_back(b / a);
            }
	    else if( i->head.atomName == "=" ){
              auto a = mem_vec.back(); mem_vec.pop_back();auto b = mem_vec.back(); mem_vec.pop_back();
              mem_vec.push_back(b == a);
            }
          } 
          
          else if( SV::str_is_num(i->head.atomName) ){
            mem_vec.push_back(std::stoul(i->head.atomName));
          }
	  else {
	    fprintf(stderr, "%lu:%lu:ERROR: expected number or binary operation but got `%s`\n",
		    i->head.atomIndexLine, i->head.atomIndex, i->head.atomName.c_str());
	    exit(1);
	  }
          ++i;
	}
        if( mem_vec.size() > 1 ){
          fprintf(stderr, "%lu:%lu:ERROR: after static definition not empty stack of operations\n",
              i->head.atomIndexLine, i->head.atomIndex);
          exit(1);
        }
	return mem_vec;
                                                                                                       
    }
  void extend_block(std::vector<Token>& result, std::vector<Token>::iterator& i){
    int do_count = 0;
    std::string local_mem_name;
    

    while(do_count > 0 || i->head.atomName != "end"){
      if( i->head.atomName == "local"){
	result.push_back(Token{
	    .type = i->type,
	    .head = {
	      .atomName		= "__ALOCATE__",
	      .atomType         = i->head.atomType,
	      .atomIndex	= i->head.atomIndex,
	      .atomIndexLine	= i->head.atomIndexLine,
	      .atomLinkedIndex	= i->head.atomLinkedIndex
	  
	    }
	  });
	++i;
	std::string local_mem_name = i->head.atomName;
	++i;
	std::vector<Token> local_mem_vec;
	while(i->head.atomName != "end"){
	  if (Macros.find(i->head.atomName) != Macros.end()){
	    for(Token& tk: Macros[i->head.atomName])
	      extend_macro(local_mem_vec, tk);
	  }
	  else{
	    local_mem_vec.push_back(Token{
		.type = i->type,
		.head = i->head
	      });
	  }
	  ++i;	    
	}

	++i;

	std::vector<uint64_t> local_mem_size = compiler_time_eval(local_mem_vec);

	local_memories[local_mem_name] = {
	  proc_context, proc_local_vars 
	};


	proc_local_vars += (int)local_mem_size[0];
	proc_need_allocate[proc_context] = (int)proc_local_vars;

	result.push_back(Token{
	    .type = i->type,
	    .head = {
	      .atomName		= local_mem_name,
	      .atomType         = i->head.atomType,
	      .atomIndex	= i->head.atomIndex,
	      .atomIndexLine	= i->head.atomIndexLine,
	      .atomLinkedIndex	= i->head.atomLinkedIndex
	  
	    }
	  });
	continue;
	
      }
      if      (i->head.atomName == "do") do_count++;
      else if (i->head.atomName == "end") do_count--;
	      
      if (Macros.find(i->head.atomName) != Macros.end()){
	for(Token& tk: Macros[i->head.atomName])
	  extend_macro(result, tk);
      }
      else
	result.push_back(Token{.type=i->type, .head=i->head});	      
      ++i;
    }

  }

    void lex_line(std::vector<Token> &tokenVector, std::string source, uint64_t line){
        SV::stringView src = SV::SV(source);
        uint64_t  start = src.count;
        uint64_t  pos{};
        Token_Type           actualType;
        std::string          actualWord;
        std::string          token;
        std::vector<Token>   tokenList;
        while(src.count > 1){
            actualWord = SV::separateByTokens(src);
            pos = start - src.count;
            token = Lexer::Tokenize(actualWord, actualType);
	    if(actualWord.length() > 0){
		tokenList.push_back(
            	    Token{
            	        .type = actualType,
            	        .head = Atom{
            	            actualWord, 
            	            token, 
            	            pos - actualWord.length(),
            	            line,
            	            0
            	        },
            	    }
            	);
	    }
        }
        for(auto& token_after_lex : tokenList)
            tokenVector.push_back(token_after_lex);
    }
    auto lex(std::fstream& file) -> std::vector<Token>
    {
	std::vector<Token> pre_result;
	std::string line_string;
	uint64_t    line_count = 0;
	while( getline(file, line_string)){
	    Lexer::lex_line(pre_result, line_string, line_count);
	    ++line_count;
	}
	file.close();
	std::vector<Token> result;
	for(auto i = pre_result.begin(); i != pre_result.end(); ++i){
	  if( i->head.atomName == "#define" ){
	    ++i;
	    std::string m_name = i->head.atomName;
	    if( Macros.find(m_name) != Macros.end()){
	      printf("%lu:%lu: ERROR: Redefinition of macro\n", 
	          i->head.atomIndexLine, 
	          i->head.atomIndex);
	      printf("	NOTE: previous declared here %lu:%lu\n", 
	          Macros[m_name].begin()->head.atomIndexLine, 
	          Macros[m_name].begin()->head.atomIndex);
	      exit(1);
	    }
	    ++i;
	    if( i->head.atomName != "as" ){
	      printf("%lu:%lu: ERROR: Macro definition must follow the keyword `do`\nExample: define <macro-name> as <macro-body> end\n", i->head.atomIndexLine, i->head.atomIndex);
	      printf("MACRO NAME: %s\n", m_name.c_str());
	      exit(1);
	    }
	    ++i;
	    std::vector<Token> m__body;
	    uint64_t do_counts{0};
	    while(i->head.atomName != "end" || do_counts > 0){ 
	      if (i->head.atomName == "do"){
	        ++do_counts;
	      } else if ( i->head.atomName == "end" ){
	        --do_counts;
	      }

	      m__body.push_back(
	            Token{
	              .type=i->type,
	              .head=i->head
	            }
	          );
	      ++i; 
	    }
	    Macros[m_name] = m__body;
	  }


	  else if ( Macros.find(i->head.atomName) != Macros.end()){
	    for(Token& tk: Macros[i->head.atomName]){
	      extend_macro(result, tk);
	    }
	  }
	  else if ( i->head.atomName == "#include" ){
	    ++i;

	    i->head.atomName = i->head.atomName.substr(1, i->head.atomName.length() - 2);
	    std::fstream include_file(i->head.atomName, std::ios::in);
	    if (!include_file.is_open()){
	      fprintf(stderr, 
		      "%lu:%lu: ERROR: could not open the include file `%s`\n",
		      i->head.atomIndexLine,
		      i->head.atomIndex,
		      i->head.atomName.c_str());
	      exit(1);
	    }
	    std::vector<Token> __include__content = Lexer::lex(include_file);
	    for(auto& content: __include__content){
	      result.push_back(content);
	    }
	  } 
	  else if ( i->head.atomName == "static" ){

	    ++i;
	    if( i->type != NAME ){
	      fprintf(stderr, 
	          "%lu:%lu: ERROR: expected token kind NAME after static keyword but got %s\n",
	          i->head.atomIndexLine, i->head.atomIndex, Token_Type_To_String( i->type ).c_str()
	      );
	      // TODO: report definition of static address
	      // TODO: report redefinition of intrisics
	      // i = mem_name 
	      // sintax: static steq.mem 8 end
	      exit(1);
	    }
	    // 
	    std::string mem_name = i->head.atomName;
	    ++i;
	    std::vector<Token> static_vec;
	    while( i->head.atomName != "end" ){
	      if( Macros.find(i->head.atomName) != Macros.end() ){
		for(Token& tk: Macros[i->head.atomName]){
		  extend_macro(static_vec, tk);
		}
	      }
	      else{
		static_vec.push_back(Token{
		    .type=i->type,
		    .head=i->head
		    });
	      }
	      ++i;
	    }
	    std::vector<uint64_t> static_resb_result = compiler_time_eval(static_vec);

	    static_addresses[mem_name] = (size_t)static_mem_capacity; // - (size_t)(static_resb_result.back());
	    static_mem_capacity += (int)static_resb_result.back();

	  }
	  else if (i->head.atomName == "proc"){
	    ++i;
	    proc_local_vars = 0;
	    if( proc_definitions.find(i->head.atomName) != proc_definitions.end())
	     {
	       printf("%lu:%lu: ERROR can not define the same proc twice\n",
		      i->head.atomIndexLine,
		      i->head.atomIndex);
	       exit(1);
	     }
	    proc_definitions[i->head.atomName] = 1;	    
	    std::string proc_name = i->head.atomName;

	    ++i;
	    std::vector<Token> proc_args;

	    proc_args_context[proc_name] = parse_proc_args(i);
	    if(proc_name == "main"){
	      if(proc_args_context[proc_name].first.size() != 0
		 || proc_args_context[proc_name].second.size() != 0)
		{
		  printf("%lu:%lu: ERROR the main procedure does not support input type or return type \n",
			 i->head.atomIndexLine,
			 i->head.atomIndex);
		  exit(1);
		}
	    }
	   
	    else{
	      if(proc_args_context[proc_name].first.size() == 0 ){
		printf("%lu:%lu: ERROR expected input data type of the procedure \n",
		       i->head.atomIndexLine,
		       i->head.atomIndex);
		exit(1);
	      }
	      else if (proc_args_context[proc_name].second.size() == 0){
		printf("%lu:%lu: ERROR expected return data type of the procedure\n",
		       i->head.atomIndexLine,
		       i->head.atomIndex);
		exit(1);
	      }
	    }

	    i->head.atomName = "___SKIP_PROC";
	    result.push_back(Token{.type=i->type, .head=i->head});
	    // proc NAME __SKIP_PROC PROC_ENTRY ... __PROC_LEAVE

	    i->head.atomName = "__PROC_ENTRY";
	    result.push_back(Token{.type=i->type, .head=i->head});
	    i->head.atomName = proc_name;
	    result.push_back(Token{.type=i->type, .head=i->head});
	    // proc -> __SKIP PROC -> __PROC_ENTRY ... __PROC_END	    
	    ++i;

	    proc_context = proc_name;
	    extend_block(result, i);
	    
	    
	    i->head.atomName = "__PROC_RETURN";
	    result.push_back(Token{.type=i->type, .head=i->head});
	    
 	    i->head.atomName = "__PROC_LEAVE";
	    result.push_back(Token{.type=i->type, .head=i->head});
	    
	  }
	  else if (i->head.atomName == "struct"){
	    ++i;
	    std::string struct_name = i->head.atomName;
	    ++i;

	    while(i->head.atomName != "end"){	      
	      if(i->type != Token_Type::NAME){
		fprintf(stderr,
			"ERROR: expected name inside strut definition\n");
		exit(1);
	      }

	      std::string struct_field = i->head.atomName; ++i;
	      std::string struct_data_type  = i->head.atomName; ++i;

	      struct_def[struct_name].push_back(
                std::pair<std::string, std::string>{
		  struct_field, struct_data_type
	        });
	    }
	    printf("STRUCT: %s\n", struct_name.c_str());
	    for(auto& p: struct_def[struct_name]){
	      printf("STR: data %s, type: %s\n",
		     p.first.c_str(), p.second.c_str());
	      printf("----------------------------\n");
	    }
	  }
	  else if (i->head.atomName == "enum"){
	    ++i;
	    std::string enum_name = i->head.atomName;
	    ++i;
	    int enum_count = 0;
	    while(i->head.atomName != "end"){
	      std::string name_snapshot =
		enum_name + "::" + i->head.atomName;
	      // this will help to diferenciate from normal macros
	      i->head.atomName = std::to_string(enum_count);
	      i->type = NUMBER;
	      Macros[name_snapshot] = std::vector<Token>{{
		  .type = i->type,
		  .head = i->head
		}};
	      ++enum_count;
	      ++i;
	    }
	  }
	  else if (i->head.atomName == "static_assert"){
	    ++i;
	    std::string message = i->head.atomName;
	    ++i;
	    std::vector<Token> assert_tokens;
	    while(i->head.atomName != "end"){
	      if( Macros.find(i->head.atomName) != Macros.end() ){
		for(Token& tk: Macros[i->head.atomName]){
		  extend_macro(assert_tokens, tk);
		}
	      }
	      else{
	        assert_tokens.push_back(Token{
		    .type=i->type,
		    .head=i->head
		  });
	      }
	      ++i;
	    }
	    

	    std::vector<uint64_t> assert_result = compiler_time_eval(assert_tokens);
	    if(assert_result[0] != 1){
	      printf("%lu:%lu: static assert error: %s\n",
		     i->head.atomIndexLine,
		     i->head.atomIndex,
		     message.c_str());
	      exit(1);
	    }
	  }
	  else
	  {

	    result.push_back(
		Token{ 
		  .type=i->type,
		  .head=i->head
		}
	    );
	  }
	}
	return result;
    }
}


#endif /* ifndef LEXER */

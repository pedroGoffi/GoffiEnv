#ifndef __lexer
#define __lexer
#include <climits>
#include <math.h>
#include <cassert>
#include <string>
#define LEXER_HAS_LOCATION
#include "../../common/utils.cpp"
#include "./token.cpp"
void init_stream(const char *str);
int char_to_int(char c);
char escape_to_char(char c);
void lexer_scan_char();
void lexer_scan_float();
void lexer_scan_int();
void lexer_scan_string();
void next_token();
#define MAX_STREAMS_SIZE 1024
const char*  stream;
struct Streams{
  const char*  streams[MAX_STREAMS_SIZE];
  size_t       streams_size = 0;
};

Streams streams = {};

void Streams_push(const char* str){
  assert(streams.streams_size < MAX_STREAMS_SIZE);
  strcpy((char*)streams.streams[streams.streams_size++], str);
}
const char* Streams_pop(){
  return streams.streams[streams.streams_size--];
}
const char* var_keyword = str_intern("var");
const char* file;
size_t line_pos;
size_t col_pos;
void init_stream(const char *str){
  stream = str;
  next_token();
}

int char_to_int(char c){
  switch(c){
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  case 'a': case 'A': return 10;
  case 'b': case 'B': return 11;
  case 'c': case 'C': return 12;
  case 'd': case 'D': return 13;
  case 'e': case 'E': return 14;
  case 'f': case 'F': return 15;
  default:  return 0;
  }
}
char escape_to_char(char c){
  switch(c){
  case 'n': return '\n';
  case 'r': return '\r';
  case 't': return '\t';
  case 'v': return '\v';
  case 'b': return '\b';
  case 'a': return '\a';
  case '\\': return '\\';
  default:  return 0;    
  }
}
char unescape_to_char(char c){
  switch(c){
  case '\n': return 'n';
  case '\r': return 'r';
  case '\t': return 't';
  case '\v': return 'v';
  case '\b': return 'b';
  case '\a': return 'a';
  default:  return 0;    
  }
}
const char* unescape_string(const char* str){
  char* buf = NULL;
  for(size_t i=0; i < buf__len(str);  ++i){
    char c   = str[i];
    char unc = unescape_to_char(c);
    
    if(unc == 0){
      buf__push(buf, c);
    }
    else {
      buf__push(buf, '\\');
      buf__push(buf, unc);
      
    }
  }
  return buf;
}
void lexer_scan_char(){
  assert(*stream == '\'');
  stream++;
  char c = 0;
  if(*stream == '\''){
    syntax_error("char literal can not be empty\n");
    exit(1);    
  }
  else if (*stream == '\\'){
    stream++;
    c = escape_to_char(*stream);
    if(c == 0 && *stream != '0'){
      syntax_error("invalid char literal escape `%c` found.\n", *stream);
      exit(1);
    }
    stream++;
  } else {
    c = *stream;
    stream++;
  }
  if(*stream != '\''){
    syntax_error("expected `'`, got '%c'\n", *stream);
    exit(1);
  }
  stream++;
  token.kind     = TOKEN_INT;
  token.INT      = (int)c;
  token.modifier = TOKENMOD_CHAR;
  
}

void lexer_scan_string(){
  assert(*stream == '"');
  stream++;
  char *buf = NULL;
  while(*stream && *stream != '"'){
    char c = *stream;
    
    if(*stream == '\n'){
      syntax_error("string literal can not contain newlines.\n");
      exit(1);
    } else if (c == '\\') {
      stream++;
      c = escape_to_char(*stream);
      if(c == 0 && *stream != '0'){
	syntax_error("invalid scape character in string literal '\\%c'.\n", *stream);
	exit(1);
      }      
    }
    buf__push(buf, (int)c);
    stream++;    
  }
  if(*stream){
    assert(*stream == '"');
    stream++;
  } else {
    syntax_error("reach end of the file before lex the string.\n");
    exit(1);
  }

  buf__push(buf, 0);
  token.kind    = TOKEN_STRING;
  token.STRING  = buf;
  token.name    = buf;

}
const char* strip_quotes(const char* src){
  char* str = NULL;
  for(size_t i=1; src[i] != '"'; ++i){
    buf__push(str, src[i]);
  }
  return (const char*)(str);
}
int number_base;
// FLOAT = [0-9]*[.][0-9]*([eE][+-]?[0-9]+)?
 void lexer_scan_float(){
  const char* f_start = stream;
  while(isdigit(*stream)) stream++;
  if(*stream == '.'){
    stream++;
  }
  while(isdigit(*stream)) stream++;
  if(tolower(*stream) == 'e'){
    stream++;
    if(*stream == '+' || *stream == '-'){
      //
      stream++;
    }
    if(!isdigit(*stream)){
      syntax_error("Expected digit after float literal, found '%c'\n", *stream);
      exit(1);      
    }
    while(isdigit(*stream)) stream++;
  }
  //const char* f_end = stream;
  double f_val = strtod(f_start, NULL);
  if(f_val == HUGE_VAL || f_val == -HUGE_VAL){
    syntax_error("float literal overflow\n");
    exit(1);
  }
  token.FLOAT = f_val;
  token.kind = TokenKind::TOKEN_FLOAT; 
}
 // INT   = [1-9][0-9]* | 0[xX][0-9a-fA-F]+
 void lexer_scan_int(){
  number_base = 10;
  TokenModifier mod = TokenModifier::TOKENMOD_NONE;
  if(*stream == '0'){
    stream++;
    if(tolower(*stream) == 'x'){
      stream++;
      number_base = 16;
    }
    else if (isdigit(*stream)){
      number_base = 8;
    }   
  }  
  int acc = 0;
  for(;;){
    int digit  = char_to_int(*stream);
    if(digit == 0 && *stream != '0'){
      break;
    }
    if(digit >= number_base){
      syntax_error("digit '%c' out of range for base %d\n", *stream, number_base);
      exit(1);
    }

    if(acc > (INT_MAX - digit)/number_base){
      syntax_error("Integer literal overflow\n");
      exit(1);
    }
    acc = acc*number_base + digit;
    stream++;
  }
  token.kind		= TokenKind::TOKEN_INT;
  token.INT		= acc;
  token.modifier	= mod;
}

void next_token(){
 __next_token:
  const char* stream_begin = stream;
  switch(*stream){
  case ' ': case '\t': case '\n':
    stream++;
    goto __next_token;
    break;
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': 
  case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': 
  case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': 
  case 'y': case 'z': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
  case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': 
  case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
  case 'W': case 'X': case 'Y': case 'Z': case '_':
    while(std::isalnum(*stream) || *stream == '_' ){
      stream++;
    }
    token.kind		= TokenKind::TOKEN_NAME;
    token.name          = str_intern_range(stream_begin, stream);
    break;
  case '\'':
    lexer_scan_char();
    token.name          = str_intern_range(stream_begin, stream);
    break;
  case '"':
    lexer_scan_string();
    break;
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9': {
    while(isdigit(*stream)) stream++;
    
    if(*stream == '.'){
      stream = stream_begin;
      lexer_scan_float();
    }
    else{
      stream = stream_begin;
      lexer_scan_int();      
    }
    assert(*stream != '.');
    token.name          = str_intern_range(stream_begin, stream);

  } break;
  case '/':
    token.kind = TOKEN_DIV;
    stream++;
    token.name          = str_intern_range(stream_begin, stream);
    if (*stream == '/'){
      while(*stream++ != '\n'){}
      next_token();
    }

    break;

#define kind1(k, r)							\
    case k:								\
      token.kind = r;							\
      stream++;								\
      token.name          = #k;						\
      break;
    // ------------------------------    
    kind1('#',  TOKEN_HASHTAG);
    kind1('$',  TOKEN_DOLLAR);
    kind1('&',  TOKEN_AMPERSAND);
    kind1('*',  TOKEN_STAR);

    kind1(',',  TOKEN_COMMA);
    kind1('.',  TOKEN_DOT);
    kind1('[',  TOKEN_OPEN_S_PAREN);
    kind1(']',  TOKEN_CLOSE_S_PAREN);
    kind1(';',  TOKEN_DOT_AND_COMMA);
    
    
    kind1('?',  TOKEN_QUESTION_MARK);
    kind1('(',  TOKEN_OPEN_R_PAREN);
    kind1(')',  TOKEN_CLOSE_R_PAREN);
    kind1('{',  TOKEN_OPEN_C_PAREN);
    kind1('}',  TOKEN_CLOSE_C_PAREN);
    kind1('\0', TOKEN_EOF);
#undef kind1           
#define kind2(fkind, fres, sres)					\
    case fkind:								\
      token.kind = fres;						\
      stream++;								\
      if (*stream == fkind){						\
	token.kind = sres;						\
	stream++;							\
      }									\
      token.name          = str_intern_range(stream_begin, stream);\
      break;
    // ------------------------------
    
    kind2(':', TOKEN_DOUBLE_DOT, TOKEN_ACCESS_FIELD);
    kind2('=', TOKEN_EQ, TOKEN_CMP_EQ);
#undef kind2
    //kind1('<',  TOKEN_LESS);
#define kind3(f, fk, s, sk)			\
    case f:{					\
      token.kind = fk;				\
      token.name = #f;				\
      stream++;					\
      if(*stream == s){				\
	token.kind = sk;			\
	token.name = #s;			\
	stream++;				\
      }						\
    } break;
    kind3('!', TOKEN_BANG,	'=', TOKEN_CMP_NEQ);
#define kind4(f, fk, s, sk, t, tk)		\
    case f:					\
      token.kind = fk;				\
      token.name = #f;				\
      stream++;					\
      if(*stream == s){				\
	token.kind = sk;			\
	token.name = #f #s;			\
	stream++;				\
      }						\
      else if (*stream == t){			\
	token.kind = tk;			\
	token.name = #f #t;			\
	stream++;				\
      }						\
      break;
    kind4('<', TOKEN_LESS,	'<', TOKEN_SHL,   '=', TOKEN_LESSEQ);
    kind4('>', TOKEN_GREATER,	'>', TOKEN_SHR,   '=', TOKEN_GREATEREQ);
    kind4('-', TOKEN_TAKEAWAY,  '-', TOKEN_DLESS, '=', TOKEN_EQLESS);
    kind4('+', TOKEN_PLUS,      '+', TOKEN_DPLUS, '=', TOKEN_EQPLUS);
    
      //kind2('-', TOKEN_TAKEAWAY, TOKEN_DLESS); 
      //kind2('+', TOKEN_PLUS, TOKEN_DPLUS);
    //case '<':
  //  token.kind = TOKEN_LESS;
  //  stream++;
  //  token.name = "<";
  //  if(*stream == '='){
  //    token.kind = TOKEN_LESSEQ;
  //    stream++;
  //    token.name = "<=";
  //  }
  //  break;
  default:
    stream++;
    goto __next_token;
    break;    
  }
  
}
inline bool is_token(TokenKind kind){
  return token.kind == kind;
}
inline bool is_token(char kind){
  return token.kind == (TokenKind)(kind);
}
inline bool token_is_name(const char* name){
  return token.kind == TokenKind::TOKEN_NAME && strcmp(token.name, name) == 0;
}
inline bool expect_token(TokenKind kind){
  if(is_token(kind)){
    next_token();
    return true;
  }
  return false;
}
inline bool expect_name(const char* name){
  if(token_is_name(name)){
    next_token();
    return true;
  }
  return false;
}
inline void MustExpectName(const char* n){
  if(!token_is_name(n)){
    printf("FATAL: expected `%s`, but got `%s`.\n",
	   n,
	   token.name);
    exit(1);
  }
  next_token();
}
inline bool MustExpect(TokenKind kind){
  if(is_token(kind)){
    next_token();
    return true;
  }
  printf("FATAL: expected `%s`, but got `%s`\n",
	 human_readable_tokenkind(kind),
	 human_readable_token(token));
  exit(1);
}
inline Token consume(){
  Token snapshot = token;
  next_token();
  return snapshot;
}
const char* get_file_text(const char* fp){
  file = fp;
  std::FILE* fd = fopen(fp, "r");
  if(!fd) return NULL;
  fseek(fd, 0, SEEK_END);
  auto fsize = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  char* fcontent = (char*)malloc(sizeof(char) * fsize + 1);
  fread(fcontent, 1, fsize, fd);
  fclose(fd);
  return (const char*)(fcontent);
}
#endif /* __lexer */

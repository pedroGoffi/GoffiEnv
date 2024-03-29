#ifndef __token
#define __token
#include <iostream>
#include "./lexer.cpp"
#define TOKEN_NAME_MAX_CAP 258

typedef enum TokenKind{
  // SIMPLE TOKENS
  TOKEN_EOF		= 0,
  TOKEN_BANG		= 33,
  TOKEN_HASHTAG		= 35,
  TOKEN_DOLLAR		= 36,
  TOKEN_PERCENT		= 37,
  TOKEN_AMPERSAND	= 38,
  TOKEN_OPEN_R_PAREN    = 40,
  TOKEN_CLOSE_R_PAREN   = 41,
  TOKEN_STAR		= 42,
  TOKEN_PLUS		= 43,
  TOKEN_COMMA		= 44,
  TOKEN_TAKEAWAY	= 45,
  TOKEN_DOT		= 46,
  TOKEN_DIV             = 47,
  TOKEN_DOUBLE_DOT	= 58,
  TOKEN_DOT_AND_COMMA	= 59,
  TOKEN_LESS		= 60,
  TOKEN_EQ		= 61,
  TOKEN_AT_SIGN         = 64,
  TOKEN_GREATER		= 76,
  TOKEN_QUESTION_MARK	= 77,
  TOKEN_OPEN_S_PAREN    = 91,
  TOKEN_CLOSE_S_PAREN   = 93,
  TOKEN_OPEN_C_PAREN    = 123,
  TOKEN_CLOSE_C_PAREN   = 125,
  TOKEN_LAST_CHAR	= 127,
  // COMPLEX TOKENS, NOTE: a complex token need more than one char to define
  TOKEN_DLESS,
  TOKEN_DPLUS,
  TOKEN_CMP_EQ,
  TOKEN_CMP_NEQ,
  TOKEN_ACCESS_FIELD,
  TOKEN_INT,
  TOKEN_STRING,
  TOKEN_FLOAT,
  TOKEN_NAME,
} TokenKind;

typedef enum TokenModifier{
  TOKENMOD_NONE,
  TOKENMOD_INT,
  TOKENMOD_BIN,
  TOKENMOD_OCT,
  TOKENMOD_CHAR,
} TokenModifier;

// NOTE: __STRING is a not more not less of what i just need
typedef struct __STRING{
  const char *start;
  const char *end;

  // NOTE: fmt() will return a value that first in %s for printf
  const char *__fmt;
  const char* fmt(){
    char buf[TOKEN_NAME_MAX_CAP];
    sprintf(buf, "%.*s", (int)(this->end - this->start), this->start);
    this->__fmt = buf;
    return this->__fmt;
  }
  // NOTE: this will construct a string based on two pointers difference
  void set(const char* start, const char* end){
    this->start = start;
    this->end   = end;
  }
} __STRING;
typedef struct Token{
  TokenKind     kind;
  TokenModifier modifier;
  union{
    int           INT;
    double        FLOAT;
    const char*   STRING;
    const char*   name;
  };
} Token;
Token token;

const char *human_readable_token(TokenKind kind){  
  switch(kind){
  case TokenKind::TOKEN_STRING:		return "string";
  case TokenKind::TOKEN_INT:		return "number";
  case TokenKind::TOKEN_NAME:		return "name";
  case TokenKind::TOKEN_FLOAT:		return "float";
  case TokenKind::TOKEN_BANG:		return "!";
  case TokenKind::TOKEN_HASHTAG:	return "#";
  case TokenKind::TOKEN_DOLLAR:		return "$";
  case TokenKind::TOKEN_AMPERSAND:	return "&";
  case TokenKind::TOKEN_STAR:		return "*";
  case TokenKind::TOKEN_PLUS:		return "+";
  case TokenKind::TOKEN_COMMA:		return ",";
  case TokenKind::TOKEN_TAKEAWAY:	return "-";
  case TokenKind::TOKEN_DOT:		return ".";
  case TokenKind::TOKEN_DOUBLE_DOT:	return ":";
  case TokenKind::TOKEN_DOT_AND_COMMA:	return ";";
  case TokenKind::TOKEN_LESS:		return "<";
  case TokenKind::TOKEN_EQ:		return "=";
  case TokenKind::TOKEN_CMP_EQ:         return "==";
  case TokenKind::TOKEN_GREATER:	return ">";
  case TokenKind::TOKEN_QUESTION_MARK:	return "?";
  case TokenKind::TOKEN_DLESS:		return "--";
  case TokenKind::TOKEN_DPLUS:		return "++";
  case TokenKind::TOKEN_ACCESS_FIELD:	return "::";
  case TokenKind::TOKEN_OPEN_R_PAREN:   return "(";
  case TokenKind::TOKEN_CLOSE_R_PAREN:  return ")";
  case TokenKind::TOKEN_OPEN_C_PAREN:   return "{";
  case TokenKind::TOKEN_CLOSE_C_PAREN:  return "}";
  case TokenKind::TOKEN_OPEN_S_PAREN:   return "[";
  case TokenKind::TOKEN_CLOSE_S_PAREN:  return "]";    
  case TokenKind::TOKEN_PERCENT:        return "%";
  case TokenKind::TOKEN_DIV:            return "/";
  case TokenKind::TOKEN_EOF:            return "<EOF>";
  
    

  case TokenKind::TOKEN_LAST_CHAR:
  default:
    fatal("unexpected tokenkind in human_readable_token.\n");
    exit(1);
  }
}

#endif /* __token */

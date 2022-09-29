#ifndef __string__
#define __string__
#include <iostream>
#include <cstring>
#include "./utils.cpp"
InternStr istr = {};

char* trim_left(char* str, size_t str_len){
  for(size_t i=0; i < str_len; ++i){
    if(!isspace(str[i])){
      return str - i;
    }
  }
  return str + str_len;
}
char* str_chop_by(char* str, char delimiter){
  char* result = new char;
  char* str_begin = str;
  while(*str != '\0' && *str++ != delimiter ){} 
  sprintf(result, "%.*s", (int)(str - str_begin) - 1, str_begin);

  
 
  return result;

}
bool str_cmp(const char* a, const char* b){
  size_t a_len = strlen(a);
  size_t b_len = strlen(b);
  if(a_len != b_len) return false;
  for(size_t i=0; i< a_len; ++i){
    if(a[i] != b[i]) return false;
  }
  return true;
}
InternStr InternStr_chop_line(){
  char* resultstr = new char;
  char* end = (char*)memchr(istr.str, '\n', istr.len);

  size_t n = (end != NULL)? (size_t)(end - istr.str): istr.len;
  sprintf(resultstr, "%.*s",(int)n, istr.str);

  istr.str = end;
  istr.len -= n;
  if( istr.str != NULL ){
    istr.str++;
    istr.len--;
  }
  return {
    .len = n,
    .str = resultstr
  };
}

void InternStr_trim_left(InternStr* internstr){
  for(size_t i=0; isspace(internstr->str++[i]); ++i){}
  internstr->len = strlen(internstr->str);
}
#define InternStr_eq(a, b) (strcmp((a).str, (b).str) == 0)
#define Internstr(name) InternStr{.len = strlen(name), .str = (name)}
void init_internstr(const char* str){
  istr.len = strlen(str);
  istr.str = str;
}
#endif /* __string__ */

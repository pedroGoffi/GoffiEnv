#ifndef __lexer_gfsl
#define __lexer_gfsl
#include <iostream>
#define not_impl \
  printf("ERRROR: procedure %s is not implemented yet.\n", __FUNCTION__); \
  exit(1);
void init_stream(const char* str);
void next_token();
void lexer_prepare_stream_with_fp(const char* fp);
const char* get_file_text(const char* fp);
const char* stream = nullptr;

void lexer_prepare_stream_with_fp(const char* fp){
  const char* str = get_file_text(fp);
  init_stream(str);
}
void init_stream(const char* str){
  stream = str;
  next_token();
}
void next_token(){
  const char* begin = stream;
  while(*stream != ' ' || *stream != '\n' || *stream != '\0') {
    stream++;
  }
  printf("found a stream %.*s\n", (int)(stream-begin), begin);
  exit(1);
}

const char* get_file_text(const char* fp){
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
#endif /* __lexer_gfsl */

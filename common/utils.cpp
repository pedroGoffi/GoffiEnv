#ifndef __util
#define __util
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <string.h>

#define printf_buffer stdout
#undef  printf
#define printf(...) fprintf(printf_buffer, __VA_ARGS__)

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))

#define error_here(...)							\
  fprintf(stderr, "[%s:: %s] :: error: ", __FILE__, __FUNCTION__);	\
  fprintf(stderr, __VA_ARGS__);						\
  fprintf(stderr, "\n");						\
  abort();

#define find_map(map, what) (map.find(what) != map.end())

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
auto usage(std::FILE *stream, const char* program) -> void {
  fprintf(stream, "|| USAGE:\n");
  fprintf(stream, "||     %s <SUBCOMMAND> [OPTION] <file>\n", program);
  fprintf(stream, "|| SUBCOMMANDS:\n");
  fprintf(stream, "||     -h         will print this message to the standart output\n");
  fprintf(stream, "||     -o <out>   use the <out> as a file_path in order to generate the file\n");  
  fprintf(stream, "|| OPTIONS:\n");
  fprintf(stream, "||     -gen:ast   dump the parsed AST\n");
  fprintf(stream, "||     -gen:c     generate C code\n");
  fprintf(stream, "||     -gen:asm   generate asm, ARM x86_64 by default\n");
  
}
inline auto shift(int *argc, char***argv) -> const char* {
  assert(*argc > 0);
  const char* r = **argv;
  *argv += 1;
  *argc -= 1;
  return r;
}

typedef struct BufHdr{
  size_t len;
  size_t cap;
  char   buf[1]; // NOTE: if the allocator buggy, change the array size to zero  
} BufHdr;

#define BUF_SIZE_MAX 1024*1024*1024
#define buf__hdr(b)       ((BufHdr *)((char *)b - offsetof(BufHdr, buf)))
#define buf__fits(b, x)   (buf__len(b) + (x) <= buf__cap(b))
#define buf__fit(b, x)    (buf__fits(b, x) ? 0 : *(void **)&(b) = buf__grow(b, buf__len(b) + (x), sizeof(*(b))))
#define buf__end(b)       ((b) + buf__len(b))
			  
#define buf__len(b)       ((b) ? buf__hdr(b)->len  : 0)
#define buf__cap(b)       ((b) ? buf__hdr(b)->cap  : 0)
#define buf__push(b, ...) (buf__fit((b), 1 + buf__len(b)), (b)[buf__hdr(b)->len++] = (__VA_ARGS__))

#define buf__free(b)      ((b) ? free(buf__hdr(b)) : (void)0), ((b) = NULL)


void *buf__grow(const void *buf, size_t new_len, size_t elem_size){
  size_t new_cap = MAX(1+2*buf__cap(buf), new_len);
  assert(buf__cap(buf) <= (BUF_SIZE_MAX - 1) / 2);
  assert(new_len <= new_cap);
  size_t new_size = offsetof(BufHdr, buf) + new_cap*elem_size;
  BufHdr *new_hdr;
  if(buf){
    new_hdr  = (BufHdr*)std::realloc(buf__hdr(buf), new_size);
  } else {
    new_hdr  = (BufHdr*)std::malloc(new_size);
    new_hdr->len = 0;
  }
  new_hdr->cap = new_cap;
  return new_hdr->buf;
}


void buf_tests(){
  int *buf = NULL;  
  assert(buf__len(buf) == 0);
  size_t N = 1024;
  for(size_t i = 0; i < N; i++) {
    buf__push(buf, 420);
  }

  assert(buf__len(buf) == N);  
  printf("|| BufHdr TESTS ||\n");
  for(size_t i = 0; i < buf__len(buf); i++){
    int x = buf[i];
    printf("|| buffer || x[%zu] = %d\n", i, x);
  }
  buf__free(buf);
  assert(buf__len(buf) == 0);
  assert(buf == NULL);
}

typedef struct InternStr{
  size_t     len;
  const char *str;
} InternStr;

static InternStr *intern_str;
const char* str_intern_range(const char *start, const char* end){
  size_t len = end - start;
  for(size_t i=0; i < buf__len(intern_str); ++i){
    if(intern_str[i].len == len && strncmp(intern_str[i].str, start, len) == 0){
      return intern_str[i].str;
    }
  }
  char *str = (char*)std::malloc(len + 1);
  memcpy(str, start, len);
  str[len] = 0;
  InternStr new_intern = {
    .len = len,
    .str = str
  };
  buf__push(intern_str, new_intern);
  return str;
}
const char* str_intern(const char *str){
  return str_intern_range(str, str + strlen(str));
}

#ifdef LEXER_HAS_LOCATION
extern const char* file;
extern size_t      line_pos;
extern size_t      col_pos;

void fatal(const char *fmt, ...){
  va_list args;
  va_start (args, fmt);
  fprintf(stderr, "[%s:%zu:%zu]: fatal error: ", file, line_pos, col_pos);
  vprintf(fmt, args);
  va_end (args);
}
void syntax_error(const char* fmt, ...){
  va_list args;
  va_start (args, fmt);
  fprintf(stderr, "[%s:%zu:%zu]: syntax error: ", file, line_pos, col_pos);
  vprintf(fmt, args);
  va_end (args);
  exit(1);
}
#endif

size_t test_ok_lvl = 0;
#define TEST_OK printf("TEST %zu: OK AT %s\n", ++test_ok_lvl, __FUNCTION__);
#define STR_CMP(a, b) (strcmp((a), (b)) == 0)

// table is a std::vector
#define  LOOK_UP_TABLE(table, field, param, else_ret)	\
  for(size_t i=0; i < (table).size(); ++i){		\
    if(!strcmp((table)[i]->field, param)){		\
      return (table)[i];				\
    }							\
  }							\
  return else_ret;

#endif /* __util */



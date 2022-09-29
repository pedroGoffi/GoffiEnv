#ifndef compiler
#define compiler
#include "./ast.cpp"
#include <cassert>
FILE* fd = stderr;
#define writef(...) fprintf(fd, __VA_ARGS__);
#define writefl(...) writef(__VA_ARGS__); writef("\n");

#endif /* compiler */

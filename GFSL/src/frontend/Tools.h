#ifndef LEXING_STAGE_H
#define LEXING_STAGE_H
#include <cassert>
#include <string>
#include <cstdio>
void usage(const char* program);
char *shift(int *argc, char ***argv);
bool is_number(const std::string& str);
#include "./Tools.cpp"
#include "./StringView.cpp"
#include "./CrossReference.cpp"
#endif /* ifndef LEXING_STAGE_H */

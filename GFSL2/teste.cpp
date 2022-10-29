#include <iostream>
#include "src/gfsl.cpp"
#include "src/token.cpp"
struct newtoken{
  TokenKind kind;
  int INT;
  const char* str;
};
int main() {
  printf("newtoken size = %zu\n", sizeof(struct newtoken));
  printf("|-> offset of kind = %zu\n", offsetof(newtoken, kind));
  printf("|-> offset of INT = %zu\n", offsetof(newtoken, INT));
  printf("|-> offset of str = %zu\n", offsetof(newtoken, str));
}

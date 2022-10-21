#ifndef __OPERAND__
#define __OPERAND__
#include "../ast.cpp"
enum OPERAND_kind{
  OPERAND_INT,
  OPERAND_REG,
  OPERAND_WORD
};
const char* OPERAND_kind_cstr(OPERAND_kind kind){
  switch(kind){
#define CASE(k) case k: return (#k);
    CASE(OPERAND_INT);
    CASE(OPERAND_REG);
    CASE(OPERAND_WORD);
#undef CASE
  }
  abort();
}
struct OPERAND{
  OPERAND_kind kind;
  union{
    int INT;
    struct{
      // word REG
      const char* wname;
      OPERAND*    operand;
    } word;
  };
};
OPERAND* OPERAND_new(OPERAND_kind kind){
  OPERAND* ret = new OPERAND;
  ret->kind = kind;
  return ret;
}
OPERAND* OPERAND_int(int val){
  OPERAND* ret = OPERAND_new(OPERAND_INT);
  ret->INT = val;
  return ret;
}

#endif /*__OPERAND__*/

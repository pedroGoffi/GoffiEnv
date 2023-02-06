#ifndef __preprocess
#define __preprocess
#include "ast.cpp"

void prep_expr()
{
}
void pre_process_stream()
{
  if(expect_name("sizeof")){
    prep_expr();  
  }
  else {
    prep_expr();
  }
}
#endif /*__preprocess*/

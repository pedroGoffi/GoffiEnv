#include "./src/gfi.cpp"

int main(int argc, char** argv)
{
  int exit_val = gfi_main(argc, argv);
  if (exit_val < 0){
    // TODO: report this better
    printf("Error: programa foi fechada de forma inesperada.\n");    
  }
  return exit_val;
}

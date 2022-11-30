#ifndef __resolve
#define __resolve
#include "./ast.cpp"

// compiler behaviour vars
size_t       rbp_stack_offset		= 0;
int          depth			= 0;
int          DEBUG_COUNT		= 0;
bool         load_vars			= true;
bool         ns_ctx			= false;
Decl*        current_decl		= NULL;	     
char*        ns_name			= NULL;
const        char* current_reasign_var	= NULL;
FILE*        output_file		= NULL;
Var**        local_vars			= NULL;
Var**        global_vars		= NULL;
const char** strings			= NULL;
Proc**       procs			= NULL;
#define DEBUG_OK							\
  if(0) printf("[%i]: PASSED BY %s\n", DEBUG_COUNT++, __FUNCTION__);
#define unimplemented(st)				\
  printf("UNIMEPLMENTED %s: %s\n", __FUNCTION__, #st);	\
  exit(1);
#define unreachable()					\
  printf("ERROR: unrechable %s.\n", __FUNCTION__);	\
  exit(1);



int align_to(int n, int align) {
  return (n + align - 1) / align * align;
}
Proc* Proc_get(Proc*** procs, const char* name){
  for(size_t i=0; i < buf__len(*procs); ++i){
    if(STR_CMP((*procs)[i]->name, name))
      return (*procs)[i];
  }
  return NULL;
}
Var* Var_get(Var*** vars, const char* name){
  for(size_t i=0; i < buf__len(*vars); ++i){
    if(STR_CMP((*vars)[i]->type_field->name, name)){
      return (*vars)[i];
    }
  }
  return NULL;
}
void  Proc_push(Proc*** procs, Decl* decl){
  assert(decl->kind == DECL_PROC);
  if(Proc_get(procs, decl->name)){
    fprintf(stderr,
	    "ERROR: the procedure '%s' was already declared.\n",
	    decl->name);
    exit(1);
  }

  buf__push(*procs, &decl->as.procDecl);
  
}

void Var_push(Var*** vars, Var* var){
  assert(!Var_get(vars, var->type_field->name));
  buf__push(*vars, var);
}

int count(void) {
  static int i = 1;
  return i++;
}
#define println(...)				\
  {						\
    fprintf(output_file, __VA_ARGS__);		\
    fprintf(output_file, "\n");			\
  }

#endif /* __resolve */

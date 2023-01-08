#ifndef __resolve
#define __resolve
#include "./ast.cpp"
#include "../../common/utils.cpp"
struct Compiled_struct{
  const char* name;
  size_t      total_alloc;
  Var**       vars;
  Proc**      procs;  
};
const char* built_in_kws[] = {
  "pass",	// general bultins
  "__print__",	// asm bultins
  "__asm__",
  "SYSCALL0",
  "SYSCALL1",
  "SYSCALL2",
  "SYSCALL3",
  "SYSCALL4",
  "SYSCALL5",
  "SYSCALL6",  
  "__set_acc",	// 3bc bultins
  "__writestr__",
};


bool is_builtin(const char* name){
  for(auto& str: built_in_kws){
    if(STR_CMP(name, str)) return true;
  }
  return false;
}
// compiler behaviour vars

extern Proc*       current_proc;
size_t             rbp_stack_offset	= 0;
int                depth		= 0;
int                DEBUG_COUNT		= 0;
bool               load_vars		= true;
bool               ns_ctx		= false;
Decl*              current_decl		= NULL;	     
char*              ns_name		= NULL;
const char*        current_reasign_var	= NULL;
FILE*              output_file		= NULL;
Var**              local_vars		= NULL;
Var**              global_vars		= NULL;
Typedef**          typedefs             = NULL;
const char**       strings		= NULL;
Proc**             procs		= NULL;
Compiled_struct**  Structs              = NULL;


Compiled_struct* Compiled_struct_get(const char* name){
  for(size_t i=0; i< buf__len(Structs); ++i){
    if(STR_CMP(Structs[i]->name, name))
      return Structs[i];
  }
  return NULL;
}
void current_proc_reserve_memory_for_compiled_struct(Compiled_struct* st)
{
  assert(current_proc);
  current_proc += st->total_alloc;
}
void Compiled_struct_push(Compiled_struct* st){
  assert(!Compiled_struct_get(st->name));
  buf__push(Structs, st);
}

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
Typedef* Typedef_get(Typedef*** tflist, const char* name){
  for(size_t i=0; i < buf__len(typedefs); ++i){
    if(STR_CMP(typedefs[i]->name, name)) return typedefs[i];
  }
  return NULL;
}
void Typedef_push(Typedef*** tflist, Typedef* tf){
  if(Typedef_get(tflist, tf->name)){
    fprintf(stderr,
	    "ERROR: compiler time type definition already declared '%s'.\n",
	    tf->name);
    exit(1);
  }
  buf__push(*tflist, tf);
}
void raise_if_typedef(Type** tptr){
  Type* type = Type_root(*tptr);
  bool as_cte = type->is_const;
  if(type->kind == TYPE_UNSOLVED){
    if(type->name){
      if(Typedef* tf = Typedef_get(&typedefs,
				   type->name)){
	printf("ty->name = %s\n", type->name);
	*type = *tf->type;
	type->is_const = as_cte;
	raise_if_typedef(&type);
      }
    }
  }
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
#define print(...)				\
  {						\
    fprintf(output_file, __VA_ARGS__);		\
  }
#endif /* __resolve */

#ifndef __c_compiler
#define __c_compiler
#include "./ast.cpp"
#include "./compiler.cpp"

void c_ident() {
  int i = 0;
  while (i < ident){
    print(" ");
    ++i;
  }
}

#define c_cls(...) \
  print("(");	   \
  __VA_ARGS__;   \
  print(")");

void c_block(StmtList* sl);
void c_type(Type* t){
  if(t->is_const){
    print("const ");
  }
  switch(t->kind){
  case TYPE_I64:  print("int");  return;
  case TYPE_CHAR: print("char"); return;
  case TYPE_NONE: print("void"); return;
  case TYPE_PTR:
    c_type(t->ptr.base);
    print("*");
    return;
  }
  assert(t->name);
  print(t->name);
}
void c_typefield(TypeField* tf){
  c_type(tf->type);
  print(" %s", tf->name);
}

void c_expr(Expr* e){
  switch(e->kind){
  case EXPRKIND_INT:
    print("%i", e->as.INT);
    break;
  case EXPRKIND_STRING_LITERAL: {
    const char* str = unescape_string(e->as.STRING);
    print("\"%s\"", str);
  } break;
  case EXPRKIND_NAME:
    if(Macro* m = GFL_Macros_find(e->name)){
      c_expr(m->expr);
      return;
    }
    print("%s", e->name);
    break;
  case EXPRKIND_LOCAL_IF:
    print("(");
    c_expr(e->as.local_if.cond);
    print(")? ");
    c_expr(e->as.local_if.if_body);
    print(": ");
    c_expr(e->as.local_if.else_body);
    break;

  case EXPRKIND_PROC_CALL: {
    Call* call = &e->as.call;

    print("%s(", call->p_name);
    for(size_t i=0; i< call->args_size; ++i){
      c_expr(call->args[i]);
      print(i + 1 < call->args_size? ", ": "");
    }
    print(")");
  } break;

  case EXPRKIND_ADDROF_NAME:
    print("&");
    c_expr(e->as.addr_of);
    break;
  case EXPRKIND_DERREF_NAME:
    print("*");
    c_expr(e->as.derref);
    break;
        
  case EXPRKIND_CAST:
    c_cls(c_type(e->as.cast.type->type););
    c_expr(e->as.cast.expr);
    break;

  case EXPR_COMPARASION:
    c_cls({
	c_expr(e->as.comparasion.lhs);
	print(" %s ", human_expr_cmp_kind(e->as.comparasion.op));
	c_expr(e->as.comparasion.rhs);
      });
    break;
  case EXPRKIND_ARRAY_ACCESS:
    print("%s[", e->as.ArrayAccess.name);
    c_expr(e->as.ArrayAccess.expr);
    print("]");
    break;
  case EXPR_BINARY_OP:
    c_cls({
	c_expr(e->as.BinaryOp.lhs);
	print(" %s ", human_expr_binary_op_kind(e->as.BinaryOp.op));
	c_expr(e->as.BinaryOp.rhs);
	
      });
    break;
  case EXPRKIND_REASIGN:
    c_expr(e->as.Reasign.from);
    print(" %s ", human_readable_tokenkind(e->as.Reasign.token.kind));
    c_expr(e->as.Reasign.to);    
    break;
  default:
    print_expr(e);
    fprintf(stderr,
	    "error: unexpected expr_kind.\n");
    exit(1);
  }
}
void c_stmt(Stmt* st){
  switch(st->kind){
  case STMTKIND_RETURN:
    c_ident();
    print("return ");
    c_expr(st->as.expr);
    print(";\n");
    break;
  
  case STMTKIND_EXPR:
    c_ident();
    c_expr(st->as.expr);
    print(";\n")
    break;
  
  
  case STMTKIND_LOCAL_VAR: {
    c_ident();
    Var* lv = st->as.var;
    c_typefield(lv->type_field);
    if(lv->expr){
      print(" = ");
      c_expr(lv->expr);
    }
    print(";\n");
  } break;
  case STMTKIND_IF:
    c_ident();
    
    print("if");
    c_cls(c_expr(st->as.__if.expr));
    print("{\n");
    ident++;
    c_block(st->as.__if.block);
    ident--;
    c_ident();
    print("}\n");
    break;
  

  case STMTKIND_WHILE:
      c_ident();
      print("while");
      c_cls(c_expr(st->as.__while.expr););
      print("{\n");
      ident++;
      c_block(st->as.__while.block);
      ident--;
      c_ident();
      print("}\n");
      break;
  default:
    fprintf(stderr,
	    "ERROR: c_stmt.\n");
    exit(1);
  }
}
void c_block(StmtList* sl){
  for(size_t i=0; i < sl->stmts_size; ++i){
    c_stmt(sl->stmts[i]);;
  }
}
void c_decl(Decl* decl){
  switch(decl->kind){
  case DECL_PROC: {
    Proc proc = decl->as.procDecl;
    if(!proc.block) return;
    c_type(proc.ret_type);
    print(" %s", proc.name);
    {
      print("(");
      ProcArgs* args = proc.args;
      // lp: last param
      size_t lp = (size_t)buf__len(args->vars);
      for(size_t i=0; i < lp; i++){
	Var* param = proc.args->vars[i];
	c_typefield(param->type_field);
	print(i + 1 < lp? ", ": "");	
      }
      print(")");
    }
    print("{\n");
    ident++;
    c_block(proc.block);
    ident--;
    print("}\n");
    break;
  }
  default:
    print_decl(decl);
    fatal("Unexpected c_decl.\n");
    exit(1);
  }
}
void assembly_c_ast(Decl** ast, const char* outfp){
  output_file = debug_mode? stdout: fopen(outfp, "wb");
  assert(output_file);
  const char* std_stuff[] = {
    "<iostream>",
    "<cstdio>",
    "<cassert>",
    "<cstring>"
  };
  for(auto& std: std_stuff){
    print("#include %s\n", std);
  }
  
  for(size_t i=0; i < buf__len(ast); ++i){
    c_decl(ast[i]);
  }

  fclose(output_file);
}
void compile_c(const char* file_path){
  if(debug_mode) return;

  char buff[256];
#define CMD(...)				\
  sprintf(buff, __VA_ARGS__);			\
  printf("[CMD]: %s\n", buff);			\
  system(buff);

  CMD("g++ %s -o %s.bin", file_path, file_path);
#undef CMD

}
#endif /* __c_compiler */

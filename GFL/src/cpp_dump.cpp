#ifndef __cpp_dump
#define __cpp_dump
#include "./ast.cpp"
FILE* fd = NULL;

inline void gen_type(Type* type);
inline void gen_stmt_list(StmtList* block);
void gen_expr(Expr* expr){
  switch(expr->kind){
  case EXPRKIND_INT:
    fprintf(fd, "%i", expr->as.INT);
    break;
  case EXPRKIND_NAME:
    fprintf(fd, "%s", expr->as.name);
    break;
  case EXPRKIND_PROC_CALL:
    fprintf(fd, "%s",expr->as.Call.p_name);
    {
      fprintf(fd, "(");
      for(size_t i=0;i<buf__len(expr->as.Call.args);++i){
	fprintf(fd, (i>0)?", ":"");
        gen_expr(expr->as.Call.args[i]);
      }
      fprintf(fd, ")");
    }
    break;
  case EXPRKIND_REASIGN:
    gen_expr(expr->as.Reasign.from);    
    fprintf(fd, "=");
    gen_expr(expr->as.Reasign.to);
    fprintf(fd, ";");    
    break;
  case EXPRKIND_CAST:
    fprintf(fd, "((");
    gen_type(expr->as.Cast.type->type);
    fprintf(fd, ")");
    gen_expr(expr->as.Cast.expr);
    fprintf(fd, ")");
    break;
  case EXPRKIND_STRING_LITERAL:
    fprintf(fd, "\"%s\"", expr->as.name);
    break;
  case EXPR_BINARY_OP:
    fprintf(fd, "(");
    gen_expr(expr->as.BinaryOp.lhs);
    fprintf(fd, "%c", expr->as.BinaryOp.op);
    gen_expr(expr->as.BinaryOp.rhs);    
    fprintf(fd, ")");
    break;
  case EXPR_COMPARASION:
    fprintf(fd, "((");
    gen_expr(expr->as.comparasion.lhs);
    fprintf(fd, ")%s(", human_readable_token(expr->as.comparasion.op));
    gen_expr(expr->as.comparasion.rhs);
    fprintf(fd, "))");    
    break;
  case EXPRKIND_ADDROF_NAME:
    fprintf(fd, "&(");
    gen_expr(expr->as.addr_of);
    fprintf(fd, ")");    
    break;
  case EXPRKIND_DERREF_NAME:
    fprintf(fd, "*(");
    gen_expr(expr->as.derref);
    fprintf(fd, ")");
    break;
  default:
    fprintf(stderr,
	    "ERROR: could not generate the expression.\n");
    exit(1);
  }
}
inline void gen_type(Type* type){
  if(type->is_const) fprintf(fd, "const ");
  switch(type->kind){
  case TypeKind::TYPESPEC_NAME:
    fprintf(fd, "%s", type->type_name);
    break;
  case TypeKind::TYPESPEC_PTR:
    gen_type(type->ptr_to);
    fprintf(fd, "*");
    break;
  case TypeKind::TYPESPEC_ARRAY:
    gen_type(type->array.type);
    fprintf(fd, "[");
    if(type->array.array_size)
      gen_expr(type->array.array_size);
    fprintf(fd, "]");
    break;
  case TypeKind::TYPESPEC_CONST:
    fprintf(fd, "const ");
    gen_type(type->as_const);
    break;

  case TypeKind::TYPESPEC_UNSOLVED:
    fprintf(stderr,
	    "ERROR: the compiler could not gen the automatic type for the variable '%s', please specify the type.\n",
	    "TODO: get the var name");
    exit(1);
    break;
  case TypeKind::TYPESPEC_NONE:
  default:
    fprintf(stderr, "ERROR: unexpected type->kind in gen_type.\n");
    exit(1);
  }
}
void gen_var(Var* var){
  gen_type(var->type);
  fprintf(fd, " %s", var->name);
  if(var->expr != nullptr){
    fprintf(fd, "=");
    gen_expr(var->expr);
  }
  fprintf(fd, ";\n");
}
inline void gen_typespec(TypeSpec* typespec){
  gen_type(typespec->type);
  fprintf(fd, " %s", typespec->name);
}
void gen_proc_args(ProcArgs* args){
  for(size_t i=0; i<args->argsList_size; ++i){
    fprintf(fd, i>0?", ":"");
    gen_typespec(args->argsList[i]);
  }
}

void gen_stmt(Stmt* stmt){
  switch(stmt->kind){
  case STMTKIND_RETURN:
    fprintf(fd, "return ");
    gen_expr(stmt->as.expr);
    fprintf(fd, ";\n");
    break;
  case STMTKIND_LOCAL_VAR:{
    gen_var(stmt->as.var);
  } break;
  case STMTKIND_EXPR:
    gen_expr(stmt->as.expr);
    fprintf(fd, ";\n");
    break;
  case STMTKIND_IF: {
    // if(!(expr)) goto END
    //
    fprintf(fd, "if(");
    gen_expr(stmt->as.__if.expr);
    fprintf(fd, "){\n");
    gen_stmt_list(stmt->as.__if.block);
    fprintf(fd, "}\n");
    if(stmt->as.__if.elif_nodes){
      for(size_t i = 0; i < stmt->as.__if.elif_nodes->nodes_size; ++i ){
	fprintf(fd, "else if (");
	gen_expr(stmt->as.__if.elif_nodes->node_expr[i]);
	fprintf(fd, "){");
	gen_stmt_list(stmt->as.__if.elif_nodes->node_block[i]);
	fprintf(fd, "}\n");	
      }
    }
    if(stmt->as.__if.else_block){
      fprintf(fd, "else{");
      gen_stmt_list(stmt->as.__if.else_block);
      fprintf(fd, "}\n");
    }
  } break;
  case STMTKIND_WHILE:{
    // TODO: pre-compile the label number for nested ifs

    fprintf(fd, "while(");
    gen_expr(stmt->as.__while.expr);
    fprintf(fd, "){\n");
    gen_stmt_list(stmt->as.__while.block);
    fprintf(fd, "}\n");
  } break;
  default:
    fprintf(stderr, "ERROR: could not generate the statement.\n");
    exit(1);
  }
}
inline void gen_stmt_list(StmtList* block){
  for(size_t i=0; i<block->stmts_size; ++i){
    gen_stmt(block->stmts[i]);
  }
}
void gen_cpp_from_ast_node(AST_NODE node){
  switch(node->kind){
  case DeclKind::DECLKIND_TYPEDEF:
    fprintf(fd, "typedef ");
    gen_type(node->as.Typedef.type);
    fprintf(fd, " ");
    gen_type(node->as.Typedef.type_equivalent);
    fprintf(fd, ";\n");
    break;
  case DeclKind::DECLKIND_VAR:{
    Var* var = new Var{
      node->name,
      node->as.varDecl.type,
      node->as.varDecl.expr
      
    };
    gen_var(var);
    break;
  }
  case DeclKind::DECLKIND_PROC:{
    printf("at declkind_proc\n");
    gen_type(node->as.procDecl.ret_type);
    fprintf(fd, " %s", node->name);
    { // gen the '(' args ')' stuff
      fprintf(fd, "(");
      gen_proc_args(node->as.procDecl.args);
      fprintf(fd, ")");
    }
    
    if(node->as.procDecl.block){ // gen the procedure body
      fprintf(fd, "{\n");
      gen_stmt_list(node->as.procDecl.block);    
      fprintf(fd, "}\n");
    } else {
      fprintf(fd, ";\n");
    }
  } break;
  case DeclKind::DECLKIND_ENUM:
  case DeclKind::DECLKIND_STRUCT:
  case DeclKind::DECLKIND_UNION:
  case DeclKind::DECLKIND_CIMPORT:
  case DeclKind::DECLKIND_NONE:
  default:
    fprintf(stderr, "ERROR: unexpected node->kind in gen_cpp_from_ast_node.\n");
    exit(1);
  }
}
void gen_cpp_file_from_ast(AST_ROOT ast, const char* output_fp)
{
  fd = fopen(output_fp, "wb");
  assert(fd);
  // todo some fun stuff
  fprintf(fd, "#include <iostream>\n");
  fprintf(fd, "#include <cstring>\n");
  fprintf(fd, "#include <cassert>\n");
    
  for(size_t i=0; i<buf__len(ast); ++i){
    gen_cpp_from_ast_node(ast[i]);
  }
  fclose(fd);
}
#endif /* __cpp_dump */

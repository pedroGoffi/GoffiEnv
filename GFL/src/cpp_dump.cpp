#ifndef __cpp_dump
#define __cpp_dump
#include "./ast.cpp"
FILE* fd = NULL;

inline void gen_type(Type* type);
inline void gen_stmt_list(StmtList* block);
const char* std_includes[] = {
  "<iostream>",
  "<cstring>",
  "<cassert>",
  "<cstdarg>",
  "<cstdint>",
  "<cstdlib>",  
  "\"std/gfl.cpp\""
};
#define genf(...) fprintf(fd, __VA_ARGS__)
void gen_expr(Expr* expr){
  switch(expr->kind){
  case EXPRKIND_INT:
    genf("%i", expr->as.INT);
    break;
  case EXPRKIND_NAME:
    genf("%s", expr->name);
    break;
  case EXPRKIND_PROC_CALL:
    genf("%s",expr->as.Call.p_name);
    {
      genf("(");
      for(size_t i=0;i<buf__len(expr->as.Call.args);++i){
	genf((i>0)?", ":"");
        gen_expr(expr->as.Call.args[i]);
      }
      genf(")");
    }
    break;
  case EXPRKIND_REASIGN:
    gen_expr(expr->as.Reasign.from);    
    genf("=");
    gen_expr(expr->as.Reasign.to);
    genf(";");    
    break;
  case EXPRKIND_CAST:
    genf("((");
    gen_type(expr->as.Cast.type->type);
    genf(")");
    gen_expr(expr->as.Cast.expr);
    genf(")");
    break;
  case EXPRKIND_STRING_LITERAL:
    genf("\"%s\"", expr->name);
    break;
  case EXPR_BINARY_OP:
    genf("(");
    gen_expr(expr->as.BinaryOp.lhs);
    genf("%s", human_expr_binary_op_kind(expr->as.BinaryOp.op));
    gen_expr(expr->as.BinaryOp.rhs);    
    genf(")");
    break;
  case EXPR_COMPARASION:
    genf("((");
    gen_expr(expr->as.comparasion.lhs);
    genf(")%s(", human_expr_cmp_kind(expr->as.comparasion.op));
    gen_expr(expr->as.comparasion.rhs);
    genf("))");    
    break;
  case EXPRKIND_ADDROF_NAME:
    genf("&(");
    gen_expr(expr->as.addr_of);
    genf(")");    
    break;
  case EXPRKIND_DERREF_NAME:
    genf("*(");
    gen_expr(expr->as.derref);
    genf(")");
    break;
  default:
    fprintf(stderr,
	    "ERROR: could not generate the expression.\n");
    exit(1);
  }
}
inline void gen_type(Type* type){
  switch(type->kind){
    
  case TypeKind::TYPE_I64:
    genf("int64_t");
    break;
  case TypeKind::TYPE_F64:
    genf("double");
    break;
    
  case TypeKind::TYPE_PTR:
    gen_type(type->ptr.base);
    genf("*");
    break;
  case TypeKind::TYPE_ARRAY:
    gen_type(type->array.base);
    genf("[");
    if(type->array.size){
      //gen_expr(type->array.array_size);
    }
    genf("]");
    break;

  case TypeKind::TYPE_UNSOLVED:
    fprintf(stderr,
	    "ERROR: the compiler could not gen the automatic type for the variable '%s', please specify the type.\n",
	    "TODO: get the var name");
    exit(1);
    break;
  case TypeKind::TYPE_NONE:
  default:
    fprintf(stderr, "ERROR: unexpected type->kind in gen_type.\n");
    exit(1);
  }
}
void gen_var(Var* var){
  gen_type(var->type_field->type);
  genf(" %s", var->type_field->name);
  if(var->expr){
    genf("=");
    gen_expr(var->expr);
  }
  genf(";\n");
}
inline void gen_typespec(TypeSpec* typespec){
  gen_type(typespec->type);
  genf(" %s", typespec->name);
}
void gen_proc_args(ProcArgs* args){
  for(size_t i=0; i<args->argsList_size; ++i){
    genf(i>0?", ":"");
    gen_typespec(args->argsList[i]);
  }
}

void gen_stmt(Stmt* stmt){
  switch(stmt->kind){
  case STMTKIND_RETURN:
    genf("return ");
    gen_expr(stmt->as.expr);
    genf(";\n");
    break;
  case STMTKIND_LOCAL_VAR:{
    gen_var(stmt->as.var);
  } break;
  case STMTKIND_EXPR:
    gen_expr(stmt->as.expr);
    genf(";\n");
    break;
  case STMTKIND_IF: {
    // if(!(expr)) goto END
    //
    genf("if(");
    gen_expr(stmt->as.__if.expr);
    genf("){\n");
    gen_stmt_list(stmt->as.__if.block);
    genf("}\n");
    if(stmt->as.__if.elif_nodes){
      for(size_t i = 0; i < stmt->as.__if.elif_nodes->nodes_size; ++i ){
	genf("else if (");
	gen_expr(stmt->as.__if.elif_nodes->node_expr[i]);
	genf("){");
	gen_stmt_list(stmt->as.__if.elif_nodes->node_block[i]);
	genf("}\n");	
      }
    }
    if(stmt->as.__if.else_block){
      genf("else{");
      gen_stmt_list(stmt->as.__if.else_block);
      genf("}\n");
    }
  } break;
  case STMTKIND_WHILE:{
    // TODO: pre-compile the label number for nested ifs

    genf("while(");
    gen_expr(stmt->as.__while.expr);
    genf("){\n");
    gen_stmt_list(stmt->as.__while.block);
    genf("}\n");
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
  case DeclKind::DECL_TYPEDEF:
    genf("typedef ");
    gen_type(node->as.Typedef.type);
    genf(" ");
    gen_type(node->as.Typedef.type_equivalent);
    genf(";\n");
    break;
  case DeclKind::DECL_VAR:{
    Var* var = new Var{
      node->as.varDecl.type_field,
      node->as.varDecl.expr
    };
    gen_var(var);
    break;
  }
  case DeclKind::DECL_PROC:{
    printf("at declkind_proc\n");
    gen_type(node->as.procDecl.ret_type);
    genf(" %s", node->name);
    { // gen the '(' args ')' stuff
      genf("(");
      gen_proc_args(node->as.procDecl.args);
      genf(")");
    }
    
    if(node->as.procDecl.block){ // gen the procedure body
      genf("{\n");
      gen_stmt_list(node->as.procDecl.block);    
      genf("}\n");
    } else {
      genf(";\n");
    }
  } break;
  case DeclKind::DECL_ENUM:
  case DeclKind::DECL_STRUCT:
  case DeclKind::DECL_UNION:
  case DeclKind::DECL_CIMPORT:
  default:
    fprintf(stderr, "ERROR: unexpected node->kind in gen_cpp_from_ast_node.\n");
    exit(1);
  }
}

void gen_cpp_file_from_ast(AST_ROOT ast, const char* output_fp)
{
  fd = fopen(output_fp, "wb");
  assert(fd);
  
  for(auto& inc: std_includes){
    genf("#include %s\n", inc);
  }
  for(size_t i=0; i<buf__len(ast); ++i){
    gen_cpp_from_ast_node(ast[i]);
  }
  fclose(fd);
}
#endif /* __cpp_dump */

#ifndef __resolve
#define __resolve
#include "./ast.cpp"
#include "../../GFM/src/machine.cpp"
#define GLOBAL_PROCS_CAPACITY 1024
#define GLOBAL_VARS_CAPACITY  1024
void AST_Compile_expr(Machine* mn, Expr* e);

void AST_Compile_cast(Machine* mn, TypeSpec* ts);
void AST_NODE_Compile_procedure_block(Machine* mn, StmtList* block);
void AST_Compile_expr(Machine* mn, Expr* e);
struct GLOBAL_PROC {
  InternStr    name;
  Inst_Addr    addr;
  size_t       arity;
};
struct GLOBAL_PROCS{
  GLOBAL_PROC procs[GLOBAL_PROCS_CAPACITY];
  size_t      procs_size;
  GLOBAL_PROCS() = default;
};
struct GLOBAL_VAR {
  InternStr name;
  size_t    loc;
  size_t    size;
};
struct GLOBAL_VARS {
  GLOBAL_VAR vars[GLOBAL_VARS_CAPACITY];
  size_t     vars_size;
  size_t     total_offset;
  GLOBAL_VARS() = default;
};

GLOBAL_PROCS global_procs;
GLOBAL_VARS  global_vars;
int GLOBAL_VARS_FIND(GLOBAL_VARS* gvars, const char* name){
  for(size_t i=0; i < gvars->vars_size; i++){
    if(strcmp(gvars->vars[i].name.str, name) == 0){
      return (int)gvars->vars[i].loc;
    }      
  }
  return -1;
}
void GLOBAL_VARS_PUSH(GLOBAL_VARS* gvars, const char* name, size_t size){
  assert(gvars->vars_size <= GLOBAL_VARS_CAPACITY);
  gvars->vars[gvars->vars_size++] = {
    .name = Internstr(name),
    .loc  = gvars->total_offset,
    .size = size
  };
  gvars->total_offset += size;
}

int GLOBAL_PROCS_FIND(GLOBAL_PROCS* gps, const char* name){
  for(size_t i=0; i < gps->procs_size; i++){
    if(strcmp(gps->procs[i].name.str, name) == 0){
      return (int)gps->procs[i].addr;
    }      
  }
  return -1;
}
void GLOBAL_PROCS_PUSH(GLOBAL_PROCS* gps, GLOBAL_PROC gp){
  assert(gps->procs_size + 1 < GLOBAL_PROCS_CAPACITY);
  gps->procs[gps->procs_size++] = gp;
  
}
void __procedure_entry_logic__(Machine* mn, AST_NODE node){
  /*
    See if procedure name located in node->name is already defined in 
    global_procs    
   */
  int last_proc_loc = GLOBAL_PROCS_FIND(&global_procs, node->name);
  if( last_proc_loc >= 0){
    printf("ERROR: can not rename an existent procedure.\n"
	   "NOTE: procedure name is: %s\n",
	   node->name);
    exit(1);
  }
  // TODO: the two options semms to make the same
  GLOBAL_PROC proc = {
    .name  = Internstr(node->name),
    .addr  = (Inst_Addr)mn->program_size,
    .arity = node->as.procDecl.args->argsList_size
  };
  if(strcmp(proc.name.str, "main") == 0){
    if(mn->has_entry){
      printf("ERROR: entry points can only happen in one procedure.\n");
      exit(1);
    }
    mn->has_entry   = true;   
    mn->entry_point = (int)mn->program_size;
  }
  GLOBAL_PROCS_PUSH(&global_procs, proc);
  // push a label with the addr and the name of the procedure
  Label_Table_push(&label_table, {
      .name = Internstr(node->name),
      .addr = Word_i64((int)mn->program_size)
    });
 
}
void EXPR_expect_arity(Expr* e, size_t arity){
  if(buf__len(e->as.Call.args) != arity){
    printf("ERROR: the procedure %s expects %zu arguments.\n",
	   e->as.Call.p_name, arity);
    exit(1);
  }
}
void PROC_expect_arity(const char* p_name, size_t arity){
  for(size_t i=0; i < global_procs.procs_size; i++){
    if(strcmp(global_procs.procs[i].name.str, p_name) != 0) continue;
    if(global_procs.procs[i].arity != arity){
      printf("ERROR: expected %zu numbers pr arguments for the procedure '%s' but got %zu.\n",
	      global_procs.procs[i].arity,
	      p_name,
	      arity);
      exit(1);
    }

  }
}
bool AST_Try_resolve_natives_expr(Machine* mn, Expr* e){
  const char* p_name = e->as.Call.p_name;
  Word native_loc = Native_Record_find(p_name);
  if(native_loc.as.i64 < 0){
    return false;
  }
  for(size_t i=0; i < buf__len(e->as.Call.args); ++i){      
    AST_Compile_expr(mn, e->as.Call.args[i]);
  }    
  Machine_push_inst(mn, {
      .type    = InstType::Native,
      .operand = native_loc
    });
  return true;
}
void AST_Compile_cast(Machine* mn, TypeSpec* ts){
  Type* type = ts->type;
  switch(type->kind){
  default:
    printf("ERROR: undefined behavior for casting to '%s'\n",
	   ts->name);
    exit(1);
  }
}
void AST_Compile_expr(Machine* mn, Expr* e){
  switch(e->kind){
  case ExprKind::EXPRKIND_INT: {
    Word op = Word_i64(e->as.INT);
    Machine_push_inst(mn, {
	.type    = InstType::Push,
	.operand = op
      });
  } break;
  case ExprKind::EXPRKIND_STRING_LITERAL: {
    size_t str_loc =
      Global_string_push(mn, "<string>", e->as.STRING);
    Machine_push_inst(mn, {
	.type    = InstType::Push,
	.operand = Word_i64((int)str_loc)
      });
  } break;
  case ExprKind::EXPRKIND_ADDROF_NAME: {
    AST_Compile_expr(mn, e->as.derref);
    const char* var_name = e->as.derref->as.name;
    int var_memory_loc = GLOBAL_VARS_FIND(&global_vars, var_name);
    if(var_memory_loc < 0){
      printf("ERROR: the variable '%s' was not declared in this scope.\n",
	     var_name);
      exit(1);
    }
    Machine_push_inst(mn, {
	.type    = InstType::Push,
	.operand = Word_i64(var_memory_loc)
      });
    Machine_push_inst(mn, {
	.type    = InstType::Load64,
	.operand = Word_i64(0)
      });
    Machine_push_inst(mn, {
	.type    = InstType::Get_addr,
	.operand = Word_i64(0)
      });
  } break;
  case ExprKind::EXPRKIND_DERREF_NAME: {
    AST_Compile_expr(mn, e->as.derref);

    //Machine_push_inst(mn, {
    //	.type    = InstType::Load64,
    //	.operand = Word_i64(0)
    //  });
    
  } break;
  case ExprKind::EXPRKIND_PROC_CALL: {
    if(!AST_Try_resolve_natives_expr(mn, e)){
      int proc_loc = GLOBAL_PROCS_FIND(&global_procs, e->as.Call.p_name);
      if(proc_loc < 0){
	printf("ERROR: '%s' was not declared in this scope.\n",
	       e->as.Call.p_name);
	exit(1);
      }
      {
	Machine_push_inst(mn, {
	    .type    = InstType::Call,
	    .operand = Word_i64(proc_loc)
	  });
      }
      
    }
  } break;
  case ExprKind::EXPRKIND_REASIGN: {
    if(1){
      AST_Compile_expr(mn, e->as.Reasign.from);
      AST_Compile_expr(mn, e->as.Reasign.to);
      Machine_push_inst(mn, {
	  .type    = InstType::Store64,
	  .operand = Word_i64(0)
	});
    }
    else if(e->as.Reasign.from->kind == EXPRKIND_NAME){    
      const char* var_name = e->as.Reasign.from->as.name;
      int var_memory_loc = GLOBAL_VARS_FIND(&global_vars, var_name);
      if(var_memory_loc < 0){
	printf("ERROR: the variable '%s' was not declared in this scope.\n",
	       var_name);
	exit(1);
      }
      Machine_push_inst(mn, {
	  .type    = InstType::Push,
	  .operand = Word_i64(var_memory_loc)
	});
      AST_Compile_expr(mn, e->as.Reasign.to);
      Machine_push_inst(mn, {
	  .type    = InstType::Store64,
	  .operand = Word_i64(0)
      });
    }
    
    else {
      printf("ERROR: the instruction is not implemented in EXPRKIND_REASIGN.\n");
      exit(1);
    }
  } break;
  case ExprKind::EXPRKIND_NAME: {
    int var_loc = GLOBAL_VARS_FIND(&global_vars,
				   e->as.name);
    if(var_loc >= 0){
      Machine_push_inst(mn,{
	  .type    = InstType::Push,
	  .operand = Word_i64(var_loc)
	});
      Machine_push_inst(mn, {
	  .type    = InstType::Load64,
	  .operand = Word_i64(0)
	});
    }
    else {
      printf("ERROR: undefined name '%s'\n", e->as.name);
      exit(1);
    }
  } break;
  case ExprKind::EXPRKIND_CAST: {
    AST_Compile_expr(mn, e->as.Cast.expr);
    //AST_Compile_cast(mn, e->as.Cast.type);
  } break;    
  case ExprKind::EXPR_BINARY_OP: {
    switch(e->as.BinaryOp.op){
    case EXPR_BINARY_OP_KIND::OP_KIND_PLUS: {
      AST_Compile_expr(mn, e->as.BinaryOp.lhs);
      AST_Compile_expr(mn, e->as.BinaryOp.rhs);
      Machine_push_inst(mn, {
	  .type    = InstType::Addi,
	  .operand = Word_i64(0)
	});
    } break;

    default:      
      printf("ERROR: unhandled EXPR BINARY OP KIND in: AST_Compile_expr\n");
      exit(1);
    }
    
  } break;
  case ExprKind::EXPR_COMPARASION: {

    // a < b

    AST_Compile_expr(mn, e->as.comparasion.lhs);
    AST_Compile_expr(mn, e->as.comparasion.rhs);
    Machine_push_inst(mn, {
	.type    = InstType::Cmp,
	.operand = Word_i64((int)e->as.comparasion.op)
      });
  } break;
  default:
    printf("ERROR: unexpected exprKind in: AST_Compile_expr\n");
    exit(1);
  }
}
void AST_Compile_stmt(Machine* mn, Stmt* st){
  switch(st->kind){
  case STMTKIND_WHILE: {
    Inst_Addr begin_loop = mn->program_size;
    AST_Compile_expr(mn, st->as.__while.expr);

    
    Machine_push_inst(mn, {
	.type    = InstType::Not,
	.operand = Word_i64(0)
      });
    
    Inst_Addr jmp_if_addr = Machine_push_inst(mn, {
	.type    = InstType::Jmp_if,
	.operand = Word_i64(0)
      });
    AST_NODE_Compile_procedure_block(mn, st->as.__while.block);
    Machine_push_inst(mn, {
	.type    = InstType::Jmp,
	.operand = Word_i64(begin_loop)
      });
    mn->program[jmp_if_addr].operand = Word_i64((int)mn->program_size);
  } break;
  case STMTKIND_RETURN:
    AST_Compile_expr(mn, st->as.expr);
    Machine_push_inst(mn, {
	.type    = InstType::Ret,
	.operand = Word_i64(0)
      });
    break;
  case STMTKIND_EXPR:
      AST_Compile_expr(mn, st->as.expr);
      break;
  case STMTKIND_IF: {

    {
      AST_Compile_expr(mn, st->as.__if.expr);
      Machine_push_inst(mn, {
	  .type   = InstType::Not,
	  .operand = Word_i64(0)
	});
      Inst_Addr if_jmp_addr = Machine_push_inst(mn, {
	  .type   = InstType::Jmp_if,
	  .operand = Word_i64(0)
	});
      AST_NODE_Compile_procedure_block(mn, st->as.__if.block);
      mn->program[if_jmp_addr].operand = Word_i64((int)mn->program_size);
      /*
	expr
	not
	jmp_if else
        else:
      */
    
      Inst_Addr **elif_list_addr = NULL;
    
      if(st->as.__if.elif_nodes){
	printf("ERROR: elif is not implemented yet.\n");
	exit(1);
      }
    
    
      if(st->as.__if.else_block){
	Inst_Addr skip_else_jmp_addr = Machine_push_inst(mn, {
	    .type = InstType::Jmp,
	    .operand = Word_i64(0)
	  });
	mn->program[if_jmp_addr].operand = Word_i64((int)mn->program_size);
	AST_NODE_Compile_procedure_block(mn, st->as.__if.else_block);
	mn->program[skip_else_jmp_addr].operand = Word_i64((int)mn->program_size);
      }
      for(size_t i=0; i < buf__len(elif_list_addr); ++i){
	mn->program[*elif_list_addr[i]].operand = Word_i64((int)mn->program_size);
      }
      
    }
  } break;
  default:
    printf("ERROR: unexpected STMTKIND at: AST_Compile_stmt\n");
    exit(1);
  }

}
void AST_NODE_Compile_procedure_block(Machine* mn, StmtList* block){
  for(size_t i=0; i < block->stmts_size; i++){
    AST_Compile_stmt(mn, block->stmts[i]);
  }
}
int Type_get_size(Type* type){
  if(strcmp(type->type_name, "i64") == 0){
    return sizeof(int64_t);
  }
  if(type->kind == TYPESPEC_PTR) {
    return sizeof(void*);
  }

  printf("ERROR: undefined type '%s'\n", type->type_name);
  exit(1);
}
void AST_ROOT_Compile(Machine* mn, AST_ROOT ast){
  Machine_push_all_natives(mn);
  mn->program_size = 1; // skip the first inst for the jmp
  for(size_t i=0; i<buf__len(ast); ++i){
    AST_NODE node = ast[i];
    switch(node->kind){
    case DECLKIND_VAR: {
      // TODO: check var requirements
      size_t var_size = Type_get_size(node->as.varDecl.type);
      GLOBAL_VARS_PUSH(&global_vars,
		       node->name,
		       var_size);
      if(node->as.varDecl.expr){
	printf("ERROR; global variable with expr is not implemented yet.\n");
	exit(1);
      }
    } break;
    case DECLKIND_PROC: {
      __procedure_entry_logic__(mn, node);
      // NOTE: type check the program
      AST_NODE_Compile_procedure_block(mn, node->as.procDecl.block);

    } break;
    default:
      error_here("some stuff");
    }       
  }
      
  Word main_loc = Label_Table_find(&label_table, Internstr("main"));
  if(main_loc.as.i64 < 0){
    printf("ERROR: the main procedure must be provided.\n");
    exit(1);
  }
  mn->program[0] = {
    .type    = InstType::Jmp,
    .operand = main_loc
  };
  Machine_push_inst(mn, {
      .type    = InstType::Halt,
      .operand = Word_i64(0)
    });
      
    
}

#endif /* __resolve */

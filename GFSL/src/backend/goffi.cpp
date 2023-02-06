#ifndef SRC_SRC_CODE
#define SRC_SRC_CODE 
#include "./Lexer.cpp"
#include "../frontend/Tools.cpp"
#include "Core/Instructions.cpp"
#include <ios>
#include <map>
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include "./Lexer.cpp"

static std::string current_proc_compiler;

#define MEMORY_SIZE 69000
#define RET_STACK_CAP 4096
#define wordName(num) "data__w"+std::to_string(num)
#define addrName(num) "__"+std::to_string(num)
#define makeLabel out << addrName(addrCount) << ":\n"; ++addrCount;

typedef uint64_t type;
static size_t addrCount = 0;

/*  
 *  This shall simulate the program
 *  using a likely virtual cpu
 *  stack beeing a vector and not std::stack
 *  for better control if i need in future
 *  simulation mode was been removed due to incompatiblity with compile_mode
 */
/*
 * This will compile the program to assembly x86_64
 */
namespace Goffi{
  void compile_program(std::vector<std::pair<VR, Token>>program, std::string outputFilePath){

    assert(NUM_OF_OPERANDS == 43 && "Exhaustive time handling operand, please update the compile_program in ");
  
    std::fstream out("out.asm", std::ios::out);
  
  
    /*  TODO:
     *      Include by ip
     */
    out << "BITS 64\n" 
  	<< "dump:\n"
  	<< "  push  rbp\n"
  	<< "  mov   rbp, rsp\n"
  	<< "  sub   rsp, 64\n"
  	<< "  mov   QWORD  [rbp-56], rdi\n"
  	<< "  mov   QWORD  [rbp-8], 1\n"
  	<< "  mov   eax, 32\n"
  	<< "  sub   rax, QWORD  [rbp-8]\n"
  	<< "  mov   BYTE  [rbp-48+rax], 10\n"
  	<< ".L2:\n"
  	<< "  mov   rcx, QWORD  [rbp-56]\n"
  	<< "  mov   rdx, -3689348814741910323\n"
  	<< "  mov   rax, rcx\n"
  	<< "  mul   rdx\n"
  	<< "  shr   rdx, 3\n"
  	<< "  mov   rax, rdx\n"
  	<< "  sal   rax, 2\n"
  	<< "  add   rax, rdx\n"
  	<< "  add   rax, rax\n"
  	<< "  sub   rcx, rax\n"
  	<< "  mov   rdx, rcx\n"
  	<< "  mov   eax, edx\n"
  	<< "  lea   edx, [rax+48]\n"
  	<< "  mov   eax, 31\n"
  	<< "  sub   rax, QWORD  [rbp-8]\n"
  	<< "  mov   BYTE  [rbp-48+rax], dl\n"
  	<< "  add   QWORD  [rbp-8], 1\n"
  	<< "  mov   rax, QWORD  [rbp-56]\n"
  	<< "  mov   rdx, -3689348814741910323\n"
  	<< "  mul   rdx\n"
  	<< "  mov   rax, rdx\n"
  	<< "  shr   rax, 3\n"
  	<< "  mov   QWORD  [rbp-56], rax\n"
  	<< "  cmp   QWORD  [rbp-56], 0\n"
  	<< "  jne   .L2\n"
  	<< "  mov   eax, 32\n"
  	<< "  sub   rax, QWORD  [rbp-8]\n"
  	<< "  lea   rdx, [rbp-48]\n"
  	<< "  lea   rcx, [rdx+rax]\n"
  	<< "  mov   rax, QWORD  [rbp-8]\n"
  	<< "  mov   rdx, rax\n"
  	<< "  mov   rsi, rcx\n"
  	<< "  mov   edi, 1\n"
  	<< "  mov   rax, 1\n"
  	<< "  syscall\n"
  	<< "  nop\n"
  	<< "  leave\n"
  	<< "  ret\n"
	<<  "global _start\n"
	<<  "segment .text\n"	
      ;
    std::vector<std::pair<VR, Token>>::iterator ip = program.begin();
  
    while( ip != program.end()){
      switch(ip->first.op){
      case DUMP_STACK:
	++ip;
	break;
      case PUSH_LOCAL_MEM:
	makeLabel;

	out
	    <<  "    mov rax, [ret_stack_rsp]\n"
	    <<  "    add rax, " << local_memories[ip->first.op_string].second  << "\n"
	    <<  "    push rax\n"
	  ;

	
	
	++ip;
	break;
      case ALLOC_LOCAL_MEM:
	// This only exits in parsing stage
	makeLabel;
	makeLabel;
	


	++ip;
	break;
      case CALL_PROC:
	{

	  makeLabel;
	  out
	      <<  "   mov rax, rsp\n"
	      <<  "   mov rsp, [ret_stack_rsp]\n"
	      <<  "   call proc_" << ip->first.op_string << "\n"	
	      <<  "   mov [ret_stack_rsp], rsp\n"
	      <<  "   mov rsp, rax\n"	    
	    ;
	  ++ip;
	  break;
	}
      case PROC_LEAVE:
	makeLabel;

	++ip;
	break;
      case PROC_RETURN:
	{
	  makeLabel;
	  out 
	      <<  "   mov rax, rsp\n"
	      <<  "   mov rsp, [ret_stack_rsp]\n"
	      <<  "   add rsp, " << proc_need_allocate[current_proc_compiler] << "\n"
	      <<  "   ret\n"
	    ;
	  ++ip;
	  break;
	}
      case PROC_ENTRY:
	{
	  makeLabel;	  
	  out <<  "proc_"<<  ip->first.op_string << ":\n"	    
	      <<  "   sub rsp, " << proc_need_allocate[ip->first.op_string] << "\n" // TODO: get all value to allocate
	      <<  "   mov [ret_stack_rsp], rsp\n"
	      <<  "   mov rsp, rax\n"
	    ;
	  current_proc_compiler = ip->first.op_string;
	  makeLabel;

	  
	  ++ip;
	  break;
	}
      case PROC_SKIP:
	{
	  makeLabel;	
	  ++ip;
	  break;
	}
      case OP_GTHAN:
	makeLabel;
	out 
	    <<  "   mov rcx, 0\n"
	    <<  "   mov rdx, 1\n"
	    <<  "   pop rbx\n"
	    <<  "   pop rax\n"
	    <<  "   cmp rax, rbx\n"
	    <<  "   cmovg rcx, rdx\n"
	    <<  "   push rcx\n"
	  ;
	++ip;
	break;
      case DUP:
	makeLabel;
	out
	    <<  "   pop rax\n"
	    <<  "   push rax\n"
	    <<  "   push rax\n"
	  ;
	++ip;
	break;
      case OP_STOREBYTE:
	{
  
	  makeLabel;
	  switch(ip->first.operand){
	  case (8):
	    out 
		<<  "   pop rbx\n"
		<<  "   pop rax\n"
		<<  "   mov BYTE [rax], bl\n"
	      ;
	    break;
	  case (64):
	    out 
		<<	"   pop rbx\n"
		<<	"   pop rax\n"
		<<	"   mov [rax], rbx\n"
	      ;
	    break;
	  }
	  ++ip;
	  break;
	}
      case OP_LOADBYTE: {
	makeLabel;
	switch(ip->first.operand){
	case (8):
	  out 
	      <<  "   pop rax\n"
	      <<  "   xor rbx, rbx\n"
	      <<  "   mov bl, BYTE [rax]\n"
	      <<  "   push rbx\n"
	    ;
	  break;
	case (64):
	  out 
	      <<	"   pop rax\n"
	      <<	"   xor rbx, rbx\n"
	      <<	"   mov rbx, [rax]\n"
	      <<	"   push rbx\n"
	    ;
	  break;
	}
	++ip;
	break;
      }
      case OP_MEM:
	makeLabel;
	out 
	    <<  "   push buffer\n"
	  ;
	++ip;
	break;
      case ARGC:
	makeLabel;
	out
	    <<	"   mov rax, [args_ptr]\n"
	    <<	"   push rax\n"
	  ;
	++ip;
	break;
      case ARGV:
	makeLabel;
	out 
	    <<	"   mov rax, [args_ptr]\n"
	    <<	"   add rax, 8\n"
	    <<	"   push rax\n"
	  ;
	++ip;
	break;
      case OP_LTHAN:
	makeLabel;
	out

	  ;
	++ip;
	break;
      case OP_DO: 
	makeLabel;
	out 
	    <<  "   pop rax\n"
	    <<  "   test rax, rax\n"
	    <<  "   jz " << addrName(ip->first.operand)  << "\n"                
	  ;
	++ip;
	break;
      case OP_END:
	makeLabel;
	out 
	  <<	"   jmp ";
	(ip->first.operand > 0)
	  ? out << addrName(ip->first.operand)
	  : out <<  addrName(addrCount);
	out << "\n";
  
	++ip;
	break;
      case OP_EQUALS:
	makeLabel;
	out
	    <<  "   mov rcx, 0\n"
	    <<  "   mov rdx, 1\n"
	    <<  "   pop rax\n"
	    <<  "   pop rbx\n"
	    <<  "   cmp rax,rbx\n"
	    <<  "   cmove rcx, rdx\n"
	    <<  "   push rcx\n"
	  ;
	++ip;
	break;
      case OP_NOT_EQUALS:
	makeLabel;
	out
	    <<	"   mov rcx, 0\n"       
	    <<  "   mov rdx, 1\n"       
	    <<  "   pop rbx\n"          
	    <<  "   pop rax\n"          
	    <<  "   cmp rax, rbx\n"     
	    <<  "   cmovne rcx, rdx\n"  
	    <<  "   push rcx\n"   
	  ;
	++ip;
	break;
  
      case PUSH_INT:
	{
	  makeLabel;
	  out
	      <<  "   push 0x" << std::hex << (int)ip->first.operand << std::dec <<"\n"
	    ;
	  ++ip;
	} break;
      case PUSH_PTR:
	{
	  makeLabel;
	  out
	      <<	"   mov rax, buffer\n"
	      <<	"   add rax, "  << static_addresses[ip->first.op_string]<< "\n" 
	      <<	"   push rax\n"
	    ;
	  ++ip;
	} break;
      case PUSH_STR:
	makeLabel;
	out
	    <<	"   mov rax, "		<< ip->first.op_string.length() << "\n"
	    <<	"   push rax\n"
	    <<	"   push " << wordName(ip->first.operand) << "\n"
	  ;
	++ip;
	break;
      case OP_DIVMOD: 
	makeLabel;
	out
	    <<	"   xor rdx, rdx\n"
	    <<	"   pop rbx\n"
	    <<	"   pop rax\n"
	    <<	"   div rbx\n"
	    <<	"   push rax\n"
	    <<	"   push rdx\n"
	  ;
	++ip;
	break;
  
      case OP_IDIVMOD: 
	makeLabel;
	out 
	    <<	"   pop rbx\n"
	    <<	"   pop rax\n"
	    <<	"   cqo rbx\n"
	    <<	"   idiv rbx\n"
	    <<	"   push rax\n"
	    <<	"   push rdx\n"
	  ;
	++ip;
	break;
      case OP_PLUS:{
	makeLabel;
	out
	    <<  "   pop rbx\n"
	    <<  "   pop rax\n"
	    <<  "   add rax, rbx\n"
	    <<  "   push rax\n"
	  ;
	++ip;
	break;
      }
      case OP_MULT:
	makeLabel;
	out
	    <<	"   pop rax\n"             
	    <<  "   pop rbx\n"             
	    <<  "   mul rbx\n"             
	    <<  "   push rax\n" 
	  ;
	++ip;
	break;
      case OP_MINUS:
	makeLabel;
	out
	    <<  "   pop rax\n"
	    <<  "   pop rbx\n"
	    <<  "   sub rbx,rax\n"
	    <<  "   push rbx\n"
	  ;
	++ip;
	break;
      case DUMP:{
	makeLabel;
	out 
	    <<  "   pop rdi\n"
	    <<  "   call dump\n"
	  ;
	++ip;
	break;
      }
      case DROP:
	makeLabel;
	out
	    <<	"   pop rax\n"
	  ;
	++ip;
	break;
      case SWAP:
	makeLabel;
	out 
	    <<	"   pop rax\n"
	    <<	"   pop rbx\n"
	    <<	"   push rax\n"
	    <<	"   push rbx\n"
	  ;
	++ip;
	break;
      case OVER:
	makeLabel;
	out
	    <<	"   pop rax\n"
	    <<	"   pop rbx\n"
	    <<	"   push rbx\n"
	    <<	"   push rax\n"
	    <<	"   push rbx\n"
	  ;
	++ip;
	break;
      case ROT:
  
	makeLabel;
	out 
	    <<	"   pop rax\n"
	    <<	"   pop rbx\n"
	    <<	"   pop rcx\n"
  
	    <<	"   push rbx\n"
	    <<	"   push rax\n"
	    <<	"   push rcx\n"
	  ;
	++ip;
	break;
      case OP_IF:
	makeLabel;

	++ip;
	break;
      case OP_ELSE:
	makeLabel;
	out 
	  <<	"   jmp " << addrName(ip->first.operand)  << "\n"                
	  ;
	++ip;
	break;
      case OP_WHILE:
	makeLabel;

	++ip;
	break;
  
      case OP_SYSCALL:
	makeLabel;

	switch( ip->first.operand ){
	case 0:
	  out  
	      << "    pop rax\n"             
	      << "    syscall\n"             
	      << "    push rax\n"            
	    ;
	  break;
  
	case 1:
	  out
	      <<  "    pop rax\n"             
	      <<  "    pop rdi\n"             
	      <<  "    syscall\n"             
	      <<  "    push rax\n"            
	    ;
	  break;
	case 2:
	  out 
	      <<  "    pop rax\n"             
	      <<  "    pop rdi\n"             
	      <<  "    pop rsi\n"             
	      <<  "    syscall\n"             
	      <<  "    push rax\n"            
	    ;
	  break;
	case 3:
	  out
	      <<  "    pop rax\n"             
	      <<  "    pop rdi\n"             
	      <<  "    pop rsi\n"             
	      <<  "    pop rdx\n"             
	      <<  "    syscall\n"             
	      <<  "    push rax\n"            
	    ;
	  break;
	case 4:
	  out 
	      <<  "    pop rax\n"             
	      <<  "    pop rdi\n"             
	      <<  "    pop rsi\n"             
	      <<  "    pop rdx\n"             
	      <<  "    pop r10\n"             
	      <<  "    syscall\n"             
	      <<  "    push rax\n"            
	    ;
	  break;
	case 5:
	  out 
	      <<  "    pop rax\n"             
	      <<  "    pop rdi\n"             
	      <<  "    pop rsi\n"             
	      <<  "    pop rdx\n"             
	      <<  "    pop r10\n"             
	      <<  "    pop r8\n"              
	      <<  "    syscall\n"             
	      <<  "    push rax\n"            
	    ;
	  break;
	case 6:		    
	  out	
		<<  "    pop rax\n"             
		<<  "    pop rdi\n"             
		<<  "    pop rsi\n"             
		<<  "    pop rdx\n"             
		<<  "    pop r10\n"             
		<<  "    pop r8\n"              
		<<  "    pop r9\n"              
		<<  "    syscall\n"             
		<<  "    push rax\n"            
	    ;
	  break;
	}
	++ip;
	break;
      case CAST:
	{
	  makeLabel;
	  ++ip;
	  break;
	}
      case SHIFT_LEFT:
	makeLabel;
	out
	    <<	"   pop rcx\n"             
	    <<	"   pop rbx\n"             
	    <<	"   shl rbx, cl\n"         
	    <<	"   push rbx\n"  
	  ;
	++ip;
	break;
  
      case SHIFT_RIGHT:
	makeLabel;
	out 
	    <<	"   pop rcx\n"     
	    <<	"   pop rbx\n"     
	    <<	"   shr rbx, cl\n" 
	    <<	"   push rbx\n"        
	  ;
	++ip;
	break;
      case OR:
	makeLabel;
	out 
	    <<  "   pop rax\n"            
	    <<  "   pop rbx\n"            
	    <<  "   or rbx, rax\n"        
	    <<  "   push rbx\n"           
	  ;
	++ip;
	break;
      case AND:
	makeLabel;
	out
	    <<  "   pop rax\n"            
	    <<  "   pop rbx\n"            
	    <<  "   and rbx, rax\n"       
	    <<  "   push rbx\n"           
	  ;
	++ip;
	break;
      case NOT:
	makeLabel;
	out 
	    <<  "   pop rax\n"            
	    <<  "   not rax\n"            
	    <<  "   push rax\n"    
	  ;
	++ip; 
	break;
      case NUM_OF_OPERANDS: break;
      default:
	{
	  assert(false && "WARNING! Unreachable operations in compile mode\n");
	  ++ip;
	  break;
	}
      }
    }

    out    <<  "_start:\n"
	   <<  "   mov [args_ptr], rsp\n"
	   <<  "   mov rax, ret_stack_end\n"
	   <<  "   mov [ret_stack_rsp], rax\n"
	   <<  "   call proc_main\n";
    makeLabel;
    out      
	   <<  "   mov rax, 60\n"
	   <<  "   mov rdi, 0\n"
	   <<  "   syscall\n"
      ;

    if( Words.empty() == 0 ){
      out <<  "segment .data\n";
      uint64_t words_count{0};
      for(uint64_t x = 0; x != Words.size(); ++x){
  	out << wordName(x) << ": db\t" << Words[x]<< ",0\n";
  	++words_count;
      }
    }
    out	<<  "segment .bss\n"

      ;
    
    out <<  "args_ptr:	     resq 1\n"
      	<<  "ret_stack_rsp:  resq 1\n"
	<<  "ret_stack:      resb " << RET_STACK_CAP << "\n"
	<<  "ret_stack_end:  resq 1\n"
      	<<  "buffer:    resb    " << static_mem_capacity << "\n"
      ;
    out.close();
    char cmd[128];
    sprintf(cmd, "nasm -felf64 -g out.asm");
    system(cmd);
  
    sprintf(cmd, "ld out.o -o %s", outputFilePath.c_str());
    system(cmd);
    system("rm out.o");
  
    //sprintf(cmd, "rm out.o");
    //system(cmd);
  
  }
}
#endif /* ifndef SRC_SRC_CODE */

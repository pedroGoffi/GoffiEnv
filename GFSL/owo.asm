BITS 64
dump:
  push  rbp
  mov   rbp, rsp
  sub   rsp, 64
  mov   QWORD  [rbp-56], rdi
  mov   QWORD  [rbp-8], 1
  mov   eax, 32
  sub   rax, QWORD  [rbp-8]
  mov   BYTE  [rbp-48+rax], 10
.L2:
  mov   rcx, QWORD  [rbp-56]
  mov   rdx, -3689348814741910323
  mov   rax, rcx
  mul   rdx
  shr   rdx, 3
  mov   rax, rdx
  sal   rax, 2
  add   rax, rdx
  add   rax, rax
  sub   rcx, rax
  mov   rdx, rcx
  mov   eax, edx
  lea   edx, [rax+48]
  mov   eax, 31
  sub   rax, QWORD  [rbp-8]
  mov   BYTE  [rbp-48+rax], dl
  add   QWORD  [rbp-8], 1
  mov   rax, QWORD  [rbp-56]
  mov   rdx, -3689348814741910323
  mul   rdx
  mov   rax, rdx
  shr   rax, 3
  mov   QWORD  [rbp-56], rax
  cmp   QWORD  [rbp-56], 0
  jne   .L2
  mov   eax, 32
  sub   rax, QWORD  [rbp-8]
  lea   rdx, [rbp-48]
  lea   rcx, [rdx+rax]
  mov   rax, QWORD  [rbp-8]
  mov   rdx, rax
  mov   rsi, rcx
  mov   edi, 1
  mov   rax, 1
  syscall
  nop
  leave
  ret
global _start
segment .text
.L3:
.P0:
   mov [ret_stack_rsp], rsp
   mov rsp, rax
.L4:
   jmp L5
.L5:
.P1:
   mov [ret_stack_rsp], rsp
   mov rsp, rax
.L6:
   push buffer
.L7:
   mov rax, global_mem
   add rax, 0
   push rax
.L8:
   pop rbx
   pop rax
   add rax, rbx
   push rax
.L9:
   jmp L10
_start:
    mov [args_ptr], rsp
    mov rax, ret_stack_end
    mov [ret_stack_rsp], rax
    call P1
.L10:
    mov rax, 60
    mov rsi, 0
    syscall
segment .bss
args_ptr:	   resq 1
ret_stack_rsp:  resq 1
ret_stack:      resb 4096
ret_stack_end:  resq 1
global_mem: resb 10

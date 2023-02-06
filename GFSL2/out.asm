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
.dump_loop:
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
	jne   .dump_loop
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
P0: ;; Procedure 'fputs'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 17]
	;;START MACRO: SYS_write, of length: 1
.L1:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
	;;END MACRO: SYS_write
.L2:	;; [INFO]: (INST)[type = SYSCALL3, operand = 0]
	pop rax
	pop rdi
	pop rsi
	pop rdx
	syscall
	push rax
.L3:	;; [INFO]: (INST)[type = DROP, operand = 0]
	pop rax
.L4:	;; [INFO]: return from fputs
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
P1: ;; Procedure 'eputs'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_INT, operand = 2]
	push 2
.L1:	;; [INFO]: (INST)[type = CALL_PROC, operand = 0]
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	call P0
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L2:	;; [INFO]: return from eputs
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
P2: ;; Procedure 'puts'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
.L1:	;; [INFO]: (INST)[type = CALL_PROC, operand = 0]
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	call P0
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L2:	;; [INFO]: return from puts
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
P3: ;; Procedure 'exit'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_INT, operand = 60]
	push 60
.L1:	;; [INFO]: (INST)[type = SYSCALL1, operand = 0]
	pop rax
	pop rdi
	syscall
	push rax
.L2:	;; [INFO]: (INST)[type = DROP, operand = 0]
	pop rax
.L3:	;; [INFO]: return from exit
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
P4: ;; Procedure 'main'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_STR, operand = 0]
	push 14
	push Data0
.L1:	;; [INFO]: (INST)[type = CALL_PROC, operand = 2]
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	call P2
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L2:	;; [INFO]: return from main
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
_start:
	mov [args_ptr], rsp
	mov rax, ret_stack_end
	mov [ret_stack_rsp], rax
	call P4
	mov rdi, 0
	mov rax, 60
	syscall
segment .data
Data0: db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 10, 0, 
segment .bss
args_ptr: resq 1 
ret_stack_rsp: resq 1 
ret_stack: resb 4096
ret_stack_end: resq 1 

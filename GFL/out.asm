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
P0: ;; Procedure 'sex'
	sub rsp, 8
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L1:	;; [INFO]: (INST)[type = SWAP, operand = 0]
	pop rax
	pop rbx
	push rax
	push rbx
.L2:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
.L3:	;; [INFO]: return from sex
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	add rsp, 8
	ret
P1: ;; Procedure 'main'
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_STATIC_PTR, operand = 0]
	push _STATIC0
.L1:	;; [INFO]: (INST)[type = PUSH_INT, operand = 10]
	push 10
.L2:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
.L3:	;; [INFO]: (INST)[type = PUSH_STATIC_PTR, operand = 0]
	push _STATIC0
.L4:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L5:	;; [INFO]: (INST)[type = DUMP, operand = 0]
	pop rdi
	call dump
.L6:	;; [INFO]: return from main
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	ret
_start:
	mov [args_ptr], rsp
	mov rax, ret_stack_end
	mov [ret_stack_rsp], rax
	call P1
	mov rdi, 0
	mov rax, 60
	syscall
segment .bss
args_ptr: resq 1 
ret_stack_rsp: resq 1 
ret_stack: resb 4096
ret_stack_end: resq 1 
_STATIC0: resb 8

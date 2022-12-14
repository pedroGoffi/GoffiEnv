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
.L0:	;; [INFO]: (INST)[type = PUSH_STR, operand = 0]
	push 12
	push STR0
P4: ;; Procedure 'str!'
	sub rsp, 129
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L1:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 339]
	;;START MACRO: rstore, of length: 2
.L2:	;; [INFO]: (INST)[type = SWAP, operand = 0]
	pop rax
	pop rbx
	push rax
	push rbx
.L3:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
	;;END MACRO: rstore
.L4:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 65]
	mov rax, [ret_stack_rsp]
	add rax, 65
	push rax
.L5:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 339]
	;;START MACRO: rstore, of length: 2
.L6:	;; [INFO]: (INST)[type = SWAP, operand = 0]
	pop rax
	pop rbx
	push rax
	push rbx
.L7:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
	;;END MACRO: rstore
.L8:	;; [INFO]: (INST)[type = DUP, operand = 0]
	pop rax
	push rax
	push rax
.L9:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 342]
	;;START MACRO: str-len, of length: 2
.L10:	;; [INFO]: (INST)[type = PUSH_INT, operand = 0]
	push 0
.L11:	;; [INFO]: (INST)[type = PLUS, operand = 0]
	pop rbx
	pop rax
	add rax, rbx
	push rax
	;;END MACRO: str-len
.L12:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 65]
	mov rax, [ret_stack_rsp]
	add rax, 65
	push rax
.L13:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L14:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
.L15:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 343]
	;;START MACRO: str-data, of length: 2
.L16:	;; [INFO]: (INST)[type = PUSH_INT, operand = 8]
	push 8
.L17:	;; [INFO]: (INST)[type = PLUS, operand = 0]
	pop rbx
	pop rax
	add rax, rbx
	push rax
	;;END MACRO: str-data
.L18:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L19:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L20:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
.L21:	;; [INFO]: return from str!
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	add rsp, 129
	ret
P5: ;; Procedure 'get-file-text'
	sub rsp, 170
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L1:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L2:	;; [INFO]: (INST)[type = SWAP, operand = 0]
	pop rax
	pop rbx
	push rax
	push rbx
.L3:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
.L4:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 26]
	mov rax, [ret_stack_rsp]
	add rax, 26
	push rax
.L5:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L6:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L7:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 334]
	;;START MACRO: fstat, of length: 2
.L8:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 21]
	;;START MACRO: SYS_fstat, of length: 1
.L9:	;; [INFO]: (INST)[type = PUSH_INT, operand = 5]
	push 5
	;;END MACRO: SYS_fstat
.L10:	;; [INFO]: (INST)[type = SYSCALL2, operand = 0]
	pop rax
	pop rdi
	pop rsi
	syscall
	push rax
	;;END MACRO: fstat
.L11:	;; [INFO]: (INST)[type = DROP, operand = 0]
	pop rax
.L12:	;; [INFO]: (INST)[type = PUSH_INT, operand = 0]
	push 0
.L13:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 0]
	mov rax, [ret_stack_rsp]
	add rax, 0
	push rax
.L14:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L15:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 12]
	;;START MACRO: MAP_PRIVATE, of length: 1
.L16:	;; [INFO]: (INST)[type = PUSH_INT, operand = 2]
	push 2
	;;END MACRO: MAP_PRIVATE
.L17:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 13]
	;;START MACRO: PROT_READ, of length: 1
.L18:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
	;;END MACRO: PROT_READ
.L19:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 26]
	mov rax, [ret_stack_rsp]
	add rax, 26
	push rax
.L20:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 353]
	;;START MACRO: stat-st_size, of length: 2
.L21:	;; [INFO]: (INST)[type = PUSH_INT, operand = 48]
	push 48
.L22:	;; [INFO]: (INST)[type = PLUS, operand = 0]
	pop rbx
	pop rax
	add rax, rbx
	push rax
	;;END MACRO: stat-st_size
.L23:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L24:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 3]
	;;START MACRO: NULL, of length: 1
.L25:	;; [INFO]: (INST)[type = PUSH_INT, operand = 0]
	push 0
	;;END MACRO: NULL
.L26:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 337]
	;;START MACRO: mmap, of length: 2
.L27:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 25]
	;;START MACRO: SYS_mmap, of length: 1
.L28:	;; [INFO]: (INST)[type = PUSH_INT, operand = 9]
	push 9
	;;END MACRO: SYS_mmap
.L29:	;; [INFO]: (INST)[type = SYSCALL6, operand = 0]
	pop rax
	pop rdi
	pop rsi
	pop rdx
	pop r10
	pop r8
	pop r9
	syscall
	push rax
	;;END MACRO: mmap
.L30:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 26]
	mov rax, [ret_stack_rsp]
	add rax, 26
	push rax
.L31:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 353]
	;;START MACRO: stat-st_size, of length: 2
.L32:	;; [INFO]: (INST)[type = PUSH_INT, operand = 48]
	push 48
.L33:	;; [INFO]: (INST)[type = PLUS, operand = 0]
	pop rbx
	pop rax
	add rax, rbx
	push rax
	;;END MACRO: stat-st_size
.L34:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L35:	;; [INFO]: (INST)[type = SWAP, operand = 0]
	pop rax
	pop rbx
	push rax
	push rbx
.L36:	;; [INFO]: (INST)[type = CAST_PTR, operand = 0]
.L37:	;; [INFO]: return from get-file-text
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	add rsp, 170
	ret
P6: ;; Procedure 'main'
	sub rsp, 25
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L0:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 17]
	mov rax, [ret_stack_rsp]
	add rax, 17
	push rax
.L1:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 5]
	;;START MACRO: O_READONLY, of length: 1
.L2:	;; [INFO]: (INST)[type = PUSH_INT, operand = 0]
	push 0
	;;END MACRO: O_READONLY
.L3:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
.L4:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 338]
	;;START MACRO: nth-argv, of length: 5
.L5:	;; [INFO]: (INST)[type = PUSH_INT, operand = 8]
	push 8
.L6:	;; [INFO]: (INST)[type = MULT, operand = 0]
	pop rax
	pop rbx
	mul rbx
	push rax
.L7:	;; [INFO]: (INST)[type = ARGV, operand = 0]
	mov rax, [args_ptr]
	add rax, 8
	push rax
.L8:	;; [INFO]: (INST)[type = PLUS, operand = 0]
	pop rbx
	pop rax
	add rax, rbx
	push rax
.L9:	;; [INFO]: (INST)[type = CAST_PTR, operand = 0]
	;;END MACRO: nth-argv
.L10:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 359]
	;;START MACRO: fopen, of length: 4
.L11:	;; [INFO]: (INST)[type = LOAD64, operand = 0]
	pop rax
	xor rbx, rbx
	mov rbx, [rax]
	push rbx
.L12:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 10]
	;;START MACRO: AT_FDCWD, of length: 3
.L13:	;; [INFO]: (INST)[type = PUSH_INT, operand = 0]
	push 0
.L14:	;; [INFO]: (INST)[type = PUSH_INT, operand = 100]
	push 100
.L15:	;; [INFO]: (INST)[type = MINUS, operand = 0]
	pop rax
	pop rbx
	sub rbx, rax
	push rbx
	;;END MACRO: AT_FDCWD
.L16:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 333]
	;;START MACRO: openat, of length: 2
.L17:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 273]
	;;START MACRO: SYS_openat, of length: 1
.L18:	;; [INFO]: (INST)[type = PUSH_INT, operand = 257]
	push 257
	;;END MACRO: SYS_openat
.L19:	;; [INFO]: (INST)[type = SYSCALL3, operand = 0]
	pop rax
	pop rdi
	pop rsi
	pop rdx
	syscall
	push rax
	;;END MACRO: openat
.L20:	;; [INFO]: (INST)[type = STORE64, operand = 0]
	pop rbx
	pop rax
	mov [rax], rbx
	;;END MACRO: fopen
.L21:	;; [INFO]: (INST)[type = DROP, operand = 0]
	pop rax
.L22:	;; [INFO]: (INST)[type = PUSH_LOCAL_PTR, operand = 17]
	mov rax, [ret_stack_rsp]
	add rax, 17
	push rax
.L23:	;; [INFO]: (INST)[type = CALL_PROC, operand = 5]
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	call P5
	mov [ret_stack_rsp], rsp
	mov rsp, rax
.L24:	;; [INFO]: (INST)[type = CALL_PROC, operand = 2]
	;;start of inline procedure puts
.L25:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 1]
	;;START MACRO: stdout, of length: 1
.L26:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
	;;END MACRO: stdout
.L27:	;; [INFO]: (INST)[type = CALL_PROC, operand = 0]
	;;start of inline procedure fputs
.L28:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 331]
	;;START MACRO: write, of length: 2
.L29:	;; [INFO]: (INST)[type = EXTEND_MACRO, operand = 17]
	;;START MACRO: SYS_write, of length: 1
.L30:	;; [INFO]: (INST)[type = PUSH_INT, operand = 1]
	push 1
	;;END MACRO: SYS_write
.L31:	;; [INFO]: (INST)[type = SYSCALL3, operand = 0]
	pop rax
	pop rdi
	pop rsi
	pop rdx
	syscall
	push rax
	;;END MACRO: write
.L32:	;; [INFO]: (INST)[type = DROP, operand = 0]
	pop rax
	;;end of inline procedure fputs
	;;end of inline procedure puts
.L33:	;; [INFO]: return from main
	mov rax, rsp
	mov rsp, [ret_stack_rsp]
	add rsp, 25
	ret
_start:
	mov [args_ptr], rsp
	mov rax, ret_stack_end
	mov [ret_stack_rsp], rax
	call P6
	mov rdi, 0
	mov rax, 60
	syscall
segment .data
STR0: db "std/std.gfsl", 0
segment .bss
args_ptr: resq 1 
ret_stack_rsp: resq 1 
ret_stack: resb 4096
ret_stack_end: resq 1 

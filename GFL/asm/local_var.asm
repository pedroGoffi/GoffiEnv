	;; word 2 bytes
	;; dword 4 bytes
	;;  QWORD 8 bytes
	;;  double

	section .text
	global _start
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



main:
	push rbp
	mov rbp, rsp
	sub rsp, 4

	mov QWORD [rsp-4], 10
	mov rdi, [rsp-4]
	call dump
	
	section .data
	L0 db "hello, world!", 10
	section .text 

	mov QWORD [rsp-4], L0
	mov rax, 1
	mov rdi, 1
	mov rsi, [rsp - 4]
	mov rdx, 14
	syscall
	mov eax, 0
	leave
	ret
	
_start:
	call main
	mov rax, 60
	mov rsi, 0
	syscall	
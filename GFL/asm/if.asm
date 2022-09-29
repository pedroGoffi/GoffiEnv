	global _start
	section .text

main:
	push rbp
	mov rbp, rsp
	sub rsp, 4

	mov QWORD [rsp-4], 10
	mov rax, 11
	cmp [rsp-4], rax
	jne SKIP_IF
	section .data
	L0 db "skipped the if", 10
	L1 db "did not skipped the if", 10
	section .text
	mov QWORD [rsp-4], L0	
SKIP_IF:
	
	
	mov rax, 1
	mov rdi, 1
	mov rsi, [rsp-4]
	mov rdx, 25
	syscall
	
	
	
	mov eax, 0
	leave
	ret
_start:
	call main
	mov rax, 60
	mov rsi, 0
	syscall

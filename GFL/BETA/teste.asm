	global _start
_start:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	
	mov QWORD [rsp - 8], 1
	push QWORD [rsp - 8]

	mov rsi, 60
	mov rax, 0
	syscall

	global _start
__print:
	push rbp
	mov rbp, rsp
	sub rsp, 64
	mov DWORD [rbp-52], edi
	mov DWORD [rbp-4], 1
	mov eax, DWORD [rbp-4]
	mov edx, 32
	sub rdx, rax
	mov BYTE [rbp-48+rdx], 10
.L2:
	mov ecx, DWORD [rbp-52]
	mov edx, ecx
	mov eax, 3435973837
	imul rax, rdx
	shr rax, 32
	mov edx, eax
	shr edx, 3
	mov eax, edx
	sal eax, 2
	add eax, edx
	add eax, eax
	sub ecx, eax
	mov edx, ecx
	mov eax, edx
	lea ecx, [rax+48]
	mov eax, DWORD [rbp-4]
	mov edx, 31
	sub rdx, rax
	mov eax, ecx
	mov BYTE [rbp-48+rdx], al
	add DWORD [rbp-4], 1
	mov eax, DWORD [rbp-52]
	mov edx, eax
	mov eax, 3435973837
	imul rax, rdx
	shr rax, 32
	shr eax, 3
	mov DWORD [rbp-52], eax
	cmp DWORD [rbp-52], 0
	jne .L2
	mov eax, DWORD [rbp-4]
	mov edx, 32
	sub rdx, rax
	lea rax, [rbp-48]
	lea rcx, [rax+rdx]
	mov eax, DWORD [rbp-4]
	mov edx, eax
	mov rsi, rcx
	mov edi, 1
	mov rax, 1
	syscall
	nop
	leave
	ret
	;; file std/string.gfl
	;; file std/string.gfl
	global strlen
	section .text
strlen:
	push rbp
	mov  rbp, rsp
	sub rsp, 16
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.1:
	mov rax, 0
	push rax
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	push rax
	push 1
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	je .L.CMP.UNMATCH.0
	mov rax, 1
	jmp .L.CMP.END0
.L.CMP.UNMATCH.0:
	mov rax, 0
.L.CMP.END0:
	cmp rax, 0
	je .L.LOOP_BREAK.1
	jmp .L.LOOP_BEGIN.1
.L.LOOP_BREAK.1:
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	jmp .L.return.strlen
.L.END_OF_PROC.__strlen__:
.L.return.strlen:
	leave
	ret
	;; file std/string.gfl
	global streq
	section .text
streq:
	push rbp
	mov  rbp, rsp
	sub rsp, 48
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 24]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 40]
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	push rax
	lea rax, QWORD [rbp - 40]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	je .L.CMP.UNMATCH.1
	mov rax, 1
	jmp .L.CMP.END1
.L.CMP.UNMATCH.1:
	mov rax, 0
.L.CMP.END1:
	cmp rax, 0
	je .L.else.2
	mov rax, 0
	jmp .L.return.streq
	jmp .L.else.end.2
.L.else.2:
.L.else.end.2:
	lea rax, QWORD [rbp - 48]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.3:
	lea rax, QWORD [rbp - 40]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.2
	mov rax, 1
	jmp .L.CMP.END2
.L.CMP.UNMATCH.2:
	mov rax, 0
.L.CMP.END2:
	cmp rax, 0
	je .L.LOOP_BREAK.3
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	push rax
	push 1
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	movsx rax, BYTE [rax]

	push rax
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	push rax
	push 1
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	je .L.CMP.UNMATCH.3
	mov rax, 1
	jmp .L.CMP.END3
.L.CMP.UNMATCH.3:
	mov rax, 0
.L.CMP.END3:
	cmp rax, 0
	je .L.else.4
	mov rax, 0
	jmp .L.return.streq
	jmp .L.else.end.4
.L.else.4:
.L.else.end.4:
	lea rax, QWORD [rbp - 48]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.3
.L.LOOP_BREAK.3:
	mov rax, 1
	jmp .L.return.streq
.L.END_OF_PROC.__streq__:
.L.return.streq:
	leave
	ret
	;; file std/string.gfl
	global stoi
	section .text
stoi:
	push rbp
	mov  rbp, rsp
	sub rsp, 32
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 16]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
	mov rax, 0
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	jne .L.CMP.UNMATCH.4
	mov rax, 1
	jmp .L.CMP.END4
.L.CMP.UNMATCH.4:
	mov rax, 0
.L.CMP.END4:
	cmp rax, 0
	je .L.else.5
	mov rax, DATA0
	mov rdi, rax
	call puts
	mov rax, 1
	mov rdi, rax
	mov rax, 60
	syscall
	jmp .L.else.end.5
.L.else.5:
.L.else.end.5:
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.6:
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	push rax
	mov rax, 0
	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.5
	mov rax, 1
	jmp .L.CMP.END5
.L.CMP.UNMATCH.5:
	mov rax, 0
.L.CMP.END5:
	cmp rax, 0
	je .L.LOOP_BREAK.6
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 48
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	pop rbx
	sub rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 10
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	mul rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.6
.L.LOOP_BREAK.6:
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	jmp .L.return.stoi
.L.END_OF_PROC.__stoi__:
.L.return.stoi:
	leave
	ret
	;; file std/string.gfl
	global strcat
	section .text
strcat:
	push rbp
	mov  rbp, rsp
	sub rsp, 48
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, DATA1
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 40]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 48]
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.7:
	lea rax, QWORD [rbp - 40]
	mov rax, QWORD [rax]

	push rax
	mov rax, 0
	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.6
	mov rax, 1
	jmp .L.CMP.END6
.L.CMP.UNMATCH.6:
	mov rax, 0
.L.CMP.END6:
	cmp rax, 0
	je .L.LOOP_BREAK.7
	lea rax, QWORD [rbp - 32]
	movsx rax, BYTE [rax]

	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	pop rdi
	mov BYTE [rdi], al
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 8]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 40]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 40]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.7
.L.LOOP_BREAK.7:
.L.LOOP_BEGIN.8:
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	push rax
	mov rax, 0
	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.7
	mov rax, 1
	jmp .L.CMP.END7
.L.CMP.UNMATCH.7:
	mov rax, 0
.L.CMP.END7:
	cmp rax, 0
	je .L.LOOP_BREAK.8
	lea rax, QWORD [rbp - 32]
	movsx rax, BYTE [rax]

	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	pop rdi
	mov BYTE [rdi], al
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 48]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.8
.L.LOOP_BREAK.8:
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	jmp .L.return.strcat
.L.END_OF_PROC.__strcat__:
.L.return.strcat:
	leave
	ret
	;; file std/string.gfl
	global is_numeric
	section .text
is_numeric:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	mov rax, 57
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	mov rax, 48
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.8
	mov rax, 1
	jmp .L.CMP.END8
.L.CMP.UNMATCH.8:
	mov rax, 0
.L.CMP.END8:
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.9
	mov rax, 1
	jmp .L.CMP.END9
.L.CMP.UNMATCH.9:
	mov rax, 0
.L.CMP.END9:
	jmp .L.return.is_numeric
.L.END_OF_PROC.__is_numeric__:
.L.return.is_numeric:
	leave
	ret
	;; file std/string.gfl
	global is_alpha
	section .text
is_alpha:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	mov rax, 90
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	mov rax, 65
	push rax
	mov rax, 122
	pop rbx
	or rax, rbx
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	mov rax, 97
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.10
	mov rax, 1
	jmp .L.CMP.END10
.L.CMP.UNMATCH.10:
	mov rax, 0
.L.CMP.END10:
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.11
	mov rax, 1
	jmp .L.CMP.END11
.L.CMP.UNMATCH.11:
	mov rax, 0
.L.CMP.END11:
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.12
	mov rax, 1
	jmp .L.CMP.END12
.L.CMP.UNMATCH.12:
	mov rax, 0
.L.CMP.END12:
	pop rbx
	cmp rax, rbx
	jg .L.CMP.UNMATCH.13
	mov rax, 1
	jmp .L.CMP.END13
.L.CMP.UNMATCH.13:
	mov rax, 0
.L.CMP.END13:
	jmp .L.return.is_alpha
.L.END_OF_PROC.__is_alpha__:
.L.return.is_alpha:
	leave
	ret
	;; file std/errno.gfl
	;; file std/gflstdio.gfl
	global sys_write
	section .text
sys_write:
	push rbp
	mov  rbp, rsp
	sub rsp, 24
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	mov [rbp - 24], rdx
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdx, rax
	mov rax, 1
	syscall
	jmp .L.return.sys_write
.L.END_OF_PROC.__sys_write__:
.L.return.sys_write:
	leave
	ret
	;; file std/gflstdio.gfl
	global fwrite
	section .text
fwrite:
	push rbp
	mov  rbp, rsp
	sub rsp, 32
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	mov [rbp - 24], rdx
	lea rax, QWORD [rbp - 32]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdx, rax
	call sys_write
	pop rdi
	mov QWORD [rdi], rax
	mov rax, 0
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.14
	mov rax, 1
	jmp .L.CMP.END14
.L.CMP.UNMATCH.14:
	mov rax, 0
.L.CMP.END14:
	cmp rax, 0
	je .L.else.9
	mov rax, errno
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	push rax
	mov rax, 18446744073709551615
	pop rbx
	mul rbx
	pop rdi
	mov QWORD [rdi], rax
	mov rax, 18446744073709551615
	jmp .L.return.fwrite
	jmp .L.else.end.9
.L.else.9:
	mov rax, errno
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	jmp .L.return.fwrite
.L.else.end.9:
.L.END_OF_PROC.__fwrite__:
.L.return.fwrite:
	leave
	ret
	;; file std/gflstdio.gfl
	global putsf
	section .text
putsf:
	push rbp
	mov  rbp, rsp
	sub rsp, 24
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 24]
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	cmp rax, 0
	je  .L.LOCAL_IF_ELSE.0
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	jmp .L.LOCAL_IF_END.0
.L.LOCAL_IF_ELSE.0:
	mov rax, 0
.L.LOCAL_IF_END.0:
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdx, rax
	call fwrite
.L.END_OF_PROC.__putsf__:
.L.return.putsf:
	leave
	ret
	;; file std/gflstdio.gfl
	global puts
	section .text
puts:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	mov rax, 1
	mov rdi, rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rsi, rax
	call putsf
.L.END_OF_PROC.__puts__:
.L.return.puts:
	leave
	ret
	;; file std/gflstdio.gfl
	global input
	section .text
input:
	push rbp
	mov  rbp, rsp
	sub rsp, 32
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	call puts
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, DATA2
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 256
	pop rdi
	mov QWORD [rdi], rax
	mov rax, 0
	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdx, rax
	mov rax, 0
	syscall
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.10:
	mov rax, 10
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	je .L.CMP.UNMATCH.15
	mov rax, 1
	jmp .L.CMP.END15
.L.CMP.UNMATCH.15:
	mov rax, 0
.L.CMP.END15:
	cmp rax, 0
	je .L.LOOP_BREAK.10
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.10
.L.LOOP_BREAK.10:
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	jmp .L.return.input
.L.END_OF_PROC.__input__:
.L.return.input:
	leave
	ret
	;; file std/matematica.gfl
	global square
	section .text
square:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	mul rbx
.L.END_OF_PROC.__square__:
.L.return.square:
	leave
	ret
	;; file std/matematica.gfl
	global inc64
	section .text
inc64:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	pop rdi
	mov QWORD [rdi], rax
.L.END_OF_PROC.__inc64__:
.L.return.inc64:
	leave
	ret
	;; file std/matematica.gfl
	global fact2
	section .text
fact2:
	push rbp
	mov  rbp, rsp
	sub rsp, 16
	mov [rbp - 8], rdi
	mov rax, 2
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	jge .L.CMP.UNMATCH.16
	mov rax, 1
	jmp .L.CMP.END16
.L.CMP.UNMATCH.16:
	mov rax, 0
.L.CMP.END16:
	cmp rax, 0
	je .L.else.11
	mov rax, 1
	jmp .L.return.fact2
	jmp .L.else.end.11
.L.else.11:
.L.else.end.11:
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	mov rdi, rax
	call fact2
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	mul rbx
	jmp .L.return.fact2
.L.END_OF_PROC.__fact2__:
.L.return.fact2:
	leave
	ret
	;; file std/matematica.gfl
	global fact
	section .text
fact:
	push rbp
	mov  rbp, rsp
	sub rsp, 24
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 16]
	push rax
	mov rax, 1
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rdi
	mov QWORD [rdi], rax
.L.LOOP_BEGIN.12:
	mov rax, 0
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	jbe .L.CMP.UNMATCH.17
	mov rax, 1
	jmp .L.CMP.END17
.L.CMP.UNMATCH.17:
	mov rax, 0
.L.CMP.END17:
	cmp rax, 0
	je .L.LOOP_BREAK.12
	lea rax, QWORD [rbp - 16]
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	mul rbx
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.12
.L.LOOP_BREAK.12:
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	jmp .L.return.fact
.L.END_OF_PROC.__fact__:
.L.return.fact:
	leave
	ret
	;; file std/matematica.gfl
	global sum_from_zero
	section .text
sum_from_zero:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	mov rax, 2
	push rax
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	pop rbx
	pop rax
	add rax, rbx
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	push rax
	pop rbx
	pop rax
	mul rbx
	pop rbx
	div rbx
.L.END_OF_PROC.__sum_from_zero__:
.L.return.sum_from_zero:
	leave
	ret
	;; file std/matematica.gfl
	global sum_range
	section .text
sum_range:
	push rbp
	mov  rbp, rsp
	sub rsp, 32
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 24]
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	call sum_from_zero
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 0
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	jbe .L.CMP.UNMATCH.18
	mov rax, 1
	jmp .L.CMP.END18
.L.CMP.UNMATCH.18:
	mov rax, 0
.L.CMP.END18:
	cmp rax, 0
	je  .L.LOCAL_IF_ELSE.1
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	call sum_from_zero
	jmp .L.LOCAL_IF_END.1
.L.LOCAL_IF_ELSE.1:
	mov rax, 0
.L.LOCAL_IF_END.1:
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	push rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	jmp .L.return.sum_range
.L.END_OF_PROC.__sum_range__:
.L.return.sum_range:
	leave
	ret
	;; file std/file.gfl
	;; file std/file.gfl
	global sys_openat
	section .text
sys_openat:
	push rbp
	mov  rbp, rsp
	sub rsp, 16
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	mov rax, 18446744073709551516
	mov rdi, rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdx, rax
	mov rax, 257
	syscall
	jmp .L.return.sys_openat
.L.END_OF_PROC.__sys_openat__:
.L.return.sys_openat:
	leave
	ret
	;; file std/file.gfl
	global assert
	section .text
assert:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	not rax
	cmp rax, 0
	je .L.else.13
	mov rax, DATA3
	mov rdi, rax
	call puts
	mov rax, 1
	mov rdi, rax
	call exit
	jmp .L.else.end.13
.L.else.13:
.L.else.end.13:
.L.END_OF_PROC.__assert__:
.L.return.assert:
	leave
	ret
	;; file std/file.gfl
	global fopen
	section .text
fopen:
	push rbp
	mov  rbp, rsp
	sub rsp, 40
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 24]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 32]
	push rax
	mov rax, 0
	pop rdi
	mov QWORD [rdi], rax
	lea rax, QWORD [rbp - 40]
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	call strlen
	pop rdi
	mov QWORD [rdi], rax
	mov rax, 114
	push rax
	mov rax, 0
	push rax
	push 1
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	jne .L.CMP.UNMATCH.19
	mov rax, 1
	jmp .L.CMP.END19
.L.CMP.UNMATCH.19:
	mov rax, 0
.L.CMP.END19:
	cmp rax, 0
	je .L.else.14
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	mov rax, DATA4
	mov rsi, rax
	call streq
	cmp rax, 0
	je .L.else.15
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	mov rax, 0
	mov rsi, rax
	call sys_openat
	jmp .L.return.fopen
	jmp .L.else.end.15
.L.else.15:
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rdi, rax
	mov rax, DATA5
	mov rsi, rax
	call streq
	cmp rax, 0
	je .L.else.16
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	mov rax, 0
	push rax
	mov rax, 1
	pop rbx
	add rax, rbx
	mov rsi, rax
	call sys_openat
	jmp .L.return.fopen
	jmp .L.else.end.15
.L.else.16:
	mov rax, 18446744073709551615
	jmp .L.return.fopen
.L.else.end.15:
	jmp .L.else.end.14
.L.else.14:
.L.else.end.14:
	mov rax, 119
	push rax
	mov rax, 0
	push rax
	push 1
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	movsx rax, BYTE [rax]

	pop rbx
	cmp rax, rbx
	jne .L.CMP.UNMATCH.20
	mov rax, 1
	jmp .L.CMP.END20
.L.CMP.UNMATCH.20:
	mov rax, 0
.L.CMP.END20:
	cmp rax, 0
	je .L.else.17
	mov rax, DATA6
	mov rdi, rax
	call puts
	mov rax, 18446744073709551615
	jmp .L.return.fopen
	jmp .L.else.end.17
.L.else.17:
	mov rax, DATA7
	mov rdi, rax
	call puts
	mov rax, 1
	mov rdi, rax
	call exit
.L.else.end.17:
	mov rax, 1
	mov rdi, rax
	mov rax, 60
	syscall
.L.END_OF_PROC.__fopen__:
.L.return.fopen:
	leave
	ret
	;; file std/bool.gfl
	global exit
	section .text
exit:
	push rbp
	mov  rbp, rsp
	sub rsp, 8
	mov [rbp - 8], rdi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	mov rax, 60
	syscall
.L.END_OF_PROC.__exit__:
.L.return.exit:
	leave
	ret
	global sys_munmap
	section .text
sys_munmap:
	push rbp
	mov  rbp, rsp
	sub rsp, 16
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	mov rax, 11
	syscall
	jmp .L.return.sys_munmap
.L.END_OF_PROC.__sys_munmap__:
.L.return.sys_munmap:
	leave
	ret
	global sys_mmap
	section .text
sys_mmap:
	push rbp
	mov  rbp, rsp
	sub rsp, 48
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
	mov [rbp - 24], rdx
	mov [rbp - 32], rcx
	mov [rbp - 40], r8
	mov [rbp - 48], r9
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	mov rdi, rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	mov rsi, rax
	lea rax, QWORD [rbp - 24]
	mov rax, QWORD [rax]

	mov rdx, rax
	lea rax, QWORD [rbp - 32]
	mov rax, QWORD [rax]

	mov rcx, rax
	lea rax, QWORD [rbp - 40]
	mov rax, QWORD [rax]

	mov r8, rax
	lea rax, QWORD [rbp - 48]
	mov rax, QWORD [rax]

	mov r9, rax
	mov rax, 9
	syscall
	jmp .L.return.sys_mmap
.L.END_OF_PROC.__sys_mmap__:
.L.return.sys_mmap:
	leave
	ret
	global main
	section .text
main:
	push rbp
	mov  rbp, rsp
	sub rsp, 16
	mov [rbp - 8], rdi
	mov [rbp - 16], rsi
.L.LOOP_BEGIN.18:
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	cmp rax, 0
	je .L.LOOP_BREAK.18
	mov rax, DATA8
	mov rdi, rax
	call puts
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	push rax
	push 8
	pop rbx
	pop rax
	mul rbx
	push rax
	lea rax, QWORD [rbp - 16]
	mov rax, QWORD [rax]

	pop rbx
	add rax, rbx
	mov rax, QWORD [rax]

	mov rdi, rax
	call puts
	mov rax, DATA9
	mov rdi, rax
	call puts
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	cmp rax, rbx
	je .L.CMP.UNMATCH.21
	mov rax, 1
	jmp .L.CMP.END21
.L.CMP.UNMATCH.21:
	mov rax, 0
.L.CMP.END21:
	cmp rax, 0
	je  .L.LOCAL_IF_ELSE.2
	mov rax, DATA10
	jmp .L.LOCAL_IF_END.2
.L.LOCAL_IF_ELSE.2:
	mov rax, DATA11
.L.LOCAL_IF_END.2:
	mov rdi, rax
	call puts
	lea rax, QWORD [rbp - 8]
	push rax
	mov rax, 1
	mov rax, 1
	push rax
	lea rax, QWORD [rbp - 8]
	mov rax, QWORD [rax]

	pop rbx
	sub rax, rbx
	pop rdi
	mov QWORD [rdi], rax
	jmp .L.LOOP_BEGIN.18
.L.LOOP_BREAK.18:
.L.END_OF_PROC.__main__:
	mov eax, 0
.L.return.main:
	leave
	ret
_start:
	mov rdi, [rsp]
	mov rsi, rsp
	add rsi, 8
	mov rbp, rsp
	call main
	mov rdi, rax
	mov rax, 60
	syscall
	segment .bss
	errno: resb 8
	segment .data
	DATA0: db 69, 82, 82, 79, 82, 58, 32, 99, 97, 110, 32, 110, 111, 116, 32, 99, 111, 110, 118, 101, 114, 116, 32, 115, 116, 114, 105, 110, 103, 32, 116, 111, 32, 105, 110, 116, 101, 103, 101, 114, 44, 32, 103, 111, 116, 32, 78, 85, 76, 76, 32, 115, 116, 114, 105, 110, 103, 46, 10, 0, 
	DATA1: db 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 
	DATA2: db 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 
	DATA3: db 65, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 108, 108, 101, 100, 10, 0, 
	DATA4: db 114, 0, 
	DATA5: db 114, 43, 0, 
	DATA6: db 69, 82, 82, 79, 82, 58, 32, 103, 102, 108, 32, 112, 111, 114, 32, 101, 110, 113, 117, 97, 110, 116, 111, 32, 110, 97, 111, 32, 99, 111, 110, 115, 101, 103, 117, 101, 32, 99, 114, 105, 97, 114, 32, 97, 114, 113, 117, 105, 118, 111, 115, 46, 10, 0, 
	DATA7: db 69, 82, 82, 79, 82, 58, 32, 105, 110, 101, 120, 112, 101, 114, 97, 100, 97, 115, 32, 102, 108, 97, 103, 115, 44, 32, 101, 109, 32, 102, 111, 112, 101, 110, 46, 10, 0, 
	DATA8: db 39, 0, 
	DATA9: db 39, 0, 
	DATA10: db 32, 45, 45, 32, 0, 
	DATA11: db 10, 0, 

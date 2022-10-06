BITS 64
%define MACHINE_WORD_SIZE 8
segment .text
global _start
print_i64:
	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	mov [stack_top], rsi
	mov rdi, 0
	dec rsp
	inc rdi
	mov BYTE [rsp], 10
.print_i64_loop:
	xor rdx, rdx
	mov rbx, 10
	div rbx
	add rdx, '0'
	dec rsp
	inc rdi
	mov [rsp], dl
	cmp rax, 0
	jne .print_i64_loop
	mov rbx, rdi
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, rbx
	syscall
	add rsp, rbx
	ret
_start:
L0:

	jmp L1
L1:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L2:

;; NOTE The instruction load is not supported for now
L3:

	mov rsi, [stack_top]
	mov QWORD [rsi], 500
	add QWORD [stack_top], MACHINE_WORD_SIZE
L4:

;; NOTE The instruction store is not supported for now
L5:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L6:

;; NOTE The instruction load is not supported for now
L7:

	mov rsi, [stack_top]
	mov QWORD [rsi], 1
	add QWORD [stack_top], MACHINE_WORD_SIZE
L8:

;; NOTE The instruction store is not supported for now
L9:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L10:

;; NOTE The instruction load is not supported for now
L11:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L12:

;; NOTE The instruction load is not supported for now
L13:

	;; ERROR: unexpected cmp flag
L14:

	;;ERROR: could not translate to asm x86 64 the instructions.
L15:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	mov rdi, inst_map
	add rdi, MACHINE_WORD_SIZE * (38)
	cmp rax, 0
	je  JIE15
	jmp [rdi]
	JIE15:
L16:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L17:

;; NOTE The instruction load is not supported for now
L18:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L19:

;; NOTE The instruction load is not supported for now
L20:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L21:

;; NOTE The instruction load is not supported for now
L22:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rbx, [rsi]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	add rax, rbx
	mov [rsi], rax
	add rsi, MACHINE_WORD_SIZE
	mov [stack_top], rsi
L23:

;; NOTE The instruction store is not supported for now
L24:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L25:

;; NOTE The instruction load is not supported for now
L26:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L27:

;; NOTE The instruction load is not supported for now
L28:

;; NOTE The instruction store is not supported for now
L29:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L30:

;; NOTE The instruction load is not supported for now
L31:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L32:

;; NOTE The instruction load is not supported for now
L33:

;; NOTE The instruction store is not supported for now
L34:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L35:

;; NOTE The instruction load is not supported for now
L36:

	call print_i64
L37:

	jmp L9
L38:

	mov rsi, [stack_top]
	mov QWORD [rsi], 420
	add QWORD [stack_top], MACHINE_WORD_SIZE
L39:

	call print_i64
L40:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L41:

;; NOTE The instruction load is not supported for now
L42:

	mov rsi, [stack_top]
	mov QWORD [rsi], 500
	add QWORD [stack_top], MACHINE_WORD_SIZE
L43:

;; NOTE The instruction store is not supported for now
L44:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L45:

;; NOTE The instruction load is not supported for now
L46:

	mov rsi, [stack_top]
	mov QWORD [rsi], 1
	add QWORD [stack_top], MACHINE_WORD_SIZE
L47:

;; NOTE The instruction store is not supported for now
L48:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L49:

;; NOTE The instruction load is not supported for now
L50:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L51:

;; NOTE The instruction load is not supported for now
L52:

	;; ERROR: unexpected cmp flag
L53:

	;;ERROR: could not translate to asm x86 64 the instructions.
L54:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	mov rdi, inst_map
	add rdi, MACHINE_WORD_SIZE * (77)
	cmp rax, 0
	je  JIE54
	jmp [rdi]
	JIE54:
L55:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L56:

;; NOTE The instruction load is not supported for now
L57:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L58:

;; NOTE The instruction load is not supported for now
L59:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L60:

;; NOTE The instruction load is not supported for now
L61:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rbx, [rsi]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	add rax, rbx
	mov [rsi], rax
	add rsi, MACHINE_WORD_SIZE
	mov [stack_top], rsi
L62:

;; NOTE The instruction store is not supported for now
L63:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L64:

;; NOTE The instruction load is not supported for now
L65:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L66:

;; NOTE The instruction load is not supported for now
L67:

;; NOTE The instruction store is not supported for now
L68:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L69:

;; NOTE The instruction load is not supported for now
L70:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L71:

;; NOTE The instruction load is not supported for now
L72:

;; NOTE The instruction store is not supported for now
L73:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L74:

;; NOTE The instruction load is not supported for now
L75:

	call print_i64
L76:

	jmp L48
L77:

	mov rsi, [stack_top]
	mov QWORD [rsi], 420
	add QWORD [stack_top], MACHINE_WORD_SIZE
L78:

	call print_i64
L79:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L80:

;; NOTE The instruction load is not supported for now
L81:

	mov rsi, [stack_top]
	mov QWORD [rsi], 500
	add QWORD [stack_top], MACHINE_WORD_SIZE
L82:

;; NOTE The instruction store is not supported for now
L83:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L84:

;; NOTE The instruction load is not supported for now
L85:

	mov rsi, [stack_top]
	mov QWORD [rsi], 1
	add QWORD [stack_top], MACHINE_WORD_SIZE
L86:

;; NOTE The instruction store is not supported for now
L87:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L88:

;; NOTE The instruction load is not supported for now
L89:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L90:

;; NOTE The instruction load is not supported for now
L91:

	;; ERROR: unexpected cmp flag
L92:

	;;ERROR: could not translate to asm x86 64 the instructions.
L93:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	mov rdi, inst_map
	add rdi, MACHINE_WORD_SIZE * (116)
	cmp rax, 0
	je  JIE93
	jmp [rdi]
	JIE93:
L94:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L95:

;; NOTE The instruction load is not supported for now
L96:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L97:

;; NOTE The instruction load is not supported for now
L98:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L99:

;; NOTE The instruction load is not supported for now
L100:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rbx, [rsi]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	add rax, rbx
	mov [rsi], rax
	add rsi, MACHINE_WORD_SIZE
	mov [stack_top], rsi
L101:

;; NOTE The instruction store is not supported for now
L102:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L103:

;; NOTE The instruction load is not supported for now
L104:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L105:

;; NOTE The instruction load is not supported for now
L106:

;; NOTE The instruction store is not supported for now
L107:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L108:

;; NOTE The instruction load is not supported for now
L109:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L110:

;; NOTE The instruction load is not supported for now
L111:

;; NOTE The instruction store is not supported for now
L112:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L113:

;; NOTE The instruction load is not supported for now
L114:

	call print_i64
L115:

	jmp L87
L116:

	mov rsi, [stack_top]
	mov QWORD [rsi], 420
	add QWORD [stack_top], MACHINE_WORD_SIZE
L117:

	call print_i64
L118:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L119:

;; NOTE The instruction load is not supported for now
L120:

	mov rsi, [stack_top]
	mov QWORD [rsi], 500
	add QWORD [stack_top], MACHINE_WORD_SIZE
L121:

;; NOTE The instruction store is not supported for now
L122:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L123:

;; NOTE The instruction load is not supported for now
L124:

	mov rsi, [stack_top]
	mov QWORD [rsi], 1
	add QWORD [stack_top], MACHINE_WORD_SIZE
L125:

;; NOTE The instruction store is not supported for now
L126:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L127:

;; NOTE The instruction load is not supported for now
L128:

	mov rsi, [stack_top]
	mov QWORD [rsi], 24
	add QWORD [stack_top], MACHINE_WORD_SIZE
L129:

;; NOTE The instruction load is not supported for now
L130:

	;; ERROR: unexpected cmp flag
L131:

	;;ERROR: could not translate to asm x86 64 the instructions.
L132:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	mov rdi, inst_map
	add rdi, MACHINE_WORD_SIZE * (155)
	cmp rax, 0
	je  JIE132
	jmp [rdi]
	JIE132:
L133:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L134:

;; NOTE The instruction load is not supported for now
L135:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L136:

;; NOTE The instruction load is not supported for now
L137:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L138:

;; NOTE The instruction load is not supported for now
L139:

	mov rsi, [stack_top]
	sub rsi, MACHINE_WORD_SIZE
	mov rbx, [rsi]
	sub rsi, MACHINE_WORD_SIZE
	mov rax, [rsi]
	add rax, rbx
	mov [rsi], rax
	add rsi, MACHINE_WORD_SIZE
	mov [stack_top], rsi
L140:

;; NOTE The instruction store is not supported for now
L141:

	mov rsi, [stack_top]
	mov QWORD [rsi], 8
	add QWORD [stack_top], MACHINE_WORD_SIZE
L142:

;; NOTE The instruction load is not supported for now
L143:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L144:

;; NOTE The instruction load is not supported for now
L145:

;; NOTE The instruction store is not supported for now
L146:

	mov rsi, [stack_top]
	mov QWORD [rsi], 16
	add QWORD [stack_top], MACHINE_WORD_SIZE
L147:

;; NOTE The instruction load is not supported for now
L148:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L149:

;; NOTE The instruction load is not supported for now
L150:

;; NOTE The instruction store is not supported for now
L151:

	mov rsi, [stack_top]
	mov QWORD [rsi], 0
	add QWORD [stack_top], MACHINE_WORD_SIZE
L152:

;; NOTE The instruction load is not supported for now
L153:

	call print_i64
L154:

	jmp L126
L155:

	mov rsi, [stack_top]
	mov QWORD [rsi], 420
	add QWORD [stack_top], MACHINE_WORD_SIZE
L156:

	call print_i64
L157:

	mov rax, 60
	mov rdi, 0
	syscall
segment .data
stack_top: dq stack
inst_map: dq L0, L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L30, L31, L32, L33, L34, L35, L36, L37, L38, L39, L40, L41, L42, L43, L44, L45, L46, L47, L48, L49, L50, L51, L52, L53, L54, L55, L56, L57, L58, L59, L60, L61, L62, L63, L64, L65, L66, L67, L68, L69, L70, L71, L72, L73, L74, L75, L76, L77, L78, L79, L80, L81, L82, L83, L84, L85, L86, L87, L88, L89, L90, L91, L92, L93, L94, L95, L96, L97, L98, L99, L100, L101, L102, L103, L104, L105, L106, L107, L108, L109, L110, L111, L112, L113, L114, L115, L116, L117, L118, L119, L120, L121, L122, L123, L124, L125, L126, L127, L128, L129, L130, L131, L132, L133, L134, L135, L136, L137, L138, L139, L140, L141, L142, L143, L144, L145, L146, L147, L148, L149, L150, L151, L152, L153, L154, L155, L156, L157,
segment .bss
stack: resq 1024

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
__0:
__1:
proc_fcin:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__2:
__3:
   push 0x0
__4:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    syscall
    push rax
__5:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__6:
__7:
__8:
proc_cin:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__9:
__10:
   push 0x0
__11:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_fcin
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__12:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__13:
__14:
__15:
proc_console.getLine:
   sub rsp, 1024
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__16:
__17:
__18:
__19:
   push 0x400
__20:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__21:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_cin
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__22:
__23:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__24:
   pop rax
   pop rbx
   push rbx
   push rax
   push rbx
__25:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__26:
   push 0x1
__27:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__28:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__29:
   push 0xa
__30:
   mov rcx, 0
   mov rdx, 1
   pop rax
   pop rbx
   cmp rax,rbx
   cmove rcx, rdx
   push rcx
__31:
   pop rax
   test rax, rax
   jz __34
__32:
   push 0x1
__33:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__34:
   jmp __35
__35:
   pop rax
   pop rbx
   push rbx
   push rax
   push rbx
__36:
   push 0x0
__37:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__38:
   pop rax
   pop rbx
   push rax
   push rbx
__39:
   pop rbx
   pop rax
   mov [rax], rbx
__40:
   push 0x8
__41:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__42:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__43:
__44:
   pop rbx
   pop rax
   mov [rax], rbx
__45:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 1024
   ret
__46:
__47:
__48:
proc_inc64:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__49:
__50:
   pop rax
   push rax
   push rax
__51:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__52:
   push 0x1
__53:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__54:
   pop rbx
   pop rax
   mov [rax], rbx
__55:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__56:
__57:
__58:
proc_inc64_by:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__59:
__60:
   pop rax
   pop rbx
   push rbx
   push rax
   push rbx
__61:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__62:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__63:
   pop rbx
   pop rax
   mov [rax], rbx
__64:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__65:
__66:
__67:
proc_nth_argv:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__68:
__69:
   push 0x8
__70:
   pop rax
   pop rbx
   mul rbx
   push rax
__71:
   mov rax, [args_ptr]
   push rax
__72:
__73:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__74:
__75:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__76:
__77:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__78:
__79:
__80:
proc_argc:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__81:
__82:
   mov rax, [args_ptr]
   push rax
__83:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__84:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__85:
__86:
__87:
proc_argv:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__88:
__89:
   mov rax, [args_ptr]
   add rax, 8
   push rax
__90:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__91:
__92:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__93:
__94:
__95:
proc_itos:
   sub rsp, 24
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__96:
__97:
__98:
__99:
__100:
__101:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__102:
   pop rax
   pop rbx
   push rax
   push rbx
__103:
__104:
   pop rbx
   pop rax
   mov [rax], rbx
__105:
__106:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__107:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__108:
   push 0x0
__109:
   mov rcx, 0
   mov rdx, 1
   pop rax
   pop rbx
   cmp rax,rbx
   cmove rcx, rdx
   push rcx
__110:
   pop rax
   test rax, rax
   jz __115
__111:
   mov rax, 1
   push rax
   push data__w0
__112:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__113:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_str_assign
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__114:
   jmp __150
__115:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__116:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__117:
   mov rax, buffer
   add rax, 0
   push rax
__118:
   push 0x20
__119:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__120:
__121:
   pop rax
   pop rbx
   push rbx
   push rax
   push rbx
__122:
   push 0x0
__123:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__124:
   pop rax
   test rax, rax
   jz __138
__125:
   push 0x1
__126:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__127:
   pop rax
   push rax
   push rax
__128:
   pop rax
   pop rbx
   pop rcx
   push rbx
   push rax
   push rcx
__129:
   push 0xa
__130:
   xor rdx, rdx
   pop rbx
   pop rax
   div rbx
   push rax
   push rdx
__131:
   pop rax
   pop rbx
   pop rcx
   push rbx
   push rax
   push rcx
__132:
   pop rax
   pop rbx
   push rax
   push rbx
__133:
   push 0x30
__134:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__135:
   pop rbx
   pop rax
   mov BYTE [rax], bl
__136:
   pop rax
   pop rbx
   push rax
   push rbx
__137:
   jmp __121
__138:
   pop rax
   push rax
   push rax
__139:
   mov rax, buffer
   add rax, 0
   push rax
__140:
   push 0x20
__141:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__142:
   pop rax
   pop rbx
   push rax
   push rbx
__143:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__144:
__145:
   pop rax
   pop rbx
   push rax
   push rbx
__146:
__147:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__148:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_str_assign
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__149:
   jmp __150
__150:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__151:
   pop rax
   push rax
   push rax
__152:
   push 0x0
__153:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__154:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__155:
   pop rax
   pop rbx
   push rax
   push rbx
__156:
   push 0x8
__157:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__158:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__159:
__160:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 24
   ret
__161:
__162:
__163:
proc_fputs:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__164:
__165:
   push 0x1
__166:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    syscall
    push rax
__167:
   pop rax
__168:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__169:
__170:
__171:
proc_fputu:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__172:
__173:
   pop rax
   pop rbx
   push rax
   push rbx
__174:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_itos
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__175:
   pop rax
   pop rbx
   pop rcx
   push rbx
   push rax
   push rcx
__176:
   push 0x1
__177:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    syscall
    push rax
__178:
   pop rax
__179:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__180:
__181:
__182:
proc_eputs:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__183:
__184:
   push 0x2
__185:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_fputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__186:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__187:
__188:
__189:
proc_eputu:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__190:
__191:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_itos
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__192:
   push 0x2
__193:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_fputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__194:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__195:
__196:
__197:
proc_puts:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__198:
__199:
   push 0x1
__200:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_fputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__201:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__202:
__203:
__204:
proc_putu:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__205:
__206:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_itos
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__207:
   push 0x1
__208:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_fputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__209:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__210:
__211:
__212:
proc_putstr:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__213:
__214:
   pop rax
   push rax
   push rax
__215:
   push 0x0
__216:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__217:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__218:
   pop rax
   pop rbx
   push rax
   push rbx
__219:
   push 0x8
__220:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__221:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__222:
__223:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_puts
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__224:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__225:
__226:
__227:
proc_print:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__228:
__229:
   pop rax
   push rax
   push rax
__230:
   pop rdi
   call dump
__231:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__232:
__233:
__234:
proc_exit:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__235:
__236:
   push 0x3c
__237:
    pop rax
    pop rdi
    syscall
    push rax
__238:
   pop rax
__239:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__240:
__241:
__242:
proc_get_new_file_stream:
   sub rsp, 16
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__243:
__244:
__245:
__246:
   pop rax
   push rax
   push rax
__247:
   push 0x0
__248:
   pop rax
   pop rbx
   push rax
   push rbx
__249:
__250:
   pop rax
   push rax
   push rax
__251:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__252:
   push 0x0
__253:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovne rcx, rdx
   push rcx
__254:
   pop rax
   test rax, rax
   jz __262
__255:
   push 0x1
__256:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__257:
   pop rax
   pop rbx
   push rax
   push rbx
__258:
   push 0x1
__259:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__260:
   pop rax
   pop rbx
   push rax
   push rbx
__261:
   jmp __250
__262:
   pop rax
__263:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__264:
   push 0x0
__265:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__266:
   pop rax
   pop rbx
   push rax
   push rbx
__267:
__268:
   pop rbx
   pop rax
   mov [rax], rbx
__269:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__270:
   push 0x8
__271:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__272:
   pop rax
   pop rbx
   push rax
   push rbx
__273:
__274:
   pop rbx
   pop rax
   mov [rax], rbx
__275:
   push 0x1a4
__276:
   push 0x40
__277:
   push 0x1
__278:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__279:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__280:
   push 0x8
__281:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__282:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__283:
   push 0x0
__284:
   push 0x64
__285:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__286:
   push 0x101
__287:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    syscall
    push rax
__288:
__289:
   pop rax
   push rax
   push rax
__290:
   push 0x0
__291:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovl rcx, rdx
   push rcx
__292:
   pop rax
   test rax, rax
   jz __310
__293:
   mov rax, 32
   push rax
   push data__w1
__294:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__295:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__296:
   pop rax
   push rax
   push rax
__297:
   push 0x0
__298:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__299:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__300:
   pop rax
   pop rbx
   push rax
   push rbx
__301:
   push 0x8
__302:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__303:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__304:
__305:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__306:
   mov rax, 2
   push rax
   push data__w2
__307:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__308:
   push 0x1
__309:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_exit
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__310:
   jmp __311
__311:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 16
   ret
__312:
__313:
__314:
proc_open_file:
   sub rsp, 312
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__315:
__316:
__317:
__318:
__319:
__320:
__321:
__322:
__323:
__324:
   pop rax
   push rax
   push rax
__325:
   push 0x0
__326:
   pop rax
   pop rbx
   push rax
   push rbx
__327:
__328:
   pop rax
   push rax
   push rax
__329:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__330:
   push 0x0
__331:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovne rcx, rdx
   push rcx
__332:
   pop rax
   test rax, rax
   jz __340
__333:
   push 0x1
__334:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__335:
   pop rax
   pop rbx
   push rax
   push rbx
__336:
   push 0x1
__337:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__338:
   pop rax
   pop rbx
   push rax
   push rbx
__339:
   jmp __328
__340:
   pop rax
__341:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__342:
   push 0x0
__343:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__344:
   pop rax
   pop rbx
   push rax
   push rbx
__345:
__346:
   pop rbx
   pop rax
   mov [rax], rbx
__347:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__348:
   push 0x8
__349:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__350:
   pop rax
   pop rbx
   push rax
   push rbx
__351:
__352:
   pop rbx
   pop rax
   mov [rax], rbx
__353:
   push 0x1a4
__354:
   push 0x0
__355:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__356:
   push 0x8
__357:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__358:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__359:
   push 0x0
__360:
   push 0x64
__361:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__362:
   push 0x101
__363:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    syscall
    push rax
__364:
__365:
   pop rax
   push rax
   push rax
__366:
   push 0x0
__367:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovl rcx, rdx
   push rcx
__368:
   pop rax
   test rax, rax
   jz __386
__369:
   mov rax, 32
   push rax
   push data__w3
__370:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__371:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__372:
   pop rax
   push rax
   push rax
__373:
   push 0x0
__374:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__375:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__376:
   pop rax
   pop rbx
   push rax
   push rbx
__377:
   push 0x8
__378:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__379:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__380:
__381:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__382:
   mov rax, 2
   push rax
   push data__w4
__383:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__384:
   push 0x1
__385:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_exit
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__386:
   jmp __387
__387:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__388:
   pop rax
   pop rbx
   push rax
   push rbx
__389:
   pop rbx
   pop rax
   mov [rax], rbx
__390:
__391:
    mov rax, [ret_stack_rsp]
    add rax, 24
    push rax
__392:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__393:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__394:
   push 0x5
__395:
    pop rax
    pop rdi
    pop rsi
    syscall
    push rax
__396:
   push 0x0
__397:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovl rcx, rdx
   push rcx
__398:
   pop rax
   test rax, rax
   jz __416
__399:
   mov rax, 46
   push rax
   push data__w5
__400:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__401:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__402:
   pop rax
   push rax
   push rax
__403:
   push 0x0
__404:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__405:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__406:
   pop rax
   pop rbx
   push rax
   push rbx
__407:
   push 0x8
__408:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__409:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__410:
__411:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__412:
   mov rax, 2
   push rax
   push data__w6
__413:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__414:
   push 0x1
__415:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_exit
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__416:
   jmp __417
__417:
   push 0x0
__418:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__419:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__420:
   push 0x2
__421:
   push 0x1
__422:
    mov rax, [ret_stack_rsp]
    add rax, 24
    push rax
__423:
   push 0x30
__424:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__425:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__426:
   push 0x0
__427:
   push 0x9
__428:
    pop rax
    pop rdi
    pop rsi
    pop rdx
    pop r10
    pop r8
    pop r9
    syscall
    push rax
__429:
    mov rax, [ret_stack_rsp]
    add rax, 168
    push rax
__430:
   pop rax
   pop rbx
   push rax
   push rbx
__431:
   pop rbx
   pop rax
   mov [rax], rbx
__432:
__433:
    mov rax, [ret_stack_rsp]
    add rax, 168
    push rax
__434:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__435:
   push 0x0
__436:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovl rcx, rdx
   push rcx
__437:
   pop rax
   test rax, rax
   jz __455
__438:
   mov rax, 38
   push rax
   push data__w7
__439:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__440:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__441:
   pop rax
   push rax
   push rax
__442:
   push 0x0
__443:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__444:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__445:
   pop rax
   pop rbx
   push rax
   push rbx
__446:
   push 0x8
__447:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__448:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__449:
__450:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__451:
   mov rax, 2
   push rax
   push data__w8
__452:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_eputs
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__453:
   push 0x1
__454:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_exit
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__455:
   jmp __456
__456:
    mov rax, [ret_stack_rsp]
    add rax, 24
    push rax
__457:
   push 0x30
__458:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__459:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__460:
    mov rax, [ret_stack_rsp]
    add rax, 168
    push rax
__461:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 312
   ret
__462:
__463:
__464:
proc_chop_line:
   sub rsp, 32
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__465:
__466:
__467:
__468:
__469:
__470:
   pop rax
   push rax
   push rax
__471:
   push 0x0
__472:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__473:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__474:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__475:
   push 0x0
__476:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__477:
   pop rax
   pop rbx
   push rax
   push rbx
__478:
__479:
   pop rbx
   pop rax
   mov [rax], rbx
__480:
   pop rax
   push rax
   push rax
__481:
   push 0x8
__482:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__483:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__484:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__485:
   push 0x8
__486:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__487:
   pop rax
   pop rbx
   push rax
   push rbx
__488:
__489:
   pop rbx
   pop rax
   mov [rax], rbx
__490:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__491:
   push 0x8
__492:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__493:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__494:
   push 0x8
__495:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__496:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__497:
__498:
   pop rbx
   pop rax
   mov [rax], rbx
__499:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__500:
   push 0x0
__501:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__502:
   push 0x0
__503:
__504:
   pop rbx
   pop rax
   mov [rax], rbx
__505:
__506:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__507:
   push 0x0
__508:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__509:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__510:
   push 0x0
__511:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__512:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__513:
   push 0x8
__514:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__515:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__516:
__517:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__518:
   push 0xa
__519:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovne rcx, rdx
   push rcx
__520:
   pop rax
   pop rbx
   and rbx, rax
   push rbx
__521:
   pop rax
   test rax, rax
   jz __539
__522:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__523:
   push 0x0
__524:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__525:
   pop rax
   push rax
   push rax
__526:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__527:
   push 0x1
__528:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__529:
   pop rbx
   pop rax
   mov [rax], rbx
__530:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__531:
   push 0x8
__532:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__533:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__534:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__535:
   push 0x0
__536:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__537:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__538:
   jmp __506
__539:
__540:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__541:
   push 0x0
__542:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__543:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__544:
   push 0x0
__545:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__546:
   pop rax
   test rax, rax
   jz __559
__547:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__548:
   push 0x0
__549:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__550:
   pop rax
   push rax
   push rax
__551:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__552:
   push 0x1
__553:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__554:
   pop rbx
   pop rax
   mov [rax], rbx
__555:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__556:
   push 0x8
__557:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__558:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__559:
   jmp __560
__560:
   pop rax
   push rax
   push rax
__561:
   push 0x8
__562:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__563:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__564:
   push 0x8
__565:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__566:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__567:
__568:
   pop rbx
   pop rax
   mov [rax], rbx
__569:
   push 0x0
__570:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__571:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__572:
   push 0x0
__573:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__574:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__575:
__576:
   pop rbx
   pop rax
   mov [rax], rbx
__577:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__578:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 32
   ret
__579:
__580:
__581:
proc_chop_word:
   sub rsp, 32
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__582:
__583:
__584:
__585:
__586:
__587:
   pop rax
   push rax
   push rax
__588:
   push 0x0
__589:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__590:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__591:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__592:
   push 0x0
__593:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__594:
   pop rax
   pop rbx
   push rax
   push rbx
__595:
__596:
   pop rbx
   pop rax
   mov [rax], rbx
__597:
   pop rax
   push rax
   push rax
__598:
   push 0x8
__599:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__600:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__601:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__602:
   push 0x8
__603:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__604:
   pop rax
   pop rbx
   push rax
   push rbx
__605:
__606:
   pop rbx
   pop rax
   mov [rax], rbx
__607:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__608:
   push 0x8
__609:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__610:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__611:
   push 0x8
__612:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__613:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__614:
__615:
   pop rbx
   pop rax
   mov [rax], rbx
__616:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__617:
   push 0x0
__618:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__619:
   push 0x0
__620:
__621:
   pop rbx
   pop rax
   mov [rax], rbx
__622:
__623:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__624:
   push 0x0
__625:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__626:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__627:
   push 0x0
__628:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__629:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__630:
   push 0x8
__631:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__632:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__633:
__634:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__635:
   push 0x20
__636:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovne rcx, rdx
   push rcx
__637:
   pop rax
   pop rbx
   and rbx, rax
   push rbx
__638:
   pop rax
   test rax, rax
   jz __656
__639:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__640:
   push 0x0
__641:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__642:
   pop rax
   push rax
   push rax
__643:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__644:
   push 0x1
__645:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__646:
   pop rbx
   pop rax
   mov [rax], rbx
__647:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__648:
   push 0x8
__649:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__650:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__651:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__652:
   push 0x0
__653:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__654:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__655:
   jmp __623
__656:
__657:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__658:
   push 0x0
__659:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__660:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__661:
   push 0x0
__662:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__663:
   pop rax
   test rax, rax
   jz __676
__664:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__665:
   push 0x0
__666:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__667:
   pop rax
   push rax
   push rax
__668:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__669:
   push 0x1
__670:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__671:
   pop rbx
   pop rax
   mov [rax], rbx
__672:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__673:
   push 0x8
__674:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__675:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__676:
   jmp __677
__677:
   pop rax
   push rax
   push rax
__678:
   push 0x8
__679:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__680:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__681:
   push 0x8
__682:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__683:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__684:
__685:
   pop rbx
   pop rax
   mov [rax], rbx
__686:
   push 0x0
__687:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__688:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__689:
   push 0x0
__690:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__691:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__692:
__693:
   pop rbx
   pop rax
   mov [rax], rbx
__694:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__695:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 32
   ret
__696:
__697:
__698:
proc_trim_left:
   sub rsp, 32
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__699:
__700:
__701:
__702:
__703:
__704:
   pop rax
   push rax
   push rax
__705:
   push 0x0
__706:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__707:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__708:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__709:
   push 0x0
__710:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__711:
   pop rax
   pop rbx
   push rax
   push rbx
__712:
__713:
   pop rbx
   pop rax
   mov [rax], rbx
__714:
   pop rax
   push rax
   push rax
__715:
   push 0x8
__716:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__717:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__718:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__719:
   push 0x8
__720:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__721:
   pop rax
   pop rbx
   push rax
   push rbx
__722:
__723:
   pop rbx
   pop rax
   mov [rax], rbx
__724:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__725:
   push 0x8
__726:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__727:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__728:
   push 0x8
__729:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__730:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__731:
__732:
   pop rbx
   pop rax
   mov [rax], rbx
__733:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__734:
   push 0x0
__735:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__736:
   push 0x0
__737:
__738:
   pop rbx
   pop rax
   mov [rax], rbx
__739:
__740:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__741:
   push 0x0
__742:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__743:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__744:
   push 0x0
__745:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovg rcx, rdx
   push rcx
__746:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__747:
   push 0x8
__748:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__749:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__750:
__751:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__752:
   push 0x20
__753:
   mov rcx, 0
   mov rdx, 1
   pop rax
   pop rbx
   cmp rax,rbx
   cmove rcx, rdx
   push rcx
__754:
   pop rax
   pop rbx
   and rbx, rax
   push rbx
__755:
   pop rax
   test rax, rax
   jz __773
__756:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__757:
   push 0x0
__758:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__759:
   pop rax
   push rax
   push rax
__760:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__761:
   push 0x1
__762:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__763:
   pop rbx
   pop rax
   mov [rax], rbx
__764:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__765:
   push 0x8
__766:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__767:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__768:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__769:
   push 0x0
__770:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__771:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__772:
   jmp __740
__773:
   pop rax
   push rax
   push rax
__774:
   push 0x8
__775:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__776:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__777:
   push 0x8
__778:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__779:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__780:
__781:
   pop rbx
   pop rax
   mov [rax], rbx
__782:
   push 0x0
__783:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__784:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__785:
   push 0x0
__786:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__787:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__788:
__789:
   pop rbx
   pop rax
   mov [rax], rbx
__790:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__791:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 32
   ret
__792:
__793:
__794:
proc_streq:
   sub rsp, 40
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__795:
__796:
__797:
__798:
__799:
__800:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__801:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_str_assign
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__802:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__803:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_str_assign
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__804:
__805:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__806:
   push 0x0
__807:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__808:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__809:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__810:
   push 0x0
__811:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__812:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__813:
   mov rcx, 0
   mov rdx, 1
   pop rax
   pop rbx
   cmp rax,rbx
   cmove rcx, rdx
   push rcx
__814:
   pop rax
   test rax, rax
   jz __861
__815:
__816:
__817:
    mov rax, [ret_stack_rsp]
    add rax, 32
    push rax
__818:
   push 0x0
__819:
__820:
   pop rbx
   pop rax
   mov [rax], rbx
__821:
__822:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__823:
   push 0x8
__824:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__825:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__826:
__827:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__828:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__829:
   push 0x8
__830:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__831:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__832:
__833:
   pop rax
   xor rbx, rbx
   mov bl, BYTE [rax]
   push rbx
__834:
   mov rcx, 0
   mov rdx, 1
   pop rax
   pop rbx
   cmp rax,rbx
   cmove rcx, rdx
   push rcx
__835:
   pop rax
   test rax, rax
   jz __861
__836:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__837:
   push 0x0
__838:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__839:
   pop rax
   push rax
   push rax
__840:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__841:
   push 0x1
__842:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__843:
   pop rbx
   pop rax
   mov [rax], rbx
__844:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__845:
   push 0x8
__846:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__847:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__848:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__849:
   push 0x0
__850:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__851:
   pop rax
   push rax
   push rax
__852:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__853:
   push 0x1
__854:
   pop rax
   pop rbx
   sub rbx,rax
   push rbx
__855:
   pop rbx
   pop rax
   mov [rax], rbx
__856:
    mov rax, [ret_stack_rsp]
    add rax, 16
    push rax
__857:
   push 0x8
__858:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__859:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_inc64
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__860:
   jmp __822
__861:
   jmp __862
__862:
   push 0x1
__863:
__864:
__865:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__866:
   push 0x0
__867:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__868:
   pop rax
   xor rbx, rbx
   mov rbx, [rax]
   push rbx
__869:
   push 0x0
__870:
   mov rcx, 0
   mov rdx, 1
   pop rbx
   pop rax
   cmp rax, rbx
   cmovne rcx, rdx
   push rcx
__871:
   pop rax
   test rax, rax
   jz __875
__872:
   pop rax
__873:
   push 0x0
__874:
__875:
   jmp __876
__876:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 40
   ret
__877:
__878:
__879:
proc_str_to_ptr:
   sub rsp, 16
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__880:
__881:
__882:
__883:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__884:
   push 0x8
__885:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__886:
   pop rax
   pop rbx
   push rax
   push rbx
__887:
__888:
   pop rbx
   pop rax
   mov [rax], rbx
__889:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__890:
   push 0x0
__891:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__892:
   pop rax
   pop rbx
   push rax
   push rbx
__893:
__894:
   pop rbx
   pop rax
   mov [rax], rbx
__895:
    mov rax, [ret_stack_rsp]
    add rax, 0
    push rax
__896:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 16
   ret
__897:
__898:
__899:
proc_str_assign:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__900:
__901:
   pop rax
   pop rbx
   push rax
   push rbx
__902:
   pop rax
   pop rbx
   push rbx
   push rax
   push rbx
__903:
   push 0x8
__904:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__905:
   pop rax
   pop rbx
   push rax
   push rbx
__906:
__907:
   pop rbx
   pop rax
   mov [rax], rbx
__908:
   push 0x0
__909:
   pop rbx
   pop rax
   add rax, rbx
   push rax
__910:
   pop rax
   pop rbx
   push rax
   push rbx
__911:
__912:
   pop rbx
   pop rax
   mov [rax], rbx
__913:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__914:
__915:
__916:
proc_printstr:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__917:
__918:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_puts
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__919:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__920:
__921:
__922:
proc_main:
   sub rsp, 0
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__923:
__924:
   mov rax, 14
   push rax
   push data__w9
__925:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   call proc_printstr
   mov [ret_stack_rsp], rsp
   mov rsp, rax
__926:
   mov rax, rsp
   mov rsp, [ret_stack_rsp]
   add rsp, 0
   ret
__927:
_start:
   mov [args_ptr], rsp
   mov rax, ret_stack_end
   mov [ret_stack_rsp], rax
   call proc_main
__928:
   mov rax, 60
   mov rdi, 0
   syscall
segment .data
data__w0: db	0x30,0
data__w1: db	0x45,0x52,0x52,0x4F,0x52,0x3A,0x20,0x63,0x6F,0x75,0x6C,0x64,0x20,0x6E,0x6F,0x74,0x20,0x6F,0x70,0x65,0x6E,0x20,0x74,0x68,0x65,0x20,0x66,0x69,0x6C,0x65,0x20,0x60,0
data__w2: db	0x60,0x0A,0
data__w3: db	0x45,0x52,0x52,0x4F,0x52,0x3A,0x20,0x63,0x6F,0x75,0x6C,0x64,0x20,0x6E,0x6F,0x74,0x20,0x6F,0x70,0x65,0x6E,0x20,0x74,0x68,0x65,0x20,0x66,0x69,0x6C,0x65,0x20,0x60,0
data__w4: db	0x60,0x0A,0
data__w5: db	0x45,0x52,0x52,0x4F,0x52,0x3A,0x20,0x63,0x6F,0x75,0x6C,0x64,0x20,0x6E,0x6F,0x74,0x20,0x6D,0x65,0x73,0x75,0x72,0x65,0x20,0x74,0x68,0x65,0x20,0x73,0x69,0x7A,0x65,0x20,0x6F,0x66,0x20,0x74,0x68,0x65,0x20,0x66,0x69,0x6C,0x65,0x20,0x60,0
data__w6: db	0x60,0x0A,0
data__w7: db	0x45,0x52,0x52,0x4F,0x52,0x3A,0x20,0x63,0x6F,0x75,0x6C,0x64,0x20,0x6E,0x6F,0x74,0x20,0x6D,0x65,0x6D,0x6F,0x72,0x79,0x20,0x6D,0x61,0x70,0x20,0x74,0x68,0x65,0x20,0x66,0x69,0x6C,0x65,0x20,0x60,0
data__w8: db	0x60,0x0A,0
data__w9: db	0x48,0x65,0x6C,0x6C,0x6F,0x2C,0x20,0x77,0x6F,0x72,0x6C,0x64,0x21,0x0A,0
segment .bss
args_ptr:	     resq 1
ret_stack_rsp:  resq 1
ret_stack:      resb 4096
ret_stack_end:  resq 1
buffer:    resb    32

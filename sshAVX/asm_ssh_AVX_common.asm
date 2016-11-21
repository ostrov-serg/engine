
extern MultiByteToWideChar:near

.data?

result		dw 128 dup(?)

.data

dbl_znak	dq 1.0, -1.0
			dq 1.0
			dq 10.0
			dq 100.0
			dq 1000.0
			dq 10000.0
			dq 100000.0
			dq 1000000.0
			dq 10000000.0
			dq 100000000.0
			dq 1000000000.0
			dq 10000000000.0
			dq 100000000000.0
hex_sym		dw 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70
is_valid	dw 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

.code

; подсчитать длину строки - SSE4.2
; rcx - string
; ret - rax
asm_ssh_wcslen proc
		mov rdx, rcx
		xor rax, rax
@@:		vlddqu xmm0, xmmword ptr [rdx + rax * 2]
		vpcmpistri xmm0, xmm0, 00010001b
		lea rax, [rax + rcx]
		jnz @b
		ret
asm_ssh_wcslen endp

; поиск подстроки - SSE4.2
; rcx - wstring
; rdx - wsubstring
; ret - rax = index
asm_ssh_wcsstr proc public
		mov rax, rdx
		vlddqu xmm0, xmmword ptr [rcx]
@@:		vpcmpistri xmm0, xmmword ptr [rdx + rax * 2], 00001101b
		lea rax, [rax + rcx]
		jnc @b
		ret
asm_ssh_wcsstr endp

; поиск символа SSE4.2
; rcx - wstring
; rdx - wchar
; ret - rax = index
asm_ssh_wcschr proc
		test rdx, rdx
		jz _zero
		vmovq xmm0, rdx
		mov rax, rcx
@@:		vpcmpistri xmm0, xmmword ptr [rax], 00000001b
		lea rax, [rax + rcx * 2]
		jnc @b
		ret
_zero:	mov r8, rcx
		call asm_ssh_wcslen
		lea rax, [r8 + rax * 2]
		ret
asm_ssh_wcschr endp

; сравнение строк  - SSE4.2
; rcx - wstring1
; rdx - wstring2
; ret - rax 
asm_ssh_wcscmp proc
		xor r10, r10
		mov r8, rcx
@@:		vlddqu xmm0, xmmword ptr [r8 + r10 * 2]
		vlddqu xmm1, xmmword ptr [rdx + r10 * 2]
		vpcmpistri xmm0, xmm1, 01011001b
		lea r10, [r10 + rcx]
		jnz @b
		movzx rcx, word ptr [r8 + r10 * 2]
		movzx rdx, word ptr [rdx + r10 * 2]
		xor rax, rax
		ret
asm_ssh_wcscmp endp
; преобразовать число в строку, в зависимости от системы счисления 0-decimal, (1-bin, 2-oct, 3-hex, 4-double,5-float,6-bool)
; rcx - указатель на число
; rdx - radix
; r8 - ptr end
; rax - result of whar_t*
asm_ssh_ntow proc public
		push r10
		push r11
		push r12
		push r13
		mov r9, offset result + 128
		mov word ptr [r9], 0
		jrcxz @f
		mov r13, r9
		mov r12, r8
		mov rax, [rcx]
		imul rdx, 24
		mov r10, offset tbl_radix
		add rdx, r10
		mov r8, offset hex_sym
		mov r10, [rdx + 8]
		mov rcx, [rdx + 16]
		call qword ptr [rdx]
		test r12, r12
		jz @f
		mov [r12], r13
@@:		mov rax, r9
		pop r13
		pop r12
		pop r11
		pop r10
		ret
str_bool db 'f', 0, 'a', 0, 'l', 0, 's', 0, 'e', 0, 0, 0, 't', 0, 'r', 0, 'u', 0, 'e', 0, 0, 0, 0, 0
tbl_radix dq nto_dec, 0, 10, nto_ohb, 1, 1, nto_ohb, 7, 3, nto_ohb, 15, 4, nto_dbl, 0, 10, nto_flt, 0, 10, nto_bool, offset str_bool, 12
nto_bool:
		test rax, rax
		cmovnz rax, rcx
		mov rdx, [rax + r10]
		mov eax, [rax + r10 + 8]
		mov [r9], rdx
		mov [r9 + 8], eax
		lea r13, [r9 + 12]
		ret
nto_ohb:sub r9, 2
		mov rdx, rax
		and rdx, r10
		movzx rdx, word ptr [rdx * 2 + r8]
		mov [r9], dx
		shr rax, cl
		jnz nto_ohb
		ret
nto_dec:; определяем знак
		sub r9, 2
		test rax, rax
		jns @f
		mov r10w, '-'
		neg rax
@@:		cmp rax, rcx
		jb @f
		xor rdx, rdx
		div rcx
		or dl, 48
		mov word ptr [r9], dx
		sub r9, 2
		jmp @b
@@:		or al, 48
		mov word ptr [r9], ax
		test r10, r10
		jz @f
		sub r9, 2
		mov [r9], r10w
@@:		ret
nto_flt:movd xmm0, eax
		mov r11, 4
		vcvtss2sd xmm0, xmm0, xmm0
		jmp @f
nto_dbl:; read double from rax
		mov r11, 8
		vmovq xmm0, rax
		; отбросим дробную часть
@@:		vroundsd xmm1, xmm0, xmm0, 3
		vcvttsd2si rax, xmm1
		; преобразуем целую часть в строку
		call nto_dec
		mov rdx, offset result + 128
		mov word ptr [rdx], '.'
		add rdx, 2
		; количество знаков после запятой
		mov rcx, r11
		mov r10, rdx
@@:		; дробная часть
		vsubsd xmm0, xmm0, xmm1
		vmulsd xmm0, xmm0, qword ptr [offset dbl_znak + 24]
		vroundsd xmm1, xmm0, xmm0, 3
		vcvttsd2si rax, xmm1
		test rax, rax
		cmovnz r10, rdx
		add rax, 48
		mov [rdx], ax
		add rdx, 2
		loop @b
		lea r13, [r10 + 2]
		mov [r13], cx
		ret
asm_ssh_ntow endp

; распарсить спецификатор wprintf
; rcx - val
; rdx - указатель на ptr
; ret - result of whar_t*
asm_ssh_parse_spec proc public
LOCAL @@end:QWORD
		push rdi
		push r12
		mov r10, rcx
		mov r11, rdx
		mov rcx, [rdx]
		mov rdx, 4
		lea r8, @@end
		call asm_ssh_wton
		vmovsd xmm0, qword ptr [rax]
		mov r12, @@end
		mov ax, [r12]
		mov rcx, 14
		call f_spec
		jnc @f
		add r12, 2
		call qword ptr [rax + rcx + 8]
@@:		mov [r11], r12
		test rax, rax
		jnz @f
		mov rax, offset result
		mov word ptr [rax], 0
@@:		add qword ptr [r10], 8
		pop r12
		pop rdi
		ret
OPTION EPILOGUE:NONE
sp_ii:	cmp dword ptr [r12], 00340036h	; I64
		jnz sp_err
		mov ax, [r12 + 4]
		mov rcx, 8
		call f_spec
		jnc sp_err
		add r12, 6
		jmp @f
sp_b:
sp_x:
sp_o:
sp_i:	mov rax, [r10]
		mov ecx, [rax]
		mov [rax], rcx
@@:		mov rcx, [r10]
		xor r8, r8
		call asm_ssh_ntow
		mov rdi, rax
		mov r9, rax
		mov rcx, offset result + 128
		sub rax, rcx
		neg rax
		shr rax, 1				; length number
		vcvttsd2si rcx, xmm0	; length spec number
		sub rcx, rax
		jle @f
		sub rdi, rcx
		sub rdi, rcx
		mov r9, rdi
		mov ax, '0'
		rep stosw
@@:		mov rax, r9
		ret
sp_err:	ret
sp_f:	mov rcx, 10
@@:		vroundsd xmm1, xmm0, xmm0, 3
		vsubsd xmm0, xmm0, xmm1
		vmulsd xmm0, xmm0, qword ptr [dbl_znak + 24]
		vcvttsd2si rax, xmm0
		test rax, rax
		loopz @b
		push rax
		mov rcx, [r10]
		lea r8, @@end
		call asm_ssh_ntow
		mov r9, rax
		mov rax, offset result + 130
		mov rdi, @@end
		sub rax, rdi
		neg rax
		shr rax, 1				; length number
		pop rcx					; length spec number
		sub rcx, rax
		jle @f
		mov ax, '0'
		rep stosw
		mov [rdi], ah
		mov rax, r9
		ret
@@:		mov [rdi + rcx * 2], ah
		mov rax, r9
		ret
sp_cc:	sub rsp, 48
		mov r8, [r10]
		mov r9, 1
		xor rcx, rcx
		xor rdx, rdx
		mov rax, offset result
		mov qword ptr [rsp + 32], rax
		mov qword ptr [rsp + 40], r9
		call MultiByteToWideChar
		add rsp, 48
		mov rax, offset result
		ret
sp_c:	mov rax, offset result + 128
		mov rcx, [r10]
		movzx rcx, word ptr [rcx]
		mov [rax], rcx
		ret
strlen:	xor rax, rax
@@:		inc rax
		cmp byte ptr [r8 + rax], 0
		jnz @b
		ret
sp_ss:	mov r8, [r10]
		mov r8, [r8]
		call strlen
		sub rsp, 48
		mov r9, rax
		xor rcx, rcx
		xor rdx, rdx
		mov rax, offset result
		mov qword ptr [rsp + 32], rax
		mov qword ptr [rsp + 40], r9
		call MultiByteToWideChar
		add rsp, 48
		mov rax, offset result
		ret
sp_s:	mov rax, [r10]
		mov rax, [rax]
		ret
f_spec:	vmovq xmm1, rax
		xor rax, rax
		vpcmpistri xmm1, xmmword ptr [_spec], 0
		jnc @f
		mov rax, offset _spec_tbl
		shl rcx, 4
		mov rdx, [rax + rcx]
		stc
@@:		ret
OPTION EPILOGUE:EPILOGUEDEF
align 16
_spec	db 'BbOoIiXxCcSsf', 0, 0, 0, 0, 0
_spec_tbl dq 1, sp_b, 1, sp_b, 2, sp_o, 2, sp_o, 0, sp_ii, 0, sp_i, 3, sp_x, 3, sp_x, 0, sp_cc, 0, sp_c, 0, sp_ss, 0, sp_s, 4, sp_f, 0, 0
asm_ssh_parse_spec endp

; преобразовать строку в число в зависимости от системы счисления
; rcx - ptr
; rdx - radix
; r8 -  &ptr end
asm_ssh_wton proc public
		push rbx
		push r10
		push r11
		push r12
		push r13
		push r14
		mov r14, r8
		xor rax, rax
		jrcxz @f
		mov r10, rcx
		xor r13, r13
		mov r9, offset radix
		imul rdx, 24
		add rdx, r9
		mov r9, [rdx + 8]; множитель порядка
		mov r8, [rdx + 16]; маска
		call qword ptr [rdx]
@@:		mov qword ptr [result], rax
		test r14, r14
		jz @f
		mov [r14], r11
@@:		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop rbx
		mov rax, offset result
		ret
_BIN	= 1
_OCT	= 2
_HEX	= 4
_DEC	= 8
tbl_hex dw 0, _HEX, _HEX, _HEX, _HEX, _HEX, _HEX, 0, 0, 0, 0, 0, 0, 0, 0, 0, _DEC + _BIN + _OCT + _HEX, _DEC + _BIN + _OCT + _HEX, _DEC + _OCT + _HEX, _DEC + _OCT + _HEX, _DEC + _OCT + _HEX
		dw _DEC + _OCT + _HEX, _DEC + _OCT + _HEX, _DEC + _OCT + _HEX, _DEC + _HEX, _DEC + _HEX
radix	dq wto_dec, 10, _DEC, wto_obh, 2, _BIN, wto_obh, 8, _OCT, wto_obh, 16, _HEX, wto_dbl, 10, _DEC, wto_flt, 10, _DEC, wto_bool, 0, 0
wto_bool:
		xor rax, rax
		cmp dword ptr [rcx], 00720074h
		setz al
		cmp dword ptr [rcx + 4], 00650075h
		setz ah
		and al, ah
		movzx rax, al
		ret
wto_obh:sub rcx, 2
		mov rdx, offset tbl_hex
@@:		add rcx, 2
		movzx rax, word ptr [rcx]
		and rax, -33
		cmp rax, 'F'
		ja @f
		cmp rax, 16
		jb @f
		and rax, 00011111b
		movzx rax, word ptr [rax * 2 + rdx]
		and rax, r8
		jnz @b
@@:		mov r11, rcx
		mov r8, 1
		xor rax, rax
		mov rdx, offset is_valid
@@:		sub rcx, 2
		cmp rcx, r10
		jb @f
		movzx rbx, word ptr [rcx]
		and rbx, -97
		movzx rbx, word ptr [rbx * 2 + rdx]
		imul rbx, r8
		add rax, rbx
		imul r8, r9
		jmp @b
@@:		ret
wto_dec:; определяем признак отрицательного значения
		xor r12, r12
		cmp word ptr [rcx], '-'
		setz r12b
		lea rcx, [rcx + r12 * 2]
		call wto_obh
		test r12, r12
		jz @f
		neg rax
@@:		ret
wto_flt:inc r13
wto_dbl:call wto_dec
		mov r10, r11
		vcvtsi2sd xmm0, xmm0, rax
		vxorps xmm1, xmm1, xmm1
		cmp word ptr [r11], '.'
		jnz @f
		mov r8, 8
		lea rcx, [r11 + 2]
		mov r10, rcx
		call wto_obh
		vcvtsi2sd xmm1, xmm1, rax
@@:		mov r9, r11
		sub r9, r10
		mov rcx, 22
		cmp r9, rcx
		cmova r9, rcx
		mov r8, offset dbl_znak
		shl r12, 1
		vdivsd xmm1, xmm1, qword ptr [r8 + r9 * 4 + 16]
		vfmadd231sd xmm0, xmm1, qword ptr [r8 + r12 * 8]
		test r13, r13
		jz @f
		vcvtsd2ss xmm0, xmm0, xmm0
@@:		vmovq rax, xmm0
		ret
asm_ssh_wton endp


;rcx - src, rdx - vec
;<[/]tag [attr = value ...][/]>
;vec - 0 - <[/], 1- tag, 2 - [/]>, 3.... - attrs
xml_chars	db 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2
			db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2, 2, 0, 0, 1
			db 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1
			db 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
asm_ssh_parse_xml proc USES rbx r10 r11 r12
		mov r11, 002d002d0021003ch
		mov r12, 003e002d002d0020h
		xor r8, r8					; cur src
		xor rbx, rbx				; count attrs
		call _skip_c
		test rax, rax
		jz _fin
		cmp rax, '<'
		jnz _err1
		mov r10, offset xml_chars
		lea r9, [r8 + 1]
		cmp word ptr [rcx + r9 * 2], '/'
		jnz @f
		inc r9
@@:		mov rax, 1
		call _set_v
		mov r8, r9
		call _skip_s
		; tag
		call _word
		mov rax, 2
		call _set_v
		mov r8, r9
@@:		call _skip_s
		cmp rax, '/'
		jz @f
		cmp rax, '>'
		jz @f
		; attrs - name
		call _word
		lea rax, [rbx + 5]
		call _set_v
		mov r8, r9
		call _skip_s
		cmp rax, '='
		jnz _err
		inc r8
		call _skip_s
		; attr - value
		call _str
		jnc _err1
		inc r8
		lea rax, [rbx + 6]
		call _set_v
		lea r8, [r9 + 1]
		add rbx, 2
		jmp @b
@@:		lea r9, [r8 + 1]
		cmp rax, '/'
		jnz @f
		cmp word ptr [rcx + r9 * 2], '>'
		jnz _err1
		inc r9
@@:		mov rax, 3
		call _set_v
		; value tag
		mov r8, r9
		call _skip_c
		call _str
		jnc @f
		inc r8
		mov rax, 4
		call _set_v
		inc r9
		jmp _ex
@@:		mov dword ptr [rdx + 16], 0
_ex:	xor rax, rax
		movzx r8, word ptr [rdx + 4]
		call _set_v
		lea rax, [rbx + 3]
		ret
_fin:	xor rax, rax
		ret
OPTION EPILOGUE:NONE
_skip_c:call _skip_s
;<!-- sergey -->
		cmp qword ptr [rcx + r8 * 2], r11
		jnz _skip_cc
		add r8, 3
@@:		inc r8
		movzx rax, word ptr [rcx + r8 * 2]
		test rax, rax
		jz _err
		cmp rax, '>'
		jnz @b
		cmp qword ptr [rcx + r8 * 2 - 6], r12
		jnz @b
		inc r8
		call _skip_s
_skip_cc:ret
_skip_s:dec r8
@@:		inc r8
		movzx rax, word ptr [rcx + r8 * 2]
		cmp rax, 33
		jae @f
		test rax, rax
		jnz @b
@@:		ret
_set_v: mov [rdx + rax * 4], r8w
		mov [rdx + rax * 4 + 2], r9w
		ret
_word:	lea r9, [r8 - 1]
		cmp rax, 48
		jb _err
		cmp rax, 58
		jb _err
@@:		inc r9
		movzx rax, word ptr [rcx + r9 * 2]
		cmp rax, 128
		jae @b
		movzx rax, byte ptr [rax + r10]
		test rax, rax		; undef
		jz _err
		dec rax				; word
		jz @b
		ret
_err:	add rsp, 8
_err1:	mov rax, -1
		ret
_str:	cmp rax, 34
		jnz _str1
		mov r9, r8
@@:		inc r9
		movzx rax, word ptr [rcx + r9 * 2]
		test rax, rax
		jz _str1
		cmp rax, 34
		jnz @b
		stc
		ret
_str1:	clc
		ret
OPTION EPILOGUE:EPILOGUEDEF
asm_ssh_parse_xml endp

end

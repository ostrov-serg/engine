
extern malloc:near

.data?

result		dw 128 dup(?)

.data
base64_chars	dw 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90
				dw 97, 98, 99, 100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122
				dw 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 43, 47, 0

.code

; 0 - register(offs), 1 - bit check, 2 - bit set 
asm_ssh_capability proc
		push rbx
		push r10
		xor r10, r10
		mov r9, offset result
		mov r8, offset cpu_caps_1
		mov rax, 1
		xor rcx, rcx
		call _cset
		mov rax, 7
		xor rcx, rcx
		mov r8, offset cpu_caps_7
		call _cset
		mov rax, r10
		pop r10
		pop rbx
		ret
_cset:	cpuid
		mov [r9 + 00], rax
		mov [r9 + 08], rbx
		mov [r9 + 16], rcx
		mov [r9 + 24], rdx
@@:		cmp dword ptr [r8], 0
		jz @f
		movzx rax, byte ptr [r8 + 00]		; offset
		movzx rcx, byte ptr [r8 + 01]		; bit check
		movzx rdx, byte ptr [r8 + 02]		; bit set
		add r8, 4
		mov rax, [r9 + rax * 2]
		bt rax, rcx
		jnc @b
		bts r10, rdx
		jmp @b
@@:		ret
cpu_caps_1	db 8, 0, 4, 0, 8, 1, 8, 0, 8, 9, 5, 0, 8, 12, 9, 0, 8, 13, 10, 0, 8, 19, 6, 0
			db 8, 20, 7, 0, 8, 22, 11, 0, 8, 23, 12, 0, 8, 25, 13, 0, 8, 28, 14, 0, 8, 29, 25, 0, 8, 30, 15, 0
			db 12, 8, 31, 0, 12, 11, 31, 0, 12, 15, 16, 0, 12, 19, 31, 0, 12, 23, 1, 0, 12, 24, 31, 0, 12, 25, 2, 0, 12, 26, 3, 0, 0, 0, 0, 0
cpu_caps_7	db 4, 3, 17, 0, 4, 5, 18, 0, 4, 8, 19, 0, 4, 16, 20, 0, 4, 18, 21, 0, 4, 26, 22, 0, 4, 27, 23, 0, 4, 28, 24, 0, 4, 29, 31, 0, 0, 0, 0, 0
asm_ssh_capability endp

;rcx = buf, rdx = count
asm_ssh_to_base64 proc public
		push rsi
		push rdi
		push rbx
		push r12
		mov r8, 3
		mov rax, rdx
		mov r12, rax				; src_len
		mov rsi, rcx				; src_buf
		xor rdx, rdx
		div r8
		test rdx, rdx
		setnz dl
		lea rcx, [rax + rdx + 2]
		shl rcx, 3					; len_ret
		call malloc
		mov qword ptr [rax], 0
		lea rdi, [rax + 8]			; buf_ret
		push rdi
		mov r8, offset base64_chars
		mov r9, offset result		; char_array_3
_loop:	xor rdx, rdx				; i = 0
@@:		test r12, r12
		jz @f
		dec r12
		lodsb
		mov [r9 + rdx], al
		inc rdx
		cmp rdx, 3
		jnz @b
		call _sub
		jmp _loop
@@:		test rdx, rdx
		jz @f
		mov dword ptr [r9 + rdx], 0
		mov r12, 3
		sub r12, rdx
		call _sub
		sub rdi, r12
		sub rdi, r12
		mov ax, '='
		mov rcx, r12
		rep stosw
@@:		mov word ptr [rdi], 0
		pop rax
		pop r12
		pop rbx
		pop rdi
		pop rsi
		ret
_sub:	movzx rcx, byte ptr [r9 + 00]
		movzx rdx, byte ptr [r9 + 01]
		movzx rbx, byte ptr [r9 + 02]
		mov rax, rcx
		and rax, 0fch
		shr rax, 2
		mov ax, [r8 + rax * 2]
		stosw
		mov rax, rdx
		and rcx, 3
		and rax, 0f0h
		shl rcx, 4
		shr rax, 4
		add rax, rcx
		mov ax, [r8 + rax * 2]
		stosw
		mov rax, rbx
		and rdx, 15
		and rax, 0c0h
		shl rdx, 2
		shr rax, 6
		add rax, rdx
		mov ax, [r8 + rax * 2]
		stosw
		and rbx, 3fh
		mov ax, [r8 + rbx * 2]
		stosw
		ret
asm_ssh_to_base64 endp

; rcx - str, rdx - length, r8 - len_buf
asm_ssh_from_base64 proc
		push rbx
		push rsi
		push rdi
		push r10
		push r12
		mov rsi, rcx			; src
		mov r12, rdx
		mov r10, rsi
		mov rcx, 3dh
		call _wcschr
		cmovc r12, rbx			; in_len
		mov rax, rdx
		shr rax, 2
		imul rax, 3
		sub rdx, r12
		sub rax, rdx			; out_len
		mov [r8], rax
		lea rcx, [rax + 14]		; 8 + 6 корекция на выход за пределы
		call malloc
		mov qword ptr [rax], 0
		lea rdi, [rax + 8]		; out_buf
		push rdi
		mov r10, offset base64_chars
		mov r9, offset result	; char_array_4
_loop:	xor rdx, rdx			; i = 0
@@:		test r12, r12
		jz @f
		dec r12
		lodsw
		mov cx, ax
		call _wcschr
		jnc @f
		mov [r9 + rdx * 2], bx
		inc rdx
		cmp rdx, 4
		jnz @b
		call _sub
		jmp _loop
@@:		test rdx, rdx
		jz @f
		mov qword ptr [r9 + rdx * 2], 0
		call _sub
@@:		mov word ptr [rdi], 0
		pop rax
		pop r12
		pop r10
		pop rdi
		pop rsi
		pop rbx
		ret
_sub:	mov rbx, rdx
		movzx rcx, byte ptr [r9 + 00]
		shl rcx, 2
		movzx rdx, byte ptr [r9 + 02]
		mov rax, rdx
		and rax, 30h
		shr rax, 4
		add rax, rcx
		stosb
		dec rbx
		jle @f
		movzx rcx, byte ptr [r9 + 04]
		and rdx, 15
		shl rdx, 4
		mov rax, rcx
		and rax, 3ch
		shr rax, 2
		add rax, rdx
		stosb
		dec rbx
		jle @f
		mov rax, rcx
		and rax, 3
		shl rax, 6
		add al, [r9 + 06]
		stosb
@@:		ret
_wcschr:mov rbx, -1
@@:		inc rbx
		mov ax, [r10 + rbx * 2]
		test ax, ax
		jz @f
		cmp ax, cx
		jnz @b
		stc
@@:		ret
asm_ssh_from_base64 endp

end

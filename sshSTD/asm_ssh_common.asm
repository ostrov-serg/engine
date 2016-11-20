
.data?

result		dw 128 dup(?)

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

end

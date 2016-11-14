
extern wcslen:near
.data

.code

asm_wcslen proc
		xorps xmm1, xmm1
		mov rax, 0ffffffffffff0000h
		movd xmm1, rax
		mov r8, rcx
		xor rax, rax
@@:		lddqu xmm2, xmmword ptr [r8 + rax * 2]
		pcmpistri xmm1, xmm2, 01110011b
		lea rax, [rax + rcx]
		jnc @b
		ret
asm_wcslen endp

asm_find_substring proc public
		xor rax, rax
		lddqu xmm0, xmmword ptr [rcx]
@@:		pcmpistri xmm0, xmmword ptr [rdx + rax * 2], 00001101b
		lea rax, [rax + rcx]
		jnc @b
		ret
asm_find_substring endp

asm_find_char proc
		movd xmm0, rcx
		xor rax, rax
@@:		pcmpistri xmm0, xmmword ptr [rdx + rax * 2], 00000001b
		lea rax, [rax + rcx]
		jnc @b
		ret
asm_find_char endp

asm_compare_string proc
		mov r8, rcx
		mov r9, rdx
		call wcslen
		mov r10, rax
		mov rcx, r9
		call wcslen
		mov r11, rax
		xor rax, rax
@@:		lddqu xmm0, xmmword ptr [r8 + rax * 2]
		pcmpestri xmm0, xmmword ptr [r9 + rax * 2], 01001001b
		lea rax, [rax + rcx]
		jnc @b
		ret
asm_compare_string endp


end


; подсчитать длину строки
asm_ssh_wcslen macro p, ret
		mov ret, -1
@@:		inc ret
		cmp word ptr [p + ret * 2], 0
		jnz @b
endm

; поиск символа
asm_ssh_wcschr macro p, chr, ret
		sub p, 2
@@:		add p, 2
		movzx ret, word ptr [p]
		test ret, ret
		jz @f
		cmp ret, chr
		jnz @b
		mov ret, p
@@:		
endm

rev macro c, s
		mov r11, rax
		mov r10, c
		and r11, r10
		shl r11, s
		shr rax, s
		and rax, r10
		or rax, r11
endm

.data

hash_tbl dd 512 dup(0)
is_hash	 dd 0

.code

asm_ssh_bits_rev proc USES r10 r11
		rev 55555555h, 1
		rev 33333333h, 2
		rev 0f0f0f0fh, 4
		mov r10, rax
		rol eax, 8
		and rax, 00ff00ffh
		and r10, 00ff00ffh
		ror r10d, 8
		or rax, r10
		ret
asm_ssh_bits_rev endp

asm_ssh_get_hash proc USES r12
		mov r8, rcx
		mov r9, offset hash_tbl
		mov r12, 11EDC6F41h			; polynom
		cmp is_hash, 1
		jz _next
		mov rcx, 255				; i
_loop0: mov rax, rcx
		call asm_ssh_bits_rev
		mov r10, 16
_loop1:	mov r11, rax
		shl rax, 1
		shr r11d, 1
		jge @f
		xor rax, r12
@@:		dec r10
		cmp r10, 8
		jnz @f
		mov r11, rax
		call asm_ssh_bits_rev
		mov [r9 + rcx * 4], eax
		mov rax, r11
@@:		test r10, r10
		jnz _loop1
		call asm_ssh_bits_rev
		mov [r9 + rcx * 4 + 256 * 4], eax
		dec rcx
		jge _loop0
		mov is_hash, 1
_next:	xor rax, rax
_loop2:	movzx rcx, word ptr [r8]
		jrcxz _exit
		xor cx, ax
		shr rax, 16
		movzx rdx, cl
		xor eax, [r9 + rdx * 4 + 256 * 4]
		movzx edx, ch
		xor eax, [r9 + rdx * 4]
		add r8, 2
		jmp _loop2
_exit:	ret
asm_ssh_get_hash endp

; поиск подстроки
; rcx - wstring
; rdx - wsubstring
; ret - rax = index
asm_ssh_wcsstr proc public USES r10 r11 r12 rdi rsi
		mov r8, rcx
		asm_ssh_wcslen rcx, r10
		mov r9, rdx
		asm_ssh_wcslen rdx, r11
		xor rdi, rdi
_next:	xor rsi, rsi
		xor r12, r12
_next_j:cmp rsi, r11
		jae _exit
		mov rcx, r11
		sub rcx, rsi
		shl cl, 1
		mov rdx, 1
		shl rdx, cl
		dec rdx
		lddqu xmm0, xmmword ptr [r9 + rsi * 2]
_next_i:cmp rdi, r10
		jae _exit
		lddqu xmm1, xmmword ptr [r8 + rdi * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb rax, xmm1
		and ax, dx
		cmp ax, dx
		jnz @f
		lea rax, [r8 + rdi * 2]
		test r12, r12
		cmovz r12, rax
		add rdi, 8
		add rsi, 8
		jmp _next_j
@@:		inc rdi
		test r12, r12
		jnz _next
		jmp _next_i
_exit:	mov rax, r12
		ret
asm_ssh_wcsstr endp

; сравнение строк
; rcx - wstring1
; rdx - wstring2
; ret - rax 
asm_ssh_wcscmp proc USES r10 r11
		cmp rdx, rcx
		jz _zero
		mov r8, rcx
		asm_ssh_wcslen rcx, r10
		mov r9, rdx
		asm_ssh_wcslen rdx, r11
		xor rax, rax
@@:		cmp rax, r10
		jae @f
		mov rcx, r10
		sub rcx, rax
		shl rcx, 1
		mov rdx, 1
		shl rdx, cl
		dec rdx
		lddqu xmm0, xmmword ptr [r8 + rax * 2]
		lddqu xmm1, xmmword ptr [r9 + rax * 2]
		pcmpeqw xmm0, xmm1
		add rax, 8
		pmovmskb rcx, xmm0
		and cx, dx
		cmp cx, dx
		jz @b
@@:		sub r10, rax
		sub r11, rax
		cmp r10, r11
		mov rax, -1
		mov rcx, 1
		cmovg rax, rcx
_zero:	jnz @f
		xor rax, rax
@@:		ret
asm_ssh_wcscmp endp

end

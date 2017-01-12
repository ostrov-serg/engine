
; поиск символа SSE4.2
asm_ssh_wcschr macro p, chr, ret
		vmovd xmm0, chr
		xor ret, ret
@@:		vpcmpistri xmm0, xmmword ptr [p], 00000001b
		jz @f
		lea p, [p + rcx * 2]
		jnc @b
		mov ret, p
@@:
endm

; подсчитать длину строки - SSE4.2
asm_ssh_wcslen macro p, ret
		xor ret, ret
@@:		vlddqu xmm0, xmmword ptr [p + ret * 2]
		vpcmpistri xmm0, xmm0, 00010001b
		lea ret, [ret + rcx]
		jnz @b
endm

.code

; поиск подстроки - SSE4.2
; rcx - wstring
; rdx - wsubstring
; ret - rax = index
asm_ssh_wcsstr proc public USES r10 r11 r12 rdi rsi
		mov r10, rcx
		asm_ssh_wcslen r10, r8
		mov r11, rdx
		asm_ssh_wcslen rdx, r9
		xor rdi, rdi		; i
_next:	xor rsi, rsi		; j
		xor r12, r12		; r
_next_j:cmp rsi, r9
		jae @f
		vlddqu xmm0, xmmword ptr [r11 + rsi * 2]
_next_i:cmp rdi, r8
		jae @f
		mov rax, r9
		mov rdx, r8
		sub rax, rsi
		sub rdx, rdi
		vpcmpestri xmm0, xmmword ptr [r10 + rdi * 2], 00001101b
		add rdi, rcx
		jrcxz _find
		test r12, r12
		jnz _next
		jmp _next_i
_find:	lea rax, [r10 + rdi * 2]
		test r12, r12
		cmovz r12, rax
		add rdi, 8
		add rsi, 8
		jmp _next_j
@@:		mov rax, r12
		ret
asm_ssh_wcsstr endp

; сравнение строк  - SSE4.2
; rcx - wstring1
; rdx - wstring2
; ret - rax 
asm_ssh_wcscmp proc USES r10 r11
		cmp rdx, rcx
		jz _zero
		mov r8, rcx
		asm_ssh_wcslen r8, r10
		mov r9, rdx
		asm_ssh_wcslen rdx, r11
		xor rax, rax
@@:		lddqu xmm0, xmmword ptr [r8 + rax * 2]
		vpcmpistri xmm0, xmmword ptr [r9 + rax * 2], 01001001b
		lea rax, [rax + rcx]
		jnc @b
		sub r10, rax
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

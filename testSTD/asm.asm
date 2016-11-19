
extern wcslen:near
.data

.code

asm_ssh_wcslen1 proc
		mov rdx, rcx
		xor rax, rax
@@:		lddqu xmm0, xmmword ptr [rdx + rax * 2]
		pcmpistri xmm0, xmmword ptr [rdx + rax * 2], 00010001b
		lea rax, [rax + rcx]
		jnz @b
		ret
asm_ssh_wcslen1 endp

end

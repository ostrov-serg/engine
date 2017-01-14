
extern wcslen:near
.data

.code

;rcx = text

ssh_asm_wcsstr1 proc USES r10 r11 r12 rsi rdi
ssh_asm_wcsstr1 endp

 ; rcx	- len
 ; rdx	- dst
 ; r8	- src
asm_bwt proc USES rdi
		xor r9, r9
		mov rdi, rdx
		mov r10, rcx
		xor rdx, rdx
_loop:	xor r11, r11
		mov r12, r9
_loop1:	cmp r12, r10
		cmovge r12, rdx
		mov al, [r8 + r12]
		stosb
		inc r12
		inc r11
		cmp r11, r10
		jb _loop1
		xor rax, rax
		stosb
		inc r9
		loop _loop
		ret
asm_bwt endp

asm_ssh_wcslen1 proc
		mov rdx, rcx
		xor rax, rax
@@:		lddqu xmm0, xmmword ptr [rdx + rax * 2]
		pcmpistri xmm0, xmmword ptr [rdx + rax * 2], 00010001b
		lea rax, [rax + rcx]
		jnz @b
		ret
asm_ssh_wcslen1 endp

; r10 - pos
; rcx - length
; 
get_bits proc USES r11 r12
		mov r11, rax
		bsf r10, r11
		mov rcx, r10
		shr r11, cl
		not r11
		bsf r11, r11
		add rcx, r11
		mov r12, 1
		shl r12, cl
		dec r12
		not r12
		and rax, r12
		mov rcx, r11
		ret
get_bits endp

; rcx - buf
; rdx - sz_buf
; r8 - offs
; r9 - ptr dict
asm_strstr proc USES rbx rsi rdi r12 r13
		xor rbx, rbx
		vlddqu xmm0, xmmword ptr [rcx + r8]
		inc r8
		mov r12, rcx
		sub rdx, r8
_next:	cmp r8, rdx
		jae _exit
		vlddqu xmm1, xmmword ptr [r8 + r12]
		vpcmpeqb xmm2, xmm1, xmm0
		vpmovmskb rax, xmm2

range:	test rax, rax
		jz _next1
		call get_bits
		lea r13, [r8 + r10]
		cmp r13, rdx
		jae _next1
		; проверить на выход за пределы
		lea r11, [r13 + rcx]
		cmp r11, rdx
		jbe @f
		sub r11, rdx
		sub rcx, r11
@@:		cmp rcx, 3
		jb range
		and r11, 111111111111b
		and rcx, 1111b
		or r11, rcx
		mov [r9], r11w
		add r9, 2
		add rbx, rcx
		sub rdx, rcx
		lea rdi, [r8 + r10]
		lea rsi, [rdi + rcx]
		mov rcx, rdx
		rep movsb
		jmp _next

_next1:	inc r8
		jmp _next
_exit:	mov rax, rbx
		ret
asm_strstr endp

end

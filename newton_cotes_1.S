.intel_syntax noprefix
.global newton_cotes_1

.data
result: .global float

.text



newton_cotes_1:
	
	push r12
	push r13
	push r14
	mov r12, rdi
	mov r13d, esi
	mov r14d, edx
	
	
	cvtsi2sd xmm0, r13d		
	call r12				
	cvtsd2ss xmm1, xmm0		
	
	cvtsi2sd xmm0, r14d		
	sub r14d, r13d			

	movq r13, xmm1			
	call r12				
	movq xmm1, r13			
	cvtsd2ss xmm0,xmm0		

	mov r10d, 2				
	cvtsi2ss xmm2, r10d		
	cvtsi2ss xmm3, r14d		
	
	addss xmm0, xmm1		
	divss xmm0, xmm2		
	mulss xmm0, xmm3		
	
	
	movss dword ptr ds:[result], xmm0 
	mov rax, result					  									-move Speicheradresse

	
	pop r14
	pop r13
	pop r12
ret
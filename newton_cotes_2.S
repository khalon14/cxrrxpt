.intel_syntax noprefix
.global newton_cotes_2

.data
result: .global double

.text


newton_cotes_2:
	
	push r12
	push r13
	push r14
	mov r12, rdi
	mov r13d, esi
	mov r14d, edx

	cvtsi2sd xmm0, r13d								
	cvtsi2sd xmm1, r14d								
	addsd xmm0, xmm1								

	mov r10d, 2										
	divsd xmm0, xmm1								

	call r12										
	movsd xmm1, xmm0                                
	mov r10d, 4										
	cvtsi2sd xmm2, r10d								
	mulsd xmm1, xmm2								
	movq r8, xmm1 									
	
	cvtsi2sd xmm0, r13d								
	push r8											
	call r12										
	pop r8											
	movsd xmm2, xmm0								
	
	movq r9, xmm2									
	cvtsi2sd xmm0, r14d								
	push r8											
	push r9										
	call r12                                      	
	pop r9											
	pop r8											
	
	movq xmm1, r8									
	movq xmm2, r9									
	addsd xmm1, xmm2								
	addsd xmm1, xmm0								

	mov r10d, 6										
	cvtsi2sd xmm2, r10d								
	divsd xmm1, xmm2								
	sub r14d, r13d									
	cvtsi2sd xmm2, r14d								
	mulsd xmm1, xmm2								

	movsd qword ptr ds:[result], xmm1				
	mov rax, result									

	
	pop r14
	pop r13
	pop r12
ret
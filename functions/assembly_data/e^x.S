.intel_syntax noprefix
.global f
.text

# Compile with: gcc -shared e ^ x.S -o e ^ x.so
# Input and return value in xmm0!
# Calculation of Euler's number from sheet 5 H1 + H2 adapted

f:	
	cvtsd2si r10d, xmm0	
	
	cmp r10d, 0			
	jl .negative		
	je .retone			
	
	mov r9, 9			
	mov r8, 1
	
	cvtsi2sd xmm0, r8 	
	cvtsi2sd xmm2, r8 	
	cvtsi2sd xmm3, r8 	
	mov rcx, 1			
	call .loop			
	call .power			
	jmp .end			
	
.loop:					
	cvtsi2sd xmm1, rcx
	movsd xmm4, xmm3
	divsd xmm4, xmm1
	mulsd xmm2, xmm4
	addsd xmm0, xmm2
	
	inc rcx 			
	cmp rcx, r9			
	jle .loop

	movsd xmm5, xmm0	
	ret
	
.power:
	cmp r10d, 1			
	jle .end			
	
	
	mulsd xmm0, xmm5
	dec r10d
	ret

.negative:
	# xmm0 = bereits negative Eingabe

	mov r10d, -1		# r10d = -1
	cvtsi2sd xmm1, r10d # xmm1 = double -1.0
	mulsd xmm0, xmm1	# xmm0 *= -1 = positiver x-Wert
	
	cvtsd2si r10d, xmm0
	mov r9, 9			# n = 9 liefert e auf 5 Nachkommastellen genau (2.718282)
	mov r8, 1			# r8 = integer 1
	
	cvtsi2sd xmm0, r8 	# xmm0 = Summe
	cvtsi2sd xmm2, r8 	# xmm2 = 1/(k-1)!
	cvtsi2sd xmm3, r8 	# xmm3 = double 1
	mov rcx, 1			# rcx = k
	
	call .loop			# e berechnen
	call .power			# e^x berechnen
	
	mov r10d, 1			# r10d = integer 1
	cvtsi2sd xmm1, r10d	# xmm1 = double 1.0
	divsd xmm1, xmm0 	# (1 / e^x) berechnen = e^-x
	movsd xmm0, xmm1	# Ergebnis in xmm0
	jmp .end

.retone:
	mov r10d, 1			# integer 1 in r10d laden
	cvtsi2sd xmm0, r10d # xmm0 = double 1.0
	jmp .end
	
.end:
ret
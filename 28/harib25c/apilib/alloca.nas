[BITS 32]

	GLOBAL	alloca

[SECTION .text]

alloca: ; void *alloca(int size)
	ADD			EAX,-4
	SUB			ESP,EAX
	JMP			DWORD [ESP+EAX] ; RET

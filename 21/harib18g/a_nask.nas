[BITS 32]
	GLOBAL	api_putchar
	GLOBAL	api_end

[SECTION .text]

api_putchar:
	MOV		EDX,1
	MOV	 AL,[ESP+4]
	INT	 0x40
	RET

api_end:
	MOV	 EDX,4
	INT		0x40

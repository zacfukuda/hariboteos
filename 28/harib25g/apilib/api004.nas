[BITS 32]

	GLOBAL	api_end

[SECTION .text]

api_end:
	MOV		EDX,4
	INT		0x40

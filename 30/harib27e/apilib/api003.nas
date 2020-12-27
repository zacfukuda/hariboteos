[BITS 32]

	GLOBAL	api_putstr1

[SECTION .text]

api_putstr1: ; void api_putstr1(char *s, int l);
	PUSH	EBX
	MOV		EDX,3
	MOV		EBX,[ESP+ 8] ; s
	MOV		ECX,[ESP+12] ; l
	INT		0x40
	POP		EBX
	RET

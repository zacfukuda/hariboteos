[BITS 32]

	GLOBAL	api_closewin

[SECTION .text]

api_closewin: ; void api_closewin(int win);
	PUSH	EBX
	MOV		EDX,14
	MOV		EBX,[ESP+8] ; win
	INT		0x40
	POP		EBX
	RET

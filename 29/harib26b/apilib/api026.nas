[BITS 32]

	GLOBAL	api_cmdline

[SECTION .text]

api_cmdline: ; int api_cmdline(char *buf, int maxsize)
	PUSH	EBX
	MOV		EDX,26
	MOV		ECX,[ESP+12] ; maxsize
	MOV		EBX,[ESP+8] ; buf
	INT		0x40
	POP		EBX
	RET

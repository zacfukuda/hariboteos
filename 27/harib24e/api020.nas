[BITS 32]

	GLOBAL	api_beep

[SECTION .text]

api_beep: ; void api_beep(int tone)
	MOV		EDX,20
	MOV		EAX,[ESP+4] ; tone
	INT		0x40
	RET

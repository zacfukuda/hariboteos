[BITS 32]

	GLOBAL	api_settimer

[SECTION .text]

api_settimer: ; void api_settimer(int timer, int time)
	PUSH	EBX
	MOV		EDX,18
	MOV		EBX,[ESP+8] ; timer
	MOV		EAX,[ESP+12] ; time
	INT		0x40
	POP		EBX
	RET

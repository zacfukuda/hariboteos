; naskfunc
; TAB=4

[BITS 32]

GLOBAL	io_hlt, io_cli, io_sti, io_stihlt
GLOBAL	io_in8,  io_in16,  io_in32
GLOBAL	io_out8, io_out16, io_out32
GLOBAL	io_load_eflags, io_store_eflags

[SECTION .text]

io_hlt:
	HLT
	RET

io_cli:
	CLI
	RET

io_sti:
	STI
	RET

io_stihlt:
	STI
	HLT
	RET

io_in8:
	MOV		EDX,[ESP+4]
	MOV		EAX,0
	IN		AL,DX
	RET

io_in16:
	MOV		EDX,[ESP+4]
	MOV		EAX,0
	IN		AX,DX
	RET

io_in32:
	MOV		EDX,[ESP+4]
	IN		EAX,DX
	RET

io_out8:
	MOV		EDX,[ESP+4]
	MOV		AL,[ESP+8]
	OUT		DX,AL
	RET

io_out16:
	MOV		EDX,[ESP+4]
	MOV		EAX,[ESP+8]
	OUT		DX,AX
	RET

io_out32:
	MOV		EDX,[ESP+4]
	MOV		EAX,[ESP+8]
	OUT		DX,EAX
	RET

io_load_eflags:
		PUSHFD
		POP		EAX
		RET

io_store_eflags:
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD
		RET

; naskfunc
; TAB=4

[BITS 32]

	GLOBAL	io_hlt, io_cli, io_sti, io_stihlt
	GLOBAL	io_in8,  io_in16,  io_in32
	GLOBAL	io_out8, io_out16, io_out32
	GLOBAL	io_load_eflags, io_store_eflags
	GLOBAL	load_gdtr, load_idtr
	GLOBAL	load_cr0, store_cr0
	GLOBAL	load_tr
	GLOBAL	asm_inthandler20, asm_inthandler21
	GLOBAL	asm_inthandler27, asm_inthandler2c
	GLOBAL	memtest_sub
	GLOBAL	farjmp, farcall
	GLOBAL	asm_cons_putchar
	EXTERN	inthandler20, inthandler21, inthandler27, inthandler2c
	EXTERN	cons_putchar

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

load_gdtr:
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr:
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

load_cr0:
		MOV		EAX,CR0
		RET

store_cr0:
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

load_tr:
		LTR		[ESP+4]
		RET

asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler20
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

memtest_sub:
		PUSH	EDI ; For (EBX,ESI,EDI)
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55 ; pat0 = 0xaa55aa55
		MOV		EDI,0x55aa55aa ; pat1 = 0x55aa55aa
		MOV		EAX,[ESP+12+4] ; i = start
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc	; p = i + 0xffc
		MOV		EDX,[EBX] ; old = *p
		MOV		[EBX],ESI	; *p = pat0
		XOR		DWORD [EBX],0xffffffff ; *p ^= 0xffffffff
		CMP		EDI,[EBX] ; if (*p != pat1) goto fin
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff ; *p ^= 0xffffffff
		CMP		ESI,[EBX] ; if (*p != pat0) goto fin
		JNE		mts_fin
		MOV		[EBX],EDX	; *p = old
		ADD		EAX,0x1000 ; i += 0x1000
		CMP		EAX,[ESP+12+8] ; if (i <= end) goto mts_loop
		JBE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX	; *p = old
		POP		EBX
		POP		ESI
		POP		EDI
		RET

farjmp:
		JMP		FAR	[ESP+4]
		RET

farcall:
		CALL	FAR [ESP+4]
		RET

asm_cons_putchar:
		STI
		PUSH	1
		AND		EAX,0xff
		PUSH	EAX
		PUSH	DWORD [0x0fec]
		CALL	cons_putchar
		ADD		ESP,12
		IRETD

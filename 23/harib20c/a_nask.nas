[BITS 32]
	GLOBAL	api_putchar
	GLOBAL	api_putstr0
	GLOBAL	api_end
	GLOBAL	api_openwin
	GLOBAL	api_putstrwin
	GLOBAL	api_boxfilwin
	GLOBAL	api_initmalloc
	GLOBAL	api_malloc
	GLOBAL	api_free
	GLOBAL	api_point
	GLOBAL	api_refreshwin

[SECTION .text]

api_putchar: ; void api_putchar(int c)
	MOV		EDX,1
	MOV	 AL,[ESP+4] ; c
	INT	 0x40
	RET

api_putstr0: ; void api_putstr0(char *s)
	PUSH	EBX
	MOV		EDX,2
	MOV		EBX,[ESP+8] ; s
	INT		0x40
	POP		EBX
	RET

api_end:
	MOV	 EDX,4
	INT		0x40

api_openwin: ; int api_openwin(char *buf, int xsiz, int ysiz, int col_inv, char *title)
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,5
	MOV		EBX,[ESP+16] ; buf
	MOV		ESI,[ESP+20] ; xsiz
	MOV		EDI,[ESP+24] ; ysiz
	MOV		EAX,[ESP+28] ; col_inv
	MOV		ECX,[ESP+32] ; title
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET

api_putstrwin: ; void api_putstrwin(int win, int x, int y, int col, int len, char *str)
	PUSH	EDI
	PUSH	ESI
	PUSH	EBP
	PUSH	EBX
	MOV		EDX,6
	MOV		EBX,[ESP+20] ; win
	MOV		ESI,[ESP+24] ; x
	MOV		EDI,[ESP+28] ; y
	MOV		EAX,[ESP+32] ; col
	MOV		ECX,[ESP+36] ; len
	MOV		EBP,[ESP+40] ; str
	INT		0x40
	POP		EBX
	POP		EBP
	POP		ESI
	POP		EDI
	RET

api_boxfilwin: ; void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col)
	PUSH	EDI
	PUSH	ESI
	PUSH	EBP
	PUSH	EBX
	MOV		EDX,7
	MOV		EBX,[ESP+20] ; win
	MOV		EAX,[ESP+24] ; x0
	MOV		ECX,[ESP+28] ; y0
	MOV		ESI,[ESP+32] ; x1
	MOV		EDI,[ESP+36] ; y1
	MOV		EBP,[ESP+40] ; col
	INT		0x40
	POP		EBX
	POP		EBP
	POP		ESI
	POP		EDI
	RET

api_initmalloc: ; void api_initmalloc(void)
	PUSH	EBX
	MOV		EDX,8
	MOV		EBX,[CS:0x0020]
	MOV		EAX,EBX
	ADD		EAX,32*1024
	MOV		ECX,[CS:0x0000]
	SUB		ECX,EAX
	INT		0x40
	POP		EBX
	RET

api_malloc: ; char *api_malloc(int size)
	PUSH	EBX
	MOV		EDX,9
	MOV		EBX,[CS:0x0020]
	MOV		ECX,[ESP+8]
	INT		0x40
	POP		EBX
	RET

api_free: ; void api_free(char *addr, int size)
	PUSH	EBX
	MOV		EDX,10
	MOV		EBX,[CS:0x0020]
	MOV		EAX,[ESP+ 8]
	MOV		ECX,[ESP+12]
	INT		0x40
	POP		EBX
	RET

api_point: ; void api_point(int win, int x, int y, int col)
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,11
	MOV		EBX,[ESP+16] ; win
	MOV		ESI,[ESP+20] ; x
	MOV		EDI,[ESP+24] ; y
	MOV		EAX,[ESP+28] ; col
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET

api_refreshwin: ; void api_refreshwin(int win, int x0, int y0, int x1, int y1);
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,12
	MOV		EBX,[ESP+16] ; win
	MOV		EAX,[ESP+20] ; x0
	MOV		ECX,[ESP+24] ; y0
	MOV		ESI,[ESP+28] ; x1
	MOV		EDI,[ESP+32] ; y1
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET

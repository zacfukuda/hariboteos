[BITS 32]
	GLOBAL	HariMain

[SECTION .text]

HariMain:
	MOV		EDX,2
	MOV		EBX,msg
	INT		0x40
	MOV		EDX,4
	INT		0x40

[SECTION .data]

msg:
	DB	"hello, world", 0x0a, 0

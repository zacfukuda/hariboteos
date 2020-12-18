[BITS 32]

	GLOBAL	HariMain

[SECTION .text]

HariMain:
	MOV		AX,1005*8
	MOV		DS,AX
	CMP		DWORD [DS:0x0004],'Hari'
	JNE		fin ; Not app, do nothing

	MOV		ECX,[DS:0x0000] ; Read the segment size of app
	MOV		AX,2005*8
	MOV		DS,AX

crackloop: ; Fill with 123
	ADD		ECX,-1
	MOV		BYTE [DS:ECX],123
	CMP		ECX,0
	JNE		crackloop

fin:
	MOV		EDX,4
	INT		0x40

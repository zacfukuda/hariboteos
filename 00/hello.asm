; Reference: https://cs.lmu.edu/~ray/notes/nasmtutorial/
;
; Commands:
; nasm -f macho64 hello.asm
; gcc hello.o -o hello.out -Wl,-no_pie
; ./hello.out

		global	_main

		section	.text

_main:
		mov			rax, 0x02000004 ; system call for write
		mov			rdi, 1 ; file handle 1 is stdout
		mov			rsi, _message ; address of string to output
		mov			rdx, 13 ; number of bytes
		syscall ; invoke operating system to do the write
		mov			rax, 0x02000001 ; system call for exit
		xor			rdi, rdi ; exit code 0
		syscall

section			.data

_message:
		db			"Hello, World", 10 ; note the newline at the end

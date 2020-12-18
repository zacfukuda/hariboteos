; https://cs.lmu.edu/~ray/notes/nasmtutorial/

; Commands:
; nasm -fmacho64 hola.asm
; gcc hola.o -o hola.out -Wl,-no_pie
; ./hola.out

		global		_main
		extern		_puts

		section		.text

_main:
		push		rbx ; Call stack must be aligned
		lea			rdi, [rel _message] ; First argument is address of message
		call		_puts
		pop			rbx ; Fix up stack before returning
		ret

		section		.data

_message:
		db			"Hola, mundo", 0 ; C strings need a zero byte at the end

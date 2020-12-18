; https://cs.lmu.edu/~ray/notes/nasmtutorial/

; Commands:
; nasm -fmacho64 echo.asm
; gcc echo.o -o echo.out -Wl,-no_pie
; ./echo.out dog 22 -zzz "hi there"

		global		_main
		extern		_puts

		section		.text

_main:
		push			rdi ; save registers that puts uses
		push			rsi
		sub				rsp, 8 ; must align stack before call

		mov				rdi, [rsi] ; the argument string to display
		call			_puts ; print it

		add				rsp, 8 ; restore %rsp to pre-aligned value
		pop				rsi ; restore registers puts used
		pop				rdi

		add				rsi, 8 ; point to next argument
		dec				rdi ; count down
		jnz				_main ; if not done counting keep going

		ret

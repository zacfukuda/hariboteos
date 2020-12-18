; Reference: https://cs.lmu.edu/~ray/notes/nasmtutorial/
;
; The order in which registers are allocated for integer:
; rdi -> rsi -> rdx -> rcx -> r8 -> r9

; Commands:
; nasm -fmacho64 fib.asm
; gcc fib.o -o fib.out -Wl,-no_pie
; ./fib.out

		global		_main
		extern		_printf

		section		.text
_main:
		push			rbx ; we have to save this since we use it
		mov				ecx, 90 ; ecx will countdown to 0
		xor				rax, rax ; rax will hold the current number
		xor				rbx, rbx ; rbx will hold the next number
		inc				rbx ; rbx is originally 1
_print:
		push			rax ; caller-save register
		push			rcx ; caller-save register

		mov				rdi, _format ; set 1st parameter (format)
		mov				rsi, rax ; set 2nd parameter (current_number)
		xor				rax, rax ; because printf is varargs

		call			_printf ; printf(format, current_number)

		pop				rcx ; restore caller-save register
		pop				rax ; restore caller-save register

		mov				rdx, rax ; save the current number
		mov				rax, rbx ; next number is now current
		add				rbx, rdx ; get the new next number
		dec				ecx ; count down
		jnz				_print ; if not done counting, do some more
		
		pop				rbx ; restore rbx before returning
		ret
_format:
		db				"%20ld", 10, 0

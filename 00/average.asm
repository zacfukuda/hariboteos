; https://cs.lmu.edu/~ray/notes/nasmtutorial/
;
; Commands
; nasm -f macho64 average.asm
; gcc average.o -o average.out
; ./average.out 54.3 -4 -3 -25 455.111

		global		_main
		extern		_atoi
		extern		_printf
		default		rel

		section		.text
_main:
		push			rbx ; we don't ever use this, but it is necesary
									; to align the stack so we can call stuff
		dec				rdi ; argc-1, since we don't count program name
		jz				nothingToAverage
		mov				[count], rdi ; save number of real arguments
accumulate:
		push			rdi ; save register across call to atoi
		push			rsi
		mov				rdi, [rsi+rdi*8] ; argv[rdi]
		call			_atoi ; now rax has the int value of arg
		pop				rsi ; restore registers after atoi call
		pop				rdi
		add				[sum], rax
		dec				rdi ; count down
		jnz				accumulate
average:
		cvtsi2sd	xmm0, [sum]
		cvtsi2sd	xmm1, [count]
		divsd			xmm0, xmm1 ; xmm0 is sum/count
		lea				rdi, [format] ; 1st arg to printf
		mov				rax, 1 ; printf is varargs, there is 1 non-int argument
		call			_printf ; printf(format, sum/count)
		jmp				done

nothingToAverage:
		lea			rdi, [error]
		mov			rax, rax
		call		_printf

done:
		pop			rbx ; undoes the stupid push at the beginning
		ret

		section	.data
count:	dq	0
sum:		dq	0
format: db	"%g", 10, 0
error:	db	"There are no command line arguments to average", 10, 0

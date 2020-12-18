; Reference: https://cs.lmu.edu/~ray/notes/nasmtutorial/
;
; Commands:
; nasm -f macho64 triangle.asm
; gcc triangle.o -o triangle.out -Wl,-no_pie
; ./triangle.out

		global		_main
		section		.text
_main:
		mov				rdx, _output ; rdx holds address of next byte to write
		mov				r8, 1 ; initial line length
		mov				r9, 0 ; number of stars written on line so far
_line:
		mov				byte [rdx], '*' ; write single star
		inc				rdx ; advance pointer to next cell to write
		inc				r9 ; "count" number so far on line
		cmp				r9, r8 ; did we reach the number of stars for this line?
		jne				_line ; not yet, keep writing on this line
_lineDone:
		mov				byte [rdx], 10 ; write a new line char
		inc				rdx ; and move pointer to where next char goes
		inc				r8 ; next line will be one char longer
		mov				r9, 0 ; reset count of stars written on this line
		cmp				r8, _maxlines ; wait, did we already finish the last line?
		jng				_line ; if not, begin writing this line
_done:
		mov				rax, 0x02000004 ; system call for write
		mov				rdi, 1 ; file handle 1 is stdout
		mov				rsi, _output ; address of string to output
		mov				rdx, _dataSize ; number of bytes
		syscall ; invoke operating system to do the write
		mov				rax, 0x02000001 ; system call for exit
		xor				rdi, rdi ; exit code 0
		syscall ; invoke operating system to exit

		section		.bss
_maxlines			equ 8
_dataSize			equ 44
_output:
		resb 			_dataSize

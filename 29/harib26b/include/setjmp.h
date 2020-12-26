/**
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Nonlocal-Gotos.html
 * https://en.wikipedia.org/wiki/Setjmp.h
 * https://man7.org/linux/man-pages/man3/setjmp.3.html 
 */

typedef int jmp_buf[3]; /* EBP, EIP, ESP */

// #define setjmp(env)			__builtin_setjmp(env)
// #define longjmp(env, val)	__builtin_longjmp(env, val)

// Call __builtin_xx directry in tek.c,
// definitions above don't work

// https://opensource.apple.com/source/Libc/Libc-167/gen.subproj/i386.subproj/strlen.c.auto.html
#include <stddef.h>

size_t strlen(char *str) {
	char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

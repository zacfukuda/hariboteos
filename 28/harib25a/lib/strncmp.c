// https://opensource.apple.com/source/Libc/Libc-167/gen.subproj/i386.subproj/strncmp.c.auto.html
int strncmp(char *s1, char *s2, int n) {
	if (n == 0) { return 0; }
		
	do {
		if (*s1 != *s2++) {
			return (*(unsigned char *)s1 - *(unsigned char *)--s2);
		}
		if (*s1++ == 0) { break; }
	} while (--n != 0);
	return 0;
}

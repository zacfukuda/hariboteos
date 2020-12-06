// https://opensource.apple.com/source/Libc/Libc-262/ppc/gen/strcmp.c.auto.html
int strcmp(char *s1, char *s2) {
	for ( ; *s1 == *s2; s1++, s2++) {
		if (*s1 == '\0') { return 0; }
	}
	return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

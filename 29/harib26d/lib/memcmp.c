/*
 * Reference:
 * HariboteOS_source/omake/tolsrc/go_0023s/golibc/memcmp.c
 * https://github.com/gcc-mirror/gcc/blob/master/libiberty/memcmp.c
 * https://opensource.apple.com/source/tcl/tcl-3.1/tcl/compat/memcmp.c
 */

#include <stddef.h>

int memcmp (const void *d, const void *s, size_t sz) {
	const char *dp = (const char*) d;
	const char *sp = (const char*) s;
	while (sz--) {
		if (*dp != *sp)
			return *dp - *sp;
		dp++;
		sp++;
	}
	return 0;
}

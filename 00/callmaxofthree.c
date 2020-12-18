/*
 * https://cs.lmu.edu/~ray/notes/nasmtutorial/
 *
 * Commands:
 * nasm -fmacho64 maxofthree.asm
 * gcc callmaxofthree.c maxofthree.o -o callmaxofthree.out
 * ./callmaxofthree.out
 */

#include <stdio.h>
#include <inttypes.h>

int64_t maxofthree(int64_t, int64_t, int64_t);

int main() {
	printf("%lld\n", maxofthree(1, -4, -7));
	printf("%lld\n", maxofthree(2, -6, 1));
	printf("%lld\n", maxofthree(2, 3, 1));
	printf("%lld\n", maxofthree(-2, 4, 3));
	printf("%lld\n", maxofthree(2, -6, 5));
	printf("%lld\n", maxofthree(2, 4, 6));
	return 0;
}

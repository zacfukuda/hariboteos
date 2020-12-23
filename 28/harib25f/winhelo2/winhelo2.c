#include "../apilib.h"

void HariMain(void) {
	int win;
	// char buf[150 * 50];
	char *buf = alloca(150 * 50);
	win = api_openwin(buf, 150, 50, -1, "hello");
	api_boxfilwin(win, 8, 36, 141, 43, 3);
	api_putstrwin(win, 28, 28, 0, 12, "hello, world");
	for (;;) {
		if (api_getkey(1) == 0x0a) { break; }
	}
	api_end();
}

#include "bootpack.h"

unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	struct MOUSE_DEC mdec;

	init_gdtidt();
	init_pic();
	io_sti(); // Undo interrupt prevention

	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); // Arrow PIC1&keyboard (11111001)
	io_out8(PIC1_IMR, 0xef); // Arrow mouse (11101111)

	init_keyboard();
	enable_mouse(&mdec);

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	i = memtest(0x00400000, 0xbfffffff) / (1024 * 1024);
	sprintf(s, "memory %dMB", i);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%x", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo) != 0) {
				i = fifo8_get(&mousefifo);
				io_sti();
				// 3-byte of data chunked
				if (mouse_decode(&mdec, i) != 0) {
					sprintf(s, "[lcr %d %d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0) { s[1] = 'L'; }
					if ((mdec.btn & 0x02) != 0) { s[3] = 'R'; }
					if ((mdec.btn & 0x04) != 0) { s[2] = 'C'; }
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
					// Mouse movement
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15); // Remove mouse
					mx += mdec.x;
					my += mdec.y;

					// Prevent mouse going out of display
					if (mx < 0) { mx = 0; }
					if (my < 0) {	my = 0; }
					if (mx > binfo->scrnx - 16) { mx = binfo->scrnx - 16; }
					if (my > binfo->scrny - 16) { my = binfo->scrny - 16; }

					sprintf(s, "(%d, %d)", mx, my);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15); // Delete coordinate
					putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s); // Write coordinate
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); // Draw mouse
				}				
			}
		}
	}
}

#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;

	// Check if 386 or later
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; // AC-bit = 1
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	// On 386, AC-bit automatically go back to 0
	if ((eflg & EFLAGS_AC_BIT) != 0) {
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; // AC-bit = 0
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; // Disable cache
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; // Enable cache
		store_cr0(cr0);
	}

	return i;
}

unsigned int memtest_sub(unsigned int start, unsigned int end)
{
	unsigned i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
	for (i = start; i <= end; i += 0x1000) {
		p = (unsigned int *) (i + 0xffc);
		old = *p; // Remember the first value
		*p = pat0; // Try writing
		*p ^= 0xffffffff; // Reverse value

		// Does reverse work?
		if (*p != pat1) {
			not_memory:
			*p = old;
			break;
		}
		*p ^= 0xffffffff; // Reverse again

		// Is it original?
		if (*p != pat0) {
			goto not_memory;
		}
		*p = old; // Restore original value
	}
	return i;
}

#include "bootpack.h"

#define MEMMAN_FREES 4090 // About 32KB
#define MEMMAN_ADDR 0x003c0000 // 0x003c0000

// Information about free memory
struct FREEINFO {
	unsigned int addr, size;
};

// Memory Manager
struct MEMMAN {
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

	init_gdtidt();
	init_pic();
	io_sti(); // Undo interrupt prevention

	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); // Arrow PIC1&keyboard (11111001)
	io_out8(PIC1_IMR, 0xef); // Arrow mouse (11101111)
	// Keyboard & mouse
	init_keyboard();
	enable_mouse(&mdec);
	// Memory
	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	sprintf(s, "memory %dMB  free : %dKB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
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

void memman_init(struct MEMMAN *man)
{
	man->frees = 0;
	man->maxfrees = 0;
	man->lostsize = 0;
	man->losts = 0;
	return;
}

// Total amount of free memory
unsigned int memman_total(struct MEMMAN *man)
{
	unsigned int i, t = 0;
	for (i = 0; i < man->frees; i++) {
		t += man->free[i].size;
	}
	return t;
}

// Allocation
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size)
{
	unsigned int i, a;
	for (i = 0; i < man->frees; i++) {
		// Sufficient free memory
		if (man->free[i].size >= size) {
			a = man->free[i].addr;
			man->free[i].addr += size;
			man->free[i].size -= size;
			// No more free space
			if (man->free[i].size == 0) {
				man->frees--;
				for (; i < man->frees; i++) {
					man->free[i] = man->free[i + 1];
				}
			}
			return a;
		}
	}
	return 0; // No free memory
}

// Release memory
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
{
	int i, j;
	// Order by address
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	// free[i - 1].addr < addr < free[i].addr
	
	// Has preceding
	if (i > 0) {
		// Can release preceding
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			man->free[i - 1].size += size;
			// Also, has follower
			if (i < man->frees) {
				// Can release follwer
				if (addr + size == man->free[i].addr) {
					man->free[i - 1].size += man->free[i].size;
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1];
					}
				}
			}
			return 0; // Success
		}
	}
	// Cannot release preceding
	if (i < man->frees) {
		// Has follower
		if (addr + size == man->free[i].addr) {
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; // Success
		}
	}
	// Cannot release preceding & follower
	if (man->frees < MEMMAN_FREES) {
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees;
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; // Success
	}
	// Cannot release follower
	man->losts++;
	man->lostsize += size;
	return -1; // Failure
}

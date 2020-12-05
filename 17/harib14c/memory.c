#include "bootpack.h"

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

unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size) {
  unsigned int a;
  size = (size + 0xfff) & 0xfffff000;
  a = memman_alloc(man, size);
  return a;
}

int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size) {
  int i;
  size = (size + 0xfff) & 0xfffff000;
  i = memman_free(man, addr, size);
  return i;
}

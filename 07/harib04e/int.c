#include "bootpack.h"

// Initialization of PIC
void init_pic()
{
  io_out8(PIC0_IMR, 0xff); // Reject all interrupt
  io_out8(PIC1_IMR, 0xff); // Reject all interrupt

  io_out8(PIC0_ICW1, 0x11); // Edge Trigger Mode
  io_out8(PIC0_ICW2, 0x20); // IRQ0-7 -> INT20-27
  io_out8(PIC0_ICW3, 1 << 2); // PIC1 to IRQ2

  io_out8(PIC1_ICW1, 0x11); // Edge Trigger Mode
  io_out8(PIC1_ICW2, 0x28); // IRQ8-15 -> INT28-2f
  io_out8(PIC1_ICW3, 2); // PIC1 to IRQ2
  io_out8(PIC1_ICW4, 0x01); // None Buffering Mode

  io_out8(PIC0_IMR, 0xfb); // Except PIC1
  io_out8(PIC1_IMR, 0xff); // Reject all interrupt

  return;
}

#define PORT_KEYDAT 0x0060

struct FIFO8 keyfifo;

// Interrupt from PS/2 Keyboard
void inthandler21(int *esp)
{
  unsigned char data;
  io_out8(PIC0_OCW2, 0x61); // Notify IRQ-01 reception done
  data = io_in8(PORT_KEYDAT);
  fifo8_put(&keyfifo, data);
  return;
}

// Interrupt from PS/2 Mouse
void inthandler2c(int *esp)
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
  for (;;) {
    io_hlt();
  }
}

// Prevention for unsuccessful interrupt
void inthandler27(int *esp)
{
  io_out8(PIC0_OCW2, 0x67);
  return;
}

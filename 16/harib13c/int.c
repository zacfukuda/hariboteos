#include "bootpack.h"

// Initialization of PIC
void init_pic()
{
  io_out8(PIC0_IMR, 0xff); // Reject all interrupt
  io_out8(PIC1_IMR, 0xff); // Reject all interrupt

  io_out8(PIC0_ICW1, 0x11); // Edge Trigger Mode
  io_out8(PIC0_ICW2, 0x20); // IRQ0-7 -> INT20-27
  io_out8(PIC0_ICW3, 1 << 2); // PIC1 to IRQ2
  io_out8(PIC0_ICW4, 0x01);

  io_out8(PIC1_ICW1, 0x11); // Edge Trigger Mode
  io_out8(PIC1_ICW2, 0x28); // IRQ8-15 -> INT28-2f
  io_out8(PIC1_ICW3, 2); // PIC1 to IRQ2
  io_out8(PIC1_ICW4, 0x01); // None Buffering Mode

  io_out8(PIC0_IMR, 0xfb); // Except PIC1
  io_out8(PIC1_IMR, 0xff); // Reject all interrupt

  return;
}

#define PORT_KEYDAT 0x0060

// Prevention for unsuccessful interrupt
void inthandler27(int *esp)
{
  io_out8(PIC0_OCW2, 0x67);
  return;
}

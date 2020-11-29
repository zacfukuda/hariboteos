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

struct FIFO8 mousefifo;

// Interrupt from PS/2 Mouse
void inthandler2c(int *esp)
{
  unsigned char data;
  io_out8(PIC1_OCW2, 0x64); // Notify IRQ-12 reception done to PIC1
  io_out8(PIC0_OCW2, 0x62); // Notify IRQ-02 reception done to PIC0
  data = io_in8(PORT_KEYDAT);
  fifo8_put(&mousefifo, data);
  return;
}

// Prevention for unsuccessful interrupt
void inthandler27(int *esp)
{
  io_out8(PIC0_OCW2, 0x67);
  return;
}

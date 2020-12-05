#include "bootpack.h"

#define FLAGS_OVERRUN 0x0001

// Reset FIFO buffer
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task) {
  fifo->size = size;
  fifo->buf = buf;
  fifo->free = size;
  fifo->flags = 0;
  fifo->p = 0; // Write index
  fifo->q = 0; // Read index
  fifo->task = task; // Data to wake up a task
  return;
}

// Write data to FIFO
int fifo32_put(struct FIFO32 *fifo, int data) {
  if (fifo->free == 0) {
    fifo->flags |= FLAGS_OVERRUN;
    return -1;
  }
  fifo->buf[fifo->p] = data;
  fifo->p++;
  if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
  fifo->free--;
  if (fifo->task != 0) {
    if (fifo->task->flags != 2) { // If the task is sleeping,
      task_run(fifo->task, 0); // Wake that up
    }
  }
  return 0;
}

// Read data from FIFO
int fifo32_get(struct FIFO32 *fifo) {
  int data;
  if (fifo->free == fifo->size) {
    return -1;
  }
  data = fifo->buf[fifo->q];
  fifo->q++;
  if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
  fifo->free++;
	return data;
}

// Report usage of FIFO
int fifo32_status(struct FIFO32 *fifo)
{
  return fifo->size - fifo->free;
}

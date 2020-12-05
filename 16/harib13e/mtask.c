#include "bootpack.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

struct TASK *task_now(void) {
  struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
  return tl->tasks[tl->now];
}

void task_add(struct TASK *task) {
  struct TASKLEVEL *tl = &taskctl->level[task->level];
	tl->tasks[tl->running] = task;
	tl->running++;
  task->flags = 2;
  return;
}

void task_remove(struct TASK *task) {
  int i;
	struct TASKLEVEL *tl = &taskctl->level[task->level];

  // Find the task
  for (i = 0; i < tl->running; i++) {
		if (tl->tasks[i] == task) { break; }
  }

  tl->running--;
  if (i < tl->now) { tl->now--; }
  if (tl->now >= tl->running) { tl->now = 0; }
  task->flags = 1; // Sleeo

  // Re-order
  for (; i < tl->running; i++) {
		tl->tasks[i] = tl->tasks[i + 1];
	}

  return;
}

void task_switchsub(void) {
  int i;
  // Find the highest level
  for (i = 0; i < MAX_TASKLEVELS; i++) {
		if (taskctl->level[i].running > 0) { break; }
  }
  taskctl->now_lv = i;
  taskctl->lv_change = 0;
  return;
}

struct TASK *task_init(struct MEMMAN *memman) {
  int i;
	struct TASK *task;
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
  taskctl = (struct TASKCTL *) memman_alloc_4k(memman, sizeof (struct TASKCTL));
  for (i = 0; i < MAX_TASKS; i++) {
    taskctl->tasks0[i].flags = 0;
		taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
		set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
  }
  for (i = 0; i < MAX_TASKLEVELS; i++) {
    taskctl->level[i].running = 0;
		taskctl->level[i].now = 0;
  }
  task = task_alloc();
  task->flags = 2; // Running task mark
  task->priority = 2; // 0.02s
  task->level = 0;
  task_add(task);
  task_switchsub();
	load_tr(task->sel);
	task_timer = timer_alloc();
	timer_settime(task_timer, task->priority);
	return task;
}

struct TASK *task_alloc(void) {
  int i;
	struct TASK *task;
  for (i = 0; i < MAX_TASKS; i++) {
    if (taskctl->tasks0[i].flags == 0) {
      task = &taskctl->tasks0[i];
      task->flags = 1; // Running mark
      task->tss.eflags = 0x00000202; // IF = 1
      task->tss.eax = 0; // Set 0 anyhow
      task->tss.ecx = 0;
			task->tss.edx = 0;
			task->tss.ebx = 0;
			task->tss.ebp = 0;
			task->tss.esi = 0;
			task->tss.edi = 0;
      task->tss.es = 0;
			task->tss.ds = 0;
			task->tss.fs = 0;
			task->tss.gs = 0;
			task->tss.ldtr = 0;
			task->tss.iomap = 0x40000000;
      return task;
    }
  }
  return 0; // Reaching max tasks
}

void task_run(struct TASK *task, int level, int priority) {
  if (level < 0) {
    level = task->level;
  }
  if (priority > 0) {
    task->priority = priority;
  }

  if (task->flags == 2 && task->level != level) { // Change level
    task_remove(task);
  }
  if (task->flags != 2) {
    task->level = level;
    task_add(task);
  }

  taskctl->lv_change = 1;
  return;
}

void task_sleep(struct TASK *task) {
  struct TASK *now_task;
  if (task->flags == 2) { // if the taks is awake
    now_task = task_now();
    task_remove(task);
    if (task == now_task) { // Sleeping itself
      task_switchsub();
      now_task = task_now();
      farjmp(0, now_task->sel);
    }
  }
  return;
}

void task_switch(void) {
  struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
	struct TASK *new_task, *now_task = tl->tasks[tl->now];
	tl->now++;
  if (tl->now == tl->running) {
    tl->now = 0;
  }
  if (taskctl->lv_change != 0) {
    task_switchsub();
    tl = &taskctl->level[taskctl->now_lv];
  }
  new_task = tl->tasks[tl->now];
	timer_settime(task_timer, new_task->priority);
	if (new_task != now_task) {
		farjmp(0, new_task->sel);
	}
  return;
}

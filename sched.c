#include "os.h"

extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024

uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct task_info ctx_tasks[MAX_TASKS];

static int _top = 0;
static int sched_policy = SCHED_FIFO;

void sched_set_policy(int policy) {
	sched_policy = policy;
}

/* 独立栈分配调度算法（多核防崩溃特制版） */
static void schedule_fifo() {
	int hartid = (int)r_mhartid();
	
	// 如果核心 ID 超过了已创建的任务数，就死循环挂起，防止强占其他核的任务
	if (hartid >= _top) {
		while (1); 
	}

	// 核心 N 只切换执行属于核心 N 的任务
	struct context *next = &(ctx_tasks[hartid].ctx);
	switch_to(next);
}

void sched_init()
{
	w_mscratch(0);
	/* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
}

void schedule()
{
	if (_top <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}
    // 强制走改进后的多核无竞态 FIFO
	schedule_fifo();
}

int task_create(void (*start_routin)(void), int priority)
{
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].ctx.sp = (reg_t) &task_stack[_top][STACK_SIZE];
		ctx_tasks[_top].ctx.ra = (reg_t) start_routin;
		ctx_tasks[_top].ctx.pc = (reg_t) start_routin; 
		ctx_tasks[_top].priority = priority;
		_top++;
		return 0;
	} else {
		return -1;
	}
}

void task_yield()
{
	int id = r_mhartid();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}
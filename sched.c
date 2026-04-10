#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024
/*
 * In the standard RISC-V calling convention, the stack pointer sp
 * is always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct task_info ctx_tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

/* 调度策略默认保持 FIFO */
static int sched_policy = SCHED_FIFO;

void sched_set_policy(int policy) {
	sched_policy = policy;
}

/* 简单的线性同余伪随机数生成器 */
static uint32_t rand_seed = 123456789;
static uint32_t os_rand(void) {
	rand_seed = rand_seed * 1664525 + 1013904223;
	return rand_seed;
}

/* FIFO 调度算法 */
static void schedule_fifo() {
	_current = (_current + 1) % _top;
	struct context *next = &(ctx_tasks[_current].ctx);
	switch_to(next);
}

/* 基于优先级权重的随机调度算法 */
static void schedule_weight() {
	int total_weight = 0;
	
	for (int i = 0; i < _top; i++) {
		total_weight += ctx_tasks[i].priority;
	}

	// 如果没有权重，退化为 FIFO
	if (total_weight <= 0) {
		schedule_fifo();
		return;
	}

	uint32_t r = os_rand() % total_weight;
	
	int accum = 0;
	int next_task = 0;
	for (int i = 0; i < _top; i++) {
		accum += ctx_tasks[i].priority;
		if (accum > r) {
			next_task = i;
			break;
		}
	}

	_current = next_task;
	struct context *next = &(ctx_tasks[_current].ctx);
	switch_to(next);
}

void sched_init()
{
	w_mscratch(0);
	/* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	if (_top <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}

	if (sched_policy == SCHED_PRIO) {
		schedule_weight();
	} else {
		schedule_fifo();
	}
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routine entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*start_routin)(void), int priority)
{
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].ctx.sp = (reg_t) &task_stack[_top][STACK_SIZE];
		// ctx_tasks[_top].ctx.ra = (reg_t) start_routin;
        // 👇 新增 pc 初始化，指向任务的起始地址
		ctx_tasks[_top].ctx.pc = (reg_t) start_routin; 
		ctx_tasks[_top].priority = priority;
		_top++;
		return 0;
	} else {
		return -1;
	}
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
	// 👇 不再直接调用 schedule()，而是通过写 CLINT MSIP 寄存器触发软件中断
	int id = r_mhartid();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}


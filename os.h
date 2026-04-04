#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "riscv.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);

/* 字节级内存管理 */
void *malloc(uint32_t size);
void free(void *p);
void malloc_test(void);

/* task management */
struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
};

/* 任务控制块 TCB */
struct task_info {
	struct context ctx;
	int priority; // 任务优先级
};

extern int  task_create(void (*task)(void), int priority);
extern void task_delay(volatile int count);
extern void task_yield();

// 添加策略宏定义 ...
#define SCHED_FIFO 0
#define SCHED_PRIO 1

extern void sched_set_policy(int policy);

/* plic */
extern int plic_claim(void);
extern void plic_complete(int irq);

#endif /* __OS_H__ */

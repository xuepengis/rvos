#include "os.h"

#define DELAY 5

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");
    // 演示协同式：刚创建完主动让出 CPU 给其他任务
	task_yield();
	
	uart_puts("Task 0: I'm back!\n");
	while (1) {
		uart_puts("Task 0: Running (Cooperative)...\n");
        // 执行一段代码后主动 yield
		task_delay(DELAY);
		task_yield(); 
	}
}

void user_task1(void)
{
	uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running (Preemptive)...\n");
		task_delay(DELAY);
        // 不主动 yield，必须等待 Timer Interrupt 来抢占它
	}
}

void user_task2(void)
{
	uart_puts("Task 2: Created!\n");
	while (1) {
		uart_puts("Task 2: Running (Preemptive)...\n");
		task_delay(DELAY);
        // 不主动 yield，必须等待 Timer Interrupt 来抢占它
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task0, 1);
	task_create(user_task1, 1);
	task_create(user_task2, 1);
}

// 下面对应实验四的压力测试：连续触发多种异常，验证 Trap 处理程序的健壮性
// extern void trap_test(void);

// void user_task0(void)
// {
// 	uart_puts("Task 0: Created!\n");
// 	while (1) {
// 		uart_puts("Task 0: Running...\n");
		
// 		trap_test();

// 		task_delay(DELAY);
// 		task_yield();
// 	}
// }

// void user_task1(void)
// {
// 	uart_puts("Task 1: Created!\n");
// 	while (1) {
// 		uart_puts("Task 1: Running...\n");
// 		task_delay(DELAY);
// 		task_yield();
// 	}
// }

// /* NOTICE: DON'T LOOP INFINITELY IN main() */
// void os_main(void)
// {
// 	task_create(user_task0, 1);
// 	task_create(user_task1, 1);
// }
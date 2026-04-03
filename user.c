#include "os.h"

#define DELAY 1000 

// 全局计数器
static int run_counts[4] = {0, 0, 0, 0};
static int total_ticks = 0;

// 统计并周期性打印
void record_and_print(int task_id) {
	run_counts[task_id]++;
	total_ticks++;
	
	// 每调度满 100 次，打印一次统计结果
	if (total_ticks % 100 == 0) {
		printf("\n--- 100 Schedules Reached ---\n");
		printf("Task 0 (Weight 1) runs: %d times\n", run_counts[0]);
		printf("Task 1 (Weight 2) runs: %d times\n", run_counts[1]);
		printf("Task 2 (Weight 3) runs: %d times\n", run_counts[2]);
		printf("Task 3 (Weight 4) runs: %d times\n", run_counts[3]);
		printf("-----------------------------\n");
		
		// 重置计数器进入下一个周期
		run_counts[0] = 0;
		run_counts[1] = 0;
		run_counts[2] = 0;
		run_counts[3] = 0;
	}
}

void user_task0(void)
{
	uart_puts("Task 0 (Weight 1): Created!\n");
	while (1) {
		record_and_print(0);
		task_delay(DELAY);
		task_yield();
	}
}

void user_task1(void)
{
	uart_puts("Task 1 (Weight 2): Created!\n");
	while (1) {
		record_and_print(1);
		task_delay(DELAY);
		task_yield();
	}
}

void user_task2(void)
{
	uart_puts("Task 2 (Weight 3): Created!\n");
	while (1) {
		record_and_print(2);
		task_delay(DELAY);
		task_yield();
	}
}

void user_task3(void)
{
	uart_puts("Task 3 (Weight 4): Created!\n");
	while (1) {
		record_and_print(3);
		task_delay(DELAY);
		task_yield();
	}
}

void os_main(void)
{
	uart_puts("Setting Schedule Policy to Priority Weight...\n");
	
	sched_set_policy(SCHED_PRIO);

	// 创建4个任务，权重分别是 1、2、3、4（总权重 10）
	task_create(user_task0, 1);
	task_create(user_task1, 2);
	task_create(user_task2, 3);
	task_create(user_task3, 4);
}
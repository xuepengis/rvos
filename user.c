#include "os.h"

#define DELAY 4000

struct userdata {
	char *name;
};

struct userdata timer3 = {"software timer 3"};
struct userdata timer5 = {"software timer 5"};
struct userdata timer7 = {"software timer 7"};

void timer_func(void *arg)
{
	if (NULL == arg) return;
	struct userdata *param = (struct userdata *)arg;
	
	// 在打印时加��当前的核心 ID，证明多核定时器并发正常且没有混淆
	printf("Hart %d: ~~~~~~~~~~ %s ~~~~~~~~~~\n", (int)r_mhartid(), param->name);
}

void user_task0(void)
{
	printf("Task 0: Created on Hart %d!\n", (int)r_mhartid());
	// Hart 0 创建 3 5 7 时钟
	timer_create(timer_func, &timer3, 3);
	timer_create(timer_func, &timer5, 5);
	timer_create(timer_func, &timer7, 7);

	while (1) { task_delay(DELAY); }
}

void user_task1(void)
{
	printf("Task 1: Created on Hart %d!\n", (int)r_mhartid());
	// Hart 1 创建 3 5 7 时钟
	timer_create(timer_func, &timer3, 3);
	timer_create(timer_func, &timer5, 5);
	timer_create(timer_func, &timer7, 7);
	
	while (1) { task_delay(DELAY); }
}

void user_task2(void)
{
	printf("Task 2: Created on Hart %d!\n", (int)r_mhartid());
	// Hart 2 创建 3 5 7 时钟
	timer_create(timer_func, &timer3, 3);
	timer_create(timer_func, &timer5, 5);
	timer_create(timer_func, &timer7, 7);
	
	while (1) { task_delay(DELAY); }
}

void os_main(void)
{
	task_create(user_task0, 1);
	task_create(user_task1, 1);
	task_create(user_task2, 1);
	
	printf("Task cmd: Created!\n");
}
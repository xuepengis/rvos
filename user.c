#include "os.h"

#define DELAY 4000

struct userdata {
	char *name;
};

/* Jack must be global */
struct userdata timer3 = {"software timer 3"};
struct userdata timer5 = {"software timer 5"};
struct userdata timer7 = {"software timer 7"};

void timer_func(void *arg)
{
	if (NULL == arg)
		return;

	struct userdata *param = (struct userdata *)arg;
	printf("~~~~~~~~~~ %s ~~~~~~~~~~\n", param->name);
}

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");

	struct timer *t1 = timer_create(timer_func, &timer3, 3);
	if (NULL == t1) {
		printf("timer_create() failed!\n");
	}
	struct timer *t2 = timer_create(timer_func, &timer5, 5);
	if (NULL == t2) {
		printf("timer_create() failed!\n");
	}
	struct timer *t3 = timer_create(timer_func, &timer7, 7);
	if (NULL == t3) {
		printf("timer_create() failed!\n");
	}
	while (1) {
		uart_puts("Task 0: Running... \n");
		task_delay(DELAY);
	}
}

void user_task1(void)
{
	uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running... \n");
		task_delay(DELAY);
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task0, 1);
	task_create(user_task1, 1);
}


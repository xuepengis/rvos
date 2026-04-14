#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");
	uart_puts("My name is Xue Peng!\n");

	page_init();

	malloc_test();

	sched_init();
	uart_puts("sched_init finish\n");

	trap_init();
	uart_puts("trap_init finish\n");

	plic_init();
	uart_puts("plic_init finish\n");

	timer_init();
	uart_puts("timer_init finish\n");

	os_main();
	uart_puts("os_main finish\n");

	schedule();

	uart_puts("Would not go here!\n");
	while (1) {}; // stop here!
}


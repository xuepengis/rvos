extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");
	uart_puts("my name is xue peng!");

	while (1) {}; // stop here!
}


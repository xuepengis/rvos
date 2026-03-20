extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, Kernel!\n");
	uart_puts("My name is 薛鹏\n");

	while (1) {}; // stop here!
}


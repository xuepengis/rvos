#include "os.h"

#define DELAY 1000

extern int uart_getc(void);

/* === 命令行读取辅助函数 === */
void gets(char *buf, int max_len) 
{
	int i = 0;
	while (i < max_len - 1) {
		char c = uart_getc();
		if (c == '\r' || c == '\n') {
			uart_puts("\n");
			break;
		} else if (c == 8 || c == 127) { // 退格键处理 (Backspace / Delete)
			if (i > 0) {
				i--;
				uart_puts("\b \b");
			}
		} else {
			buf[i++] = c;
			uart_putc(c); // 回显当前输入字符
		}
	}
	buf[i] = '\0';
}

#define CMD_MAX_LEN 128

/* === 交互命令任务 === */
void user_task_cmd(void)
{
	uart_puts("Task cmd: Created!\n");
	char cmd_buf[CMD_MAX_LEN];
	
	while (1) {
		// 阻塞读取整行
		gets(cmd_buf, CMD_MAX_LEN);
		
		// 如果只是按了回车（空串），则仅打印提示符
		if (cmd_buf[0] == '\0') {
			uart_puts("What should I do?\n");
			continue;
		}

		// 回显命令和提示信息
		uart_puts("The command is: ");
		uart_puts(cmd_buf);
		uart_puts("\nWhat should I do?\n");
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task_cmd, 1);
}
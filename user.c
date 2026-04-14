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













// TODO: 实验五，暂时兼容（实验6 和 7）未通过，硬件中断设置有问题
// extern int uart_getc(void);

// /* === 命令行读取辅助函数 === */
// void gets(char *buf, int max_len) 
// {
// 	int i = 0;
// 	while (i < max_len - 1) {
// 		char c = uart_getc();
// 		if (c == '\r' || c == '\n') {
// 			uart_puts("\n");
// 			break;
// 		} else if (c == 8 || c == 127) { // 退格键处理 (Backspace / Delete)
// 			if (i > 0) {
// 				i--;
// 				uart_puts("\b \b");
// 			}
// 		} else {
// 			buf[i++] = c;
// 			uart_putc(c); // 回显当前输入字符
// 		}
// 	}
// 	buf[i] = '\0';
// }

// #define CMD_MAX_LEN 128

// /* === 交互命令任务 === */
// void user_task_cmd(void)
// {
// 	uart_puts("Task cmd: Created!\n");
// 	char cmd_buf[CMD_MAX_LEN];
	
// 	while (1) {
// 		// 阻塞读取整行
// 		gets(cmd_buf, CMD_MAX_LEN);
		
// 		// 如果只是按了回车（空串），则仅打印提示符
// 		if (cmd_buf[0] == '\0') {
// 			uart_puts("What should I do?\n");
// 			continue;
// 		}

// 		// 回显命令和提示信息
// 		uart_puts("The command is: ");
// 		uart_puts(cmd_buf);
// 		uart_puts("\nWhat should I do?\n");
// 	}
// }

// /* NOTICE: DON'T LOOP INFINITELY IN main() */
// void os_main(void)
// {
// 	task_create(user_task_cmd, 1);
// }
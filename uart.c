#include "platform.h"
#include "types.h"

/* 
 * 串口初始化
 * 在 LiteX 中，BIOS 已经在硬件底层配置好了波特率等参数。
 * 我们只需要直接使用即可，这里留空。
 */
void uart_init()
{
}

/* 
 * 发送单个字符
 */
void uart_putc(char c)
{
    /* 1. 读取 TXFULL 寄存器，如果为 1，说明发送队列满了，就在这里死循环等待 */
    /* 注意：LiteX 的 CSR 寄存器读写通常需要强转为 32位 指针 (uint32_t *) */
    while (*(volatile uint32_t *)CSR_UART_TXFULL_ADDR != 0) {
        // 等待
    }
    
    /* 2. 将字符写入 RXTX 寄存器，触发发送 */
    *(volatile uint32_t *)CSR_UART_RXTX_ADDR = c;
}

/* 
 * 发送字符串 
 */
void uart_puts(char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}
#include "types.h"
#include "platform.h"

// 宏定义：在 LiteX UART 中，bit 0 是 TX，bit 1 是 RX
#define UART_EV_TX (1 << 0)
#define UART_EV_RX (1 << 1)

void uart_init()
{
    // 1. 清除一切残留的挂起事件 (写 1 清零)
    *(volatile uint32_t *)CSR_UART_EV_PENDING_ADDR = UART_EV_TX | UART_EV_RX;
    
    // 2. 【核心修复】：只写入 2 (UART_EV_RX)，坚决不要写 1！
    *(volatile uint32_t *)CSR_UART_EV_ENABLE_ADDR = UART_EV_RX;
}

void uart_putc(char c)
{
    while (*(volatile uint32_t *)CSR_UART_TXFULL_ADDR != 0);
    *(volatile uint32_t *)CSR_UART_RXTX_ADDR = c;
}

void uart_puts(char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}

int uart_getc(void)
{
    if (*(volatile uint32_t *)CSR_UART_RXEMPTY_ADDR != 0) {
        return -1;
    }
    return *(volatile uint32_t *)CSR_UART_RXTX_ADDR;
}

void uart_isr(void)
{
    uint32_t pending = *(volatile uint32_t *)CSR_UART_EV_PENDING_ADDR;
    
    // 如果是 RX 接收中断，我们就读取字符并回显
    if (pending & UART_EV_RX) {
        while (1) {
            int c = uart_getc();
            if (c == -1) break;
            uart_putc((char)c);
            uart_putc('\n');
        }
        // 清除 RX 中断标志
        *(volatile uint32_t *)CSR_UART_EV_PENDING_ADDR = UART_EV_RX;
    }
}
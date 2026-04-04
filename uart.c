#include "os.h"

/*
 * The UART control registers are memory-mapped at address UART0. 
 * This macro returns the address of one of the registers.
 */
#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg))

/*
 * Reference
 * [1]: TECHNICAL DATA ON 16550, http://byterunner.com/16550.html
 */

/*
 * UART control registers map. see [1] "PROGRAMMING TABLE"
 * note some are reused by multiple functions
 * 0 (write mode): THR/DLL
 * 1 (write mode): IER/DLM
 */
#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v))

/* === 环形队列实现 (Ring Buffer) === */
#define UART_BUF_SIZE 128
static char uart_buf[UART_BUF_SIZE];
static volatile int uart_buf_r = 0; // 读指针
static volatile int uart_buf_w = 0; // 写指针

void uart_init()
{
	/* disable interrupts. */
	uart_write_reg(IER, 0x00);

	/* Setting baud rate. */
	uint8_t lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7));
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);

	/* Continue setting the asynchronous data communication format. */
	lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0));

	/* enable receive interrupts. */
	uint8_t ier = uart_read_reg(IER);
	uart_write_reg(IER, ier | (1 << 0));
}

int uart_putc(char ch)
{
	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
	return uart_write_reg(THR, ch);
}

void uart_puts(char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}

int uart_getc(void)
{
	while (0 == (uart_read_reg(LSR) & LSR_RX_READY))
		;
	return uart_read_reg(RHR);
}

/*
 * 每次中断发生时，将接收到的字符放入环形队列
 */
void uart_isr(void)
{
	while (uart_read_reg(LSR) & LSR_RX_READY) {
		char c = uart_read_reg(RHR);
		int next_w = (uart_buf_w + 1) % UART_BUF_SIZE;
		// 如果队列未满，就存入；满了就直接丢弃
		if (next_w != uart_buf_r) {
			uart_buf[uart_buf_w] = c;
			uart_buf_w = next_w;
		}
	}
}
#include "os.h"

extern void trap_vector(void);
extern void uart_isr(void);

/**
 * 初始化 Trap 向量表
 */
void trap_init()
{
    /* 设置机器模式（Machine Mode）的陷阱向量基地址 */
    w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ){
      		uart_isr();
	} else if (irq) {
		printf("unexpected interrupt irq = %d\n", irq);
	}
	
	if (irq) {
		plic_complete(irq);
	}
}


/**
 * Trap 处理程序
 * epc: 异常发生时的程序计数器 (mepc)
 * cause: 异常原因 (mcause)
 */
reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & MCAUSE_MASK_ECODE;
    
    if (cause & MCAUSE_MASK_INTERRUPT) {
        /* 异步中断（Asynchronous trap - interrupt） */
        switch (cause_code) {
        case 3:
            uart_puts("software interruption!\n");
            break;
        case 7:
            uart_puts("timer interruption!\n");
            break;
        case 11:
            // uart_puts("external interruption!\n");
			external_interrupt_handler();
            break;
        default:
            printf("Unknown async exception! Code = %ld\n", cause_code);
            break;
        }
    } else {
        /* 同步异常（Synchronous trap - exception） */
        printf("Sync exceptions! Code = %ld\n", cause_code);
        switch (cause_code) {
        case 2:
            uart_puts("Exception: Illegal instruction!\n");
            break;
        case 3:
            uart_puts("Exception: Breakpoint!\n");
            break;
        case 5:
            uart_puts("Exception: Load access fault!\n");
            break;
        case 7:
            uart_puts("Exception: Store/AMO access fault!\n");
            break;
        default:
            // 使用 printf 打印更多信息方便调试
            printf("OOPS! Unknown synchronous exception! Code = %ld, EPC = 0x%lx\n", cause_code, epc);
            panic("Check your trap logic!");
            break;
        }

        /* * 动态计算返回地址
         * RISC-V 带有 C 扩展（压缩指令）时，指令可能是 16 位或 32 位。
         * 如果指令的低两位 [1:0] 为 11，则为 32 位指令；否则为 16 位指令。
         */
        unsigned short insn = *(unsigned short *)epc;
        if ((insn & 0x3) == 0x3) {
            return_pc += 4; // 32-bit instruction
        } else {
            return_pc += 2; // 16-bit instruction (e.g., c.ebreak)
        }
    }

    return return_pc;
}

/**
 * 压力测试：连续触发多种异常
 */
void trap_test()
{
    uart_puts("\n--- Trap Test Start ---\n");

    /*
     * 1. 非法指令 (Code 2)
     * 使用 .word 强制插入一个无效的操作码
     */
    uart_puts("Testing Illegal instruction (Code 2)...\n");
    __asm__ __volatile__(".word 0xffffffff");

    /*
     * 2. 断点 (Code 3)
     * 在开启 RVC 的情况下，ebreak 通常会被编码为 c.ebreak (2字节)
     */
    uart_puts("Testing Breakpoint (Code 3)...\n");
    __asm__ __volatile__("ebreak");

    /*
     * 3. 加载访问错误 (Code 5)
     * 显式使用汇编触发，防止编译器生成多余的计算指令
     */
    uart_puts("Testing Load access fault (Code 5)...\n");
    __asm__ __volatile__(
        "li t0, 0x0\n"
        "lw t1, 0(t0)\n"
        : : : "t0", "t1"
    );

    /*
     * 4. 存储访问错误 (Code 7)
     */
    uart_puts("Testing Store/AMO access fault (Code 7)...\n");
    __asm__ __volatile__(
        "li t0, 0x0\n"
        "sw zero, 0(t0)\n"
        : : : "t0"
    );

    uart_puts("Yeah! I'm return back from trap safely!\n");
    uart_puts("--- Trap Test End ---\n\n");
}
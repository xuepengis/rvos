#include "os.h"

extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_handler(void);
extern void schedule(void);

void trap_init()
{
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

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & MCAUSE_MASK_ECODE;
    
    if (cause & MCAUSE_MASK_INTERRUPT) {
        switch (cause_code) {
		case 3:
			printf("software interruption!\n");
			int id = r_mhartid();
    			*(uint32_t*)CLINT_MSIP(id) = 0;
			schedule();
			break;
        case 7:
            // 删除无锁的 uart_puts，防止多核乱码和干扰软件时钟打印输出
            timer_handler();
            break;
        case 11:
			external_interrupt_handler();
            break;
        default:
            printf("Unknown async exception! Code = %ld\n", cause_code);
            break;
        }
    } else {
        printf("Sync exceptions! Code = %ld\n", cause_code);
        switch (cause_code) {
        case 2:
            printf("Exception: Illegal instruction!\n");
            break;
        case 3:
            printf("Exception: Breakpoint!\n");
            break;
        case 5:
            printf("Exception: Load access fault!\n");
            break;
        case 7:
            printf("Exception: Store/AMO access fault!\n");
            break;
        default:
            printf("OOPS! Unknown synchronous exception! Code = %ld, EPC = 0x%lx\n", cause_code, epc);
            panic("Check your trap logic!");
            break;
        }

        unsigned short insn = *(unsigned short *)epc;
        if ((insn & 0x3) == 0x3) {
            return_pc += 4; 
        } else {
            return_pc += 2; 
        }
    }

    return return_pc;
}
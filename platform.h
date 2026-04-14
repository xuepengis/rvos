#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// 1. 修改最大 CPU 数量（根据 CONFIG_CPU_COUNT）
#define MAXNUM_CPU 1

// 2. 修改内存大小，配合你在 LiteX 里分配的 16MB
#define LENGTH_RAM (16 * 1024 * 1024)

// 3. 定义 LiteX 的 CSR 基地址
#define CSR_BASE 0xf0000000L

// 4. 重写 UART 相关的寄存器地址 (LiteX LiteUART)
#define CSR_UART_RXTX_ADDR       (CSR_BASE + 0x1800L)
#define CSR_UART_TXFULL_ADDR     (CSR_BASE + 0x1804L)
#define CSR_UART_RXEMPTY_ADDR    (CSR_BASE + 0x1808L)
#define CSR_UART_EV_STATUS_ADDR  (CSR_BASE + 0x180cL)
#define CSR_UART_EV_PENDING_ADDR (CSR_BASE + 0x1810L)
#define CSR_UART_EV_ENABLE_ADDR  (CSR_BASE + 0x1814L)

// 5. UART 中断号 (来自 soc.h 的 UART_INTERRUPT)
#define UART0_IRQ 1

// 6. PLIC 基地址 (根据仿真日志得知)
#define PLIC_BASE 0xf0c00000L

// PLIC 寄存器偏移量（VexRiscv-SMP 的 PLIC 符合 RISC-V 标准，
// 但上下文偏移步长与 QEMU 略有不同）
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4)
#define PLIC_MENABLE(hart, id) (PLIC_BASE + 0x2000 + (hart) * 0x80 + ((id) / 32) * 4)
// 注意：在 VexRiscv-SMP PLIC 中，M 模式的 threshold/claim 寄存器地址偏移可能需要调整。
// 我们可以先使用标准的 0x200000 作为偏移。如果后续中断捕获有问题，再调整这里。
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)

// 7. CLINT 基地址 (根据仿真日志得知)
#define CLINT_BASE 0xf0010000L

// CLINT 时钟频率 (来自 soc.h 的 CONFIG_CLOCK_FREQUENCY)
#define CLINT_TIMEBASE_FREQ 1000000

// 8. CLINT 寄存器偏移 (符合 RISC-V 标准)
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid))
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME (CLINT_BASE + 0xbff8) // machine time register

#endif /* __PLATFORM_H__ */
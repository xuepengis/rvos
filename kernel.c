#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

// 定义一个同步标记，供其它核心等待主核初始化完成
volatile int init_done = 0;

void start_kernel(void)
{
	reg_t hartid = r_mhartid(); // 直接使用 reg_t
	
	if (hartid == 0) {
		uart_init();
		printf("Hart %d: Hello, World!\n", (int)hartid);

		page_init();
		trap_init();
		plic_init();
		timer_init();
		sched_init();
		os_main(); 
		
		init_done = 1; // 唤醒其他核心
	} else {
		// 从核等待主核完成全局初始化
		while (init_done == 0) { }
		
		printf("Hart %d: Hello, World!\n", (int)hartid);
		trap_init();   // 本地 CSR 和 中断向量设置
		plic_init();   // PLIC 使能本核的阈值
		timer_init();  // 配置本核的时钟中断
		sched_init();  // 【重要修复】配置本核的软件中断并初始化 mscratch
	}

	schedule(); // 所有核最终都进入调度器

	while (1) {}; // stop here!
}
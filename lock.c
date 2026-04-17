#include "os.h"

void spin_lock(spinlock_t *lk)
{
	// 1. 读取当前 MIE 中断状态
	reg_t m = r_mstatus();
	
	// 2. 关闭本地中断
	w_mstatus(m & ~MSTATUS_MIE);
	
	// 3. 跨核自旋等待锁
	while (__sync_lock_test_and_set(&lk->locked, 1) != 0) {
		/* do nothing */
	}
	
	// 4. 将加锁前的中断状态存入锁内
	lk->saved_mstatus = m & MSTATUS_MIE;
}

void spin_unlock(spinlock_t *lk)
{
	// 1. 取出这把锁加锁前的中断状态
	reg_t restore_mie = lk->saved_mstatus;
	
	// 2. 释放跨核互斥锁
	__sync_lock_release(&lk->locked);
	
	// 3. 精准恢复本地中断状态（之前是开的就开，之前是关的就关）
	if (restore_mie) {
		w_mstatus(r_mstatus() | MSTATUS_MIE);
	} else {
		w_mstatus(r_mstatus() & ~MSTATUS_MIE);
	}
}
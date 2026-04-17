#include "os.h"

extern void schedule(void);

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

/* 每个核心维护自己的 tick */
static uint32_t _tick[MAXNUM_CPU] = {0};

/* 每个核心维护自己的 timer 列表 */
#define MAX_TIMER 10
static struct timer timer_list[MAXNUM_CPU][MAX_TIMER];
static spinlock_t timer_lock = {0};

void timer_load(int interval)
{
	int id = r_mhartid();
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{
	int id = r_mhartid();
	
	/* 只清空当前核心的定时器列表 */
	struct timer *t = &(timer_list[id][0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		t->func = NULL;
		t->arg = NULL;
		t++;
	}
	_tick[id] = 0;

	timer_load(TIMER_INTERVAL);
	w_mie(r_mie() | MIE_MTIE);
}

struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
	if (NULL == handler || 0 == timeout) return NULL;

	int id = r_mhartid();
	spin_lock(&timer_lock);

	struct timer *t = &(timer_list[id][0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (NULL == t->func) break;
		t++;
	}
	if (NULL != t->func) {
		spin_unlock(&timer_lock);
		return NULL;
	}

	t->func = handler;
	t->arg = arg;
	t->timeout_tick = _tick[id] + timeout;
	t->period_tick = timeout;

	spin_unlock(&timer_lock);
	return t;
}

void timer_delete(struct timer *timer)
{
	int id = r_mhartid();
	spin_lock(&timer_lock);
	
	struct timer *t = &(timer_list[id][0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (t == timer) {
			t->func = NULL;
			t->arg = NULL;
			t->timeout_tick = 0;
			t->period_tick = 0;
			break;
		}
		t++;
	}
	spin_unlock(&timer_lock);
}

static inline void timer_check()
{
	int id = r_mhartid();
	struct timer *t = &(timer_list[id][0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (NULL != t->func) {
			if (_tick[id] >= t->timeout_tick) {
				t->func(t->arg);
				t->timeout_tick += t->period_tick;
			}
		}
		t++;
	}
}

void timer_handler() 
{
	int id = r_mhartid();
	
	/* 每个核心只增加自己的 tick */
	_tick[id]++;

	/* 检查属于当前核心的定时器 */
	timer_check(); 

	timer_load(TIMER_INTERVAL);
	schedule();
}
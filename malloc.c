#include "os.h"

#define PAGE_SIZE 4096
#define ALIGNMENT 4
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

struct block_meta {
	uint32_t size;
	uint32_t is_free;
	struct block_meta *next;
};

#define META_SIZE ALIGN(sizeof(struct block_meta))
static struct block_meta *global_base = NULL;

// 仅向页分配器申请新空间
static struct block_meta *request_space(struct block_meta *last, uint32_t size) {
	uint32_t total_needed = size + META_SIZE;
	int npages = (total_needed + PAGE_SIZE - 1) / PAGE_SIZE;

	struct block_meta *block = (struct block_meta *)page_alloc(npages);
	if (!block) return NULL;

	block->size = (npages * PAGE_SIZE) - META_SIZE;
	block->is_free = 0;
	block->next = NULL;

	if (last) { last->next = block; }
	return block;
}

void *malloc(uint32_t size) {
	if (size <= 0) return NULL;
	uint32_t aligned_size = ALIGN(size);

	if (!global_base) {
		global_base = request_space(NULL, aligned_size);
		return (global_base + 1);
	}
	
	struct block_meta *last = global_base;
	while (last->next) { last = last->next; } // 找到链表末尾
	struct block_meta *block = request_space(last, aligned_size);
	return (block + 1);
}

void malloc_test() {
	printf("Test: Basic Allocation\n");
	void *p1 = malloc(10);
	printf("p1 (10 bytes) at: %p\n", p1);
}
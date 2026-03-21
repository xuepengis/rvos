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

// 辅助函数：寻找空闲块
static struct block_meta *find_free_block(struct block_meta **last, uint32_t size) {
	struct block_meta *current = global_base;
	while (current && !(current->is_free && current->size >= size)) {
		*last = current;
		current = current->next;
	}
	return current;
}

// 辅助函数：拆分得到的内存块
static void split_block(struct block_meta *block, uint32_t size) {
	if (block->size >= size + META_SIZE + ALIGNMENT) {
		struct block_meta *new_block = (struct block_meta *)((uint8_t *)block + META_SIZE + size);
		new_block->size = block->size - size - META_SIZE;
		new_block->is_free = 1;
		new_block->next = block->next;

		block->size = size;
		block->next = new_block;
	}
}

// 辅助函数：合并释放之后的碎片
static void coalesce() {
	struct block_meta *curr = global_base;
	while (curr && curr->next) {
		if (curr->is_free && curr->next->is_free) {
			curr->size += META_SIZE + curr->next->size;
			curr->next = curr->next->next;
		} else {
			curr = curr->next;
		}
	}
}

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

void free(void *ptr) {
	if (!ptr) return;
	// 通过指针偏移找到 Header
	struct block_meta *block = (struct block_meta *)ptr - 1;
	block->is_free = 1; 
	printf("Block at %p marked as free\n", ptr);
	coalesce();
}

void *malloc(uint32_t size) {
	if (size <= 0) return NULL;
	uint32_t aligned_size = ALIGN(size);

	if (!global_base) {
		global_base = request_space(NULL, aligned_size);
		return (global_base + 1);
	}

	struct block_meta *last = global_base;

	// 逻辑修改：先找，找不到再申请
	struct block_meta *block = find_free_block(&last, aligned_size);
	if (!block) {
		block = request_space(last, aligned_size);
	} else {
		block->is_free = 0;
	}
    split_block(block, aligned_size);
	return (block + 1);
}

void malloc_test() {
	printf("Test: Coalescing\n");
	void *p1 = malloc(10);
	void *p2 = malloc(10);
	free(p1);
	free(p2);
	void *p3 = malloc(30); // 如果合并成功，p3 能拿到 p1 的起始地址
	if (p3 == p1) printf("SUCCESS: Coalesce working!\n");
}
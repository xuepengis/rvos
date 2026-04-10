# 系统软件综合实验 —— 课程实验报告

## 📝 第 1 次实验

本次实验目标是在 QEMU `virt` 平台上完成一个最小可运行的 RISC-V 内核：搭建基础构建环境，编写汇编启动入口，初始化 UART 串口，并在内核启动后输出 `Hello, RVOS!` 及小组成员信息。小组采用“组长搭建主干 + 成员通过分支补充功能 + 最终合并到 `master`”的协作方式完成开发。

**题目要求**

以教师提供的代码为基础，完成最小内核的构建与启动流程，实现以下内容：

- 搭建 `Makefile`、平台常量和基础类型等编译脚手架。
- 编写汇编启动入口 `_start`，完成 hart 判断、栈初始化和跳转到 C 内核入口。
- 实现基于 UART 的最小串口驱动，支持轮询输出。
- 编写 `start_kernel()`，在控制台输出 `Hello, RVOS!`，并扩展输出小组成员姓名。

**完成情况**

已完成内核启动和小组成员信息的输出，并成功运行了整个的小组协作流程。组长负责整体框架、启动流程、串口驱动、内核入口和分支整合，其他成员分别通过独立分支补充个人姓名输出，最终经多次合并后在 `master` 上形成完整结果。当前根目录下的 `kernel.c` 已能够依次输出 `Hello, RVOS!` 以及四位成员姓名，说明最小启动链路和串口输出流程已经打通。

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 项目统筹 | 负责整体任务拆分、主干开发和最终集成验收 |
| 协作整合 | 负责审核并合并各成员分支，维护 `master` 主线完整性 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 启动与内核主流程 | a949d2e | 完成 `kernel.c` 内核入口，输出 `Hello, RVOS!` 与个人信息，打通最小启动链路 |
| 协作整合 | 9a9b059 | 审查后合并组员杨怡萱的PR|
| 协作整合 | f230f28 | 审查后解决冲突，合并组员徐蜚遥的PR |
| 协作整合 | 0869287 | 审查后合并组员姚翎的PR，主线已包含全部组员输出 |

### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 串口输出扩展 | 负责在启动输出中加入自己的姓名输出，验证多行串口打印逻辑 |
| 分支协作 | 通过个人分支提交改动，并配合组长完成合并 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 串口输出扩展 | 02f6af2 | 在 `kernel.c` 中新增 `My name is 杨怡萱!` 输出，完成个人标识接入 |

### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 串口输出扩展 | 负责在启动输出中加入自己的姓名输出，验证多行串口打印逻辑 |
| 分支协作 | 通过个人分支提交改动，并配合组长完成合并 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 串口输出扩展 | d3eb587 | 在 `kernel.c` 中新增 `My name is 徐蜚遥` 输出，完善串口横幅内容 |

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 串口输出扩展 | 负责在启动输出中加入自己的姓名输出，验证多行串口打印逻辑 |
| 分支协作 | 通过个人分支提交改动，并配合组长完成合并 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 串口输出扩展 | a9b3141 | 在 `kernel.c` 中新增 `My name is 姚翎` 输出，完成个人内容集成 |

**提交编号**  
本次实验的最终提交编号为：8317c8b

## 📝 第 2 次实验

本次实验的目标是在 RISC-V `virt` 平台上，基于现有的页分配器实现一套高效的字节级内存管理系统。实验核心在于实现 `malloc` 和 `free` 函数，并通过 **4 字节对齐**、**空闲块拆分与合并** 等技术优化内存利用率。

**题目要求**

- **对齐**：针对 RV32I 架构，采用 4 字节对齐方式，减少 Padding 浪费。
- **结构**：设计紧凑的 `block_meta` 结构体，确保其在 4 字节对齐下不产生额外开销。
- **功能实现**：
  - 实现基于隐式链表的 First-fit 查找算法。
  - 实现 `split_block` 逻辑，减少内部碎片。
  - 实现 `coalesce` 逻辑，解决外部碎片问题。

**完成情况**

成功实现字节级别的动态内存申请和释放，并进行完整测试。

---

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 架构设计 | 设计 12 字节紧凑型 `block_meta` 结构及内存布局方案 |
| 全栈开发 | 迭代完成全部功能开发与代码重构 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 基础环境搭建 | `1de1fea` | 根据现有代码，完成基于块的内存申请并进行测试 |
| 在块的基础上申请内存（字节级） | `77f98f7` | 建立 `block_meta` 链表，支持向页分配器申请空间 |
| 申请内存的释放（字节级） | `bb970ef` | 实现 `free` 接口，引入 `is_free` 状态标记 |
| 内存分配策略优化 | `d8a33d4` | 引入 First-fit 搜索，实现空闲块的循环利用 |
| 块的空余切割 | `5b90ef8` | 实现 `split_block`，将大空闲块拆分为“占用块 + 新空闲块” |
| 释放内存碎片的合并 | `502328a` | 实现 `coalesce`，在释放时物理合并相邻空闲块 |

#### ✅ 测试报告

**测试用例 1（基于 `77f98f7`）**

```c
void malloc_test() {
    printf("Test: Basic Allocation\n");
    void *p1 = malloc(10);
    printf("p1 (10 bytes) at: %p\n", p1);
    void *p2 = malloc(20);
    printf("p2 (20 bytes) at: %p\n", p2);
    void *p3 = malloc(5000);
    printf("p3 (5000 bytes) at: %p\n", p3);
    void *p4 = malloc(10);
    printf("p4 (10 bytes) at: %p\n", p4);
    void *p5 = malloc(0);
    printf("p5 (0 bytes) at: %p\n", p5);
}
```

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800043f8(aligned to 0x80005000), HEAP_SIZE = 0xffffbc07,
num of reserved pages = 8, num of pages to be allocated for heap = 1048562
TEXT:   0x80000000 -> 0x80002f58
RODATA: 0x80002f58 -> 0x800031a5
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f8
HEAP:   0x8000d000 -> 0x7ffff000
Test: Basic Allocation
p1 (10 bytes) at: 0x8000d00c
p2 (20 bytes) at: 0x8000e00c
p3 (5000 bytes) at: 0x8000f00c
p4 (10 bytes) at: 0x8001100c
p5 (0 bytes) at: 0x00000000
```

**结论**：当前 `malloc` 通过 `request_space` 向页分配器申请新内存页，且每次分配都从链表末尾追加新块，尚未复用已释放内存。但字节级内存申请能正确调用底层页分配，返回正确地址。

---

**测试用例 2（基于 `bb970ef`）**

```c
void malloc_test() {
    printf("Test: Mark as Free\n");
    void *p1 = malloc(10);
    printf("p1 (10 bytes) at: %p\n", p1);
    free(p1);
    void *p2 = malloc(20);
    printf("p2 (20 bytes) at: %p\n", p2);
    free(p2);
}
```

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800043f8(aligned to 0x80005000), HEAP_SIZE = 0xffffbc07,
num of reserved pages = 8, num of pages to be allocated for heap = 1048562
TEXT:   0x80000000 -> 0x80002f64
RODATA: 0x80002f64 -> 0x80003182
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f8
HEAP:   0x8000d000 -> 0x7ffff000
Test: Mark as Free
p1 (10 bytes) at: 0x8000d00c
Block at 0x8000d00c marked as free
p2 (20 bytes) at: 0x8000e00c
Block at 0x8000e00c marked as free
```

**结论**：`free` 能正确找到对应指针位置，并将元数据中的 `is_free` 位置 1，表示释放成功。

---

### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 功能测试 | 专项验证 `d8a33d4` 版本的“块复用（Reuse）”逻辑 |

#### ✅ 测试报告（基于 `d8a33d4`）

**测试用例**

```c
void malloc_test() {
    printf("Test: Reuse\n");
    void *p1 = malloc(10);
    printf("p1: %p\n", p1);
    free(p1);
    void *p2 = malloc(10);
    printf("p2: %p\n", p2);
    if (p1 == p2) printf("SUCCESS: Block reused!\n");
}
```

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800043f8(aligned to 0x80005000), HEAP_SIZE = 0xffffbc07,
num of reserved pages = 8, num of pages to be allocated for heap = 1048562
TEXT:   0x80000000 -> 0x80003000
RODATA: 0x80003000 -> 0x80003214
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f8
HEAP:   0x8000d000 -> 0x7ffff000
Test: Reuse
p1: 0x8000d00c
Block at 0x8000d00c marked as free
p2: 0x8000d00c
SUCCESS: Block reused!
```

**结论**：验证了 First-fit 搜索逻辑正确，已能成功复用标记为 `is_free` 的内存块，避免盲目申请新页。

---

### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 功能测试 | 验证 `5b90ef8` 版本的“内存拆分（Splitting）”功能 |

#### ✅ 测试报告（基于 `5b90ef8`）

**测试用例**

```c
void malloc_test() {
    printf("Test: Splitting\n");
    void *p1 = malloc(100);
    printf("p1: %p\n", p1);
    free(p1);
    void *p2 = malloc(20); // 会拆分 p1 之前的空间
    void *p3 = malloc(20); // 会占用 p1 拆分出来的下一段空间
    printf("p2: %p, p3: %p\n", p2, p3);
}
```

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800043f8(aligned to 0x80005000), HEAP_SIZE = 0xffffbc07,
num of reserved pages = 8, num of pages to be allocated for heap = 1048562
TEXT:   0x80000000 -> 0x800030a0
RODATA: 0x800030a0 -> 0x800032a8
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f8
HEAP:   0x8000d000 -> 0x7ffff000
Test: Splitting
p1: 0x8000d00c
Block at 0x8000d00c marked as free
p2: 0x8000d00c, p3: 0x8000d02c
```

**计算推导**

`p2_addr(0x8000d00c) + META(12) + Aligned_Data(20) = 0x8000d02c`，该地址恰好为 `p3` 的数据起始点。

**结论**：验证了 `split_block` 逻辑。分配器不再浪费整个页大块，而是精确切割并产生新的管理头。

---

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 功能测试 | 验证 `502328a` 版本的“相邻块合并（Coalescing）”功能 |

#### ✅ 测试报告（基于 `502328a`）

**测试用例**

```c
void malloc_test() {
    printf("Test: Coalescing\n");
    void *p1 = malloc(10);
    printf("Allocated p1 at %p\n", p1);
    void *p2 = malloc(10);
    printf("Allocated p2 at %p\n", p2);
    free(p1);
    free(p2);
    void *p3 = malloc(30); // 如果合并成功，p3 能拿到 p1 的起始地址
    printf("Allocated p3 at %p\n", p3);
    if (p3 == p1) printf("SUCCESS: Coalesce working!\n");
}
```

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800043f8(aligned to 0x80005000), HEAP_SIZE = 0xffffbc07,
num of reserved pages = 8, num of pages to be allocated for heap = 1048562
TEXT:   0x80000000 -> 0x80003174
RODATA: 0x80003174 -> 0x800033bc
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f8
HEAP:   0x8000d000 -> 0x7ffff000
Test: Coalescing
Allocated p1 at 0x8000d00c
Allocated p2 at 0x8000e00c
Block at 0x8000d00c marked as free
Block at 0x8000e00c marked as free
Allocated p3 at 0x8000d00c
SUCCESS: Coalesce working!
```

**结论**：验证了 `coalesce` 函数能够正确识别物理上相邻的空闲块并修改 `next` 指针完成合并，有效解决了外部碎片导致无法分配大对象的问题。

---

**提交编号**  
本次实验的最终提交编号为：ad31236

## 📝 第 3 次实验

本次实验目标是实现协作式调度器，支持任务切换；任务结构体设计、上下文切换、FIFO调度、带优先级的彩票调度。

**题目要求**

以教师提供的代码为基础，实现以下内容：

- 掌握使用汇编语言实现完整的上下文切换（context switch）。
- 学会实现任务创建（task_create）与任务让出（task_yield）接口。
- 通过 UART 输出验证多任务并发执行的正确性。
- 理解 mscratch 寄存器在调度器中的巧妙运用。
- 完成 FIFO调度、带优先级的彩票调度

**完成情况**

已完成第 3 次实验的全部要求。当前已经新增 `entry.S`、`sched.c` 和 `user.c`，形成完整的多任务执行链：内核启动后先完成 UART 和堆初始化，再初始化调度器、创建演示任务并进入 `schedule()`。调度器采用“上下文结构 + 独立任务栈 + `mscratch` 保存当前任务指针”的实现方式，能够在 `task_yield()` 调用时保存当前任务的寄存器现场并恢复下一个任务的执行环境。


**执行输出**
```
------------------------------------
Hello, RVOS!
HEAP_START = 0x80007130(aligned to 0x80008000), HEAP_SIZE = 0xffff8ecf,
num of reserved pages = 8, num of pages to be allocated for heap = 1048559
TEXT:   0x80000000 -> 0x80003918
RODATA: 0x80003918 -> 0x80003d1f
DATA:   0x80004000 -> 0x80004008
BSS:    0x80004010 -> 0x80007130
HEAP:   0x80010000 -> 0x7ffff000
Setting Schedule Policy to Priority Weight...
Task 1 (Weight 2): Created!
Task 3 (Weight 4): Created!
Task 0 (Weight 1): Created!
Task 2 (Weight 3): Created!

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 10 times
Task 1 (Weight 2) runs: 25 times
Task 2 (Weight 3) runs: 31 times
Task 3 (Weight 4) runs: 34 times
-----------------------------

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 6 times
Task 1 (Weight 2) runs: 17 times
Task 2 (Weight 3) runs: 32 times
Task 3 (Weight 4) runs: 45 times
-----------------------------

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 13 times
Task 1 (Weight 2) runs: 18 times
Task 2 (Weight 3) runs: 25 times
Task 3 (Weight 4) runs: 44 times
-----------------------------
```

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 最终测试 | 负责带优先级的彩票调度的测试 |
| 协作整合 | 负责审核并合并成员分支，维护 `master` 主线完整性 |

#### ✅ 测试报告
**测试用例**

见 user.c 文件代码

**实验结果**

```
------------------------------------
Hello, RVOS!
HEAP_START = 0x80007130(aligned to 0x80008000), HEAP_SIZE = 0xffff8ecf,
num of reserved pages = 8, num of pages to be allocated for heap = 1048559
TEXT:   0x80000000 -> 0x80003918
RODATA: 0x80003918 -> 0x80003d1f
DATA:   0x80004000 -> 0x80004008
BSS:    0x80004010 -> 0x80007130
HEAP:   0x80010000 -> 0x7ffff000
Setting Schedule Policy to Priority Weight...
Task 1 (Weight 2): Created!
Task 3 (Weight 4): Created!
Task 0 (Weight 1): Created!
Task 2 (Weight 3): Created!

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 10 times
Task 1 (Weight 2) runs: 25 times
Task 2 (Weight 3) runs: 31 times
Task 3 (Weight 4) runs: 34 times
-----------------------------

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 6 times
Task 1 (Weight 2) runs: 17 times
Task 2 (Weight 3) runs: 32 times
Task 3 (Weight 4) runs: 45 times
-----------------------------

--- 100 Schedules Reached ---
Task 0 (Weight 1) runs: 13 times
Task 1 (Weight 2) runs: 18 times
Task 2 (Weight 3) runs: 25 times
Task 3 (Weight 4) runs: 44 times
-----------------------------
```

**测试分析**

提取日志中前 3 个周期（共计 300 次调度）的数据进行汇总分析：

| 任务 | 权重 | 理论期望占比 | 第1周期 | 第2周期 | 第3周期 | 实际总次数 (共300次) | 实际总占比 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Task 0** | 1 | 10.0% | 10 | 6 | 13 | **29** | **9.67%** |
| **Task 1** | 2 | 20.0% | 25 | 17 | 18 | **60** | **20.00%** |
| **Task 2** | 3 | 30.0% | 31 | 32 | 25 | **88** | **29.33%** |
| **Task 3** | 4 | 40.0% | 34 | 45 | 44 | **123** | **41.00%** |


**结论**：

1. **符合概率分布特征**：从实际总占比（`9.67%`, `20.00%`, `29.33%`, `41.00%`）可以看出，实际运行结果极度贴近理论期望值（`10%`, `20%`, `30%`, `40%`）。误差范围被控制在 ±1% 左右。
2. **随机数的波动性体现**：观察单一周期的结果（例如第 1 周期中 Task 3 只有 34 次，第 2 周期却有 45 次），可以看出伪随机数生成器（LCG）在小样本下具有合理的波动性。这是彩票调度算法的正常表现，随着总调度次数的增加（大数定律），整体分布会越来越平滑并趋近于设定权重。
3. **有效避免饥饿（Starvation）**：尽管 Task 0 的权重只有 1，且被调度的概率最低，但它在每个周期中都获得了 CPU 执行权，没有出现被高优先级任务完全饿死的现象。
4. 开发的优先级调度器功能通过测试，代码达到了预期的设计要求，可作为操作系统的基础调度策略进行后续应用程序的开发与测试。

---

### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 重构任务管理结构 | 引入 `task_info` 结构体，为 `task_create` 加上优先级参数 |
| 实现优先级权重调度算法 | 实现线性同余伪随机数生成器，增加全局调度策略开关，分别对应 FIFO 模式和带优先级的彩票调度模式 |
| 报告编写 | 根据实验过程编写实验报告 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 重构任务管理结构 | 570eb7d | 引入 `task_info` 结构体，为 `task_create` 加上优先级参数，将**sched.c**中的 `context` 数组改为 `task_info` 数组，并修改对应逻辑的取地址方式|
| 实现优先级权重调度算法 | e5247e6 | 实现了线性同余伪随机数生成器，用于后续的优先级调度算法，并增加全局调度策略开关，分别对应 FIFO 模式和带优先级的彩票调度模式，并修改调度器的调度逻辑，使其能够根据全局调度策略开关选择相应的调度算法 |

### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

**提交编号**  
本次实验的最终提交编号为：e5247e6

## 📝 第 4 次实验

本次实验目标是模拟 `7 / 5 / 2 / 3` 四类同步异常，要求内核能够正确捕获异常、输出对应提示信息，并在实验触发点后继续执行。

**题目要求**

以教师提供的代码为基础，实现以下内容：

- 理解 RISC-V 同步异常（Synchronous Exception）的基本概念与常见类型。
- 掌握非法内存访问（访存错误）如何触发同步异常。
- 学会配置陷阱向量（mtvec）并实现基础的异常处理函数。
- 通过故意写入空地址，观察“存储访问错误”（Store Access Fault）。
- 实现异常后的安全恢复，避免系统崩溃。
- 模拟 `7 / 5 / 2 / 3` 四类同步异常，要求内核能够正确捕获异常、输出对应提示信息，并在实验触发点后继续执行。

**完成情况**

成功实现了 RISC-V 架构下的 Trap 异常捕获与恢复。系统现在能准确处理 非法指令 (Code 2)、断点 (Code 3) 及 访存错误 (Code 5/7)。实验证明，内核在触发异常后可以精准跳过故障指令，恢复原任务执行，展现了良好的多任务稳定性。我们在 trap_handler 中通过读取 epc 指向的指令低两位进行判断——若 (insn & 0x3) == 0x3 则判定为 32 位指令（PC +4），否则判定为 16 位指令（PC +2）。这解决了因 “硬编码`+4`”   导致的指令对齐错误和死循环。


**执行输出**
```
------------------------------------
Hello, RVOS!
HEAP_START = 0x8000811c(aligned to 0x80009000), HEAP_SIZE = 0xffff7ee3,
num of reserved pages = 8, num of pages to be allocated for heap = 1048558
TEXT:   0x80000000 -> 0x80003b08
RODATA: 0x80003b08 -> 0x80004048
DATA:   0x80005000 -> 0x80005008
BSS:    0x80005010 -> 0x8000811c
HEAP:   0x80011000 -> 0x7ffff000
Task 0: Created!
Task 0: Running...

--- Trap Test Start ---
Testing Illegal instruction (Code 2)...
Sync exceptions! Code = 2
Exception: Illegal instruction!
Testing Breakpoint (Code 3)...
Sync exceptions! Code = 3
Exception: Breakpoint!
Testing Load access fault (Code 5)...
Sync exceptions! Code = 5
Exception: Load access fault!
Testing Store/AMO access fault (Code 7)...
Sync exceptions! Code = 7
Exception: Store/AMO access fault!
Yeah! I'm return back from trap safely!
--- Trap Test End ---

Task 1: Created!
Task 1: Running...
Task 0: Running...

--- Trap Test Start ---
Testing Illegal instruction (Code 2)...
Sync exceptions! Code = 2
Exception: Illegal instruction!
Testing Breakpoint (Code 3)...
Sync exceptions! Code = 3
Exception: Breakpoint!
Testing Load access fault (Code 5)...
Sync exceptions! Code = 5
Exception: Load access fault!
Testing Store/AMO access fault (Code 7)...
Sync exceptions! Code = 7
Exception: Store/AMO access fault!
Yeah! I'm return back from trap safely!
--- Trap Test End ---
```

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 协作整合 | 负责审核并合并成员分支，维护 `master` 主线完整性 |


### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |


### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 基础环境搭建 | 移植 RISC-V 基础环境，确保 baseline 成功运行|
| 四种同步异常模拟 | 实现对四种同步异常的模拟 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 基础环境搭建 | a7ad3b7 | 根据已有基础代码，补充 CSR 封装部分，清理 `sched.c` 中重复的手工寄存器访问逻辑，避免调度器和 trap 路径各自维护一套 CSR 接口|
| 四种同步异常模拟 | 31ea8f8 | 修改 `trap.c` 文件代码，顺序触发 `7/5/2/3` 四类异常并输出返回提示|

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

**提交编号**  
本次实验的最终提交编号为：31ea8f8

## 📝 第 5 次实验

本次实验目标是利用中断机制，实现从键盘获取整行输入，输出整行数据，和自定义的提示信息。

**题目要求**

以教师提供的代码为基础，实现以下内容：

- 理解 RISC-V 外部中断（External Interrupt）的基本概念与处理流程。
- 掌握 UART 控制器中断的使能与配置方法。
- 学会实现中断处理程序（Interrupt Service Routine, ISR）。
- 通过 UART 接收中断实现字符回显功能。
- 理解中断嵌套与上下文保存的完整性。
- 实现从键盘获取整行输入，输出整行数据，和自定义的提示信息。

**完成情况**

已完成第 5 次实验的全部要求。新增 `plic.c`，并在 `platform.h` 中补充 UART0 的 PLIC 中断源定义与 machine-mode 所需寄存器映射；`kernel.c` 的启动流程也已经在 `trap_init()` 之后接入 `plic_init()`，使 UART 外部中断能够在内核启动后立即被启用。在串口输入路径上，为 UART 设计了一个中断驱动的环形接收缓冲区：UART 中断服务函数负责把收到的字符规范化后写入软件队列，命令任务则在用户态循环从 `uart_getc()` 读取缓冲内容并自行完成回显、退格处理和回车提交。最终 `user.c` 中的 `command_task` 被替换为单一交互任务，启动后首先打印 `Task cmd: Created! `，随后不断读取一整行命令；当检测到回车时，内核输出 `The command is: <cmd>`，并继续显示 `What should I do? ` 提示，进入下一轮等待。


**执行输出**
```
------------------------------------
Hello, RVOS!
HEAP_START = 0x8000819c(aligned to 0x80009000), HEAP_SIZE = 0xffff7e63,
num of reserved pages = 8, num of pages to be allocated for heap = 1048558
TEXT:   0x80000000 -> 0x80003fac
RODATA: 0x80003fac -> 0x800044fc
DATA:   0x80005000 -> 0x80005008
BSS:    0x80005010 -> 0x8000819c
HEAP:   0x80011000 -> 0x7ffff000
Task cmd: Created!
fhaujkdfhdsuf
The command is: fhaujkdfhdsuf
What should I do?
vhreuia
The command is: vhreuia
What should I do?
bvsuai
The command is: bvsuai
What should I do?
vbruiea
The command is: vbruiea
What should I do?
```

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 协作整合 | 负责审核并合并成员分支，维护 `master` 主线完整性 |


### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |


### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 基础环境搭建 | 移植 RISC-V 基础环境，确保 baseline 成功运行|
| 输入环形缓冲队列实现 | 为 UART 设计实现中断驱动的环形接收缓冲区 |
| 环形队列输出 | 改写 `uart_getc` 实现从环形队列读取 |
| 命令输入与交互式输出 | 从命令行读取字符串，在按下回车后输出识别到的命令内容。实现循环式交互，使系统在处理完一条命令后继续等待下一条命令输入。 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 基础环境搭建 | aef0742 | 移植 RISC-V 基础环境，确保 baseline 成功运行|
| 输入环形缓冲队列实现 | 1d1c500 | 实现中断驱动的环形接收缓冲区，设置缓冲区长度为128 |
| 环形队列输出 | 94f5cf6 | 改写 `uart_getc` 实现从环形队列读取已经存入的字符 |
| 命令输入与交互式输出 | 48f395e | 从命令行读取字符串，处理输入回退等，在按下回车后输出识别到的命令内容。实现循环式交互，使系统在处理完一条命令后继续等待下一条命令输入。 |

**提交编号**  
本次实验的最终提交编号为：48f395e

## 📝 第 6 次实验

本次实验目标是实现硬件定时器，在指定时间间隔内输出tck信息。

**题目要求**

以教师提供的代码为基础，实现以下内容：
- 理解 RISC-V 平台中 CLINT（Core-Local Interruptor）模块的作用。
- 掌握硬件定时器（Timer）中断的配置与使能方法。
- 学会实现定时器中断服务程序（ISR）。
- 通过定时器中断实现周期性任务（如打印信息）。
- 区分“中断处理”与“抢占式调度”的不同。


**完成情况**

已完成第 6 次实验的全部要求。新增 `timer.c`，并在 `platform.h` 中补充相关定义；`kernel.c` 的启动流程也已经在 `plic_init()` 之后接入 `timer_init()`，成功移植代码并能够正确输出信息


**执行输出**
```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800081a0(aligned to 0x80009000), HEAP_SIZE = 0xffff7e5f,
num of reserved pages = 8, num of pages to be allocated for heap = 1048558
TEXT:   0x80000000 -> 0x80004100
RODATA: 0x80004100 -> 0x80004652
DATA:   0x80005000 -> 0x80005008
BSS:    0x80005010 -> 0x800081a0
HEAP:   0x80011000 -> 0x7ffff000
Task 0: Created!
Task 0: Running...
Task 1: Created!
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
timer interruption!
tick: 1
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
timer interruption!
tick: 2
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
timer interruption!
tick: 3
Task 1: Running...
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
```

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 协作整合 | 负责审核并合并成员分支，维护 `master` 主线完整性 |
| 移植代码 | 将教师提供的源代码适配整合到当前版本 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 基础环境搭建 | a92a9b0 | 移植 RISC-V 基础环境，确保 baseline 成功运行，并成功输出，完成实验要求|

### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |


### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

**提交编号**  
本次实验的最终提交编号为：a92a9b0

## 📝 第 6 次实验

实现抢占式任务切换（通过timer interrupt）和协同式任务切换（通过software interrupt）相结合，看到多个任务切换的效果.

**题目要求**

以教师提供的代码为基础，实现以下内容：
- 理解 RISC-V 机器模式陷阱向量（trap vector）的完整处理流程。
- 掌握在汇编层面对所有通用寄存器进行保存与恢复的方法。
- 学会正确处理 mscratch 寄存器在上下文切换中的特殊角色。
- 理解 t6 寄存器作为 reg_save/reg_restore 基地址的巧妙设计。
- 实现一个健壮、完整的上下文切换机制，为后续功能（如系统调用）打下基础。



**完成情况**

已完成第 7 次实验的全部要求。改写 `entry.S` 中的 `switch_to` 函数，并在 `sched.c` 中完善调度策略，使得抢占式任务切换和协同式任务切换能够相互配合，并能够正确输出信息


**执行输出**
```
------------------------------------
Hello, RVOS!
HEAP_START = 0x800081c8(aligned to 0x80009000), HEAP_SIZE = 0xffff7e37,
num of reserved pages = 8, num of pages to be allocated for heap = 1048558
TEXT:   0x80000000 -> 0x800041a8
RODATA: 0x800041a8 -> 0x80004766
DATA:   0x80005000 -> 0x80005008
BSS:    0x80005010 -> 0x800081c8
HEAP:   0x80011000 -> 0x7ffff000
Task 0: Created!
software interruption!
Task 1: Created!
Task 1: Running (Preemptive)...
Task 1: Running (Preemptive)...
Task 1: Running (Preemptive)...
timer interruption!
tick: 1
Task 2: Created!
Task 2: Running (Preemptive)...
Task 2: Running (Preemptive)...
Task 2: Running (Preemptive)...
timer interruption!
tick: 2
Task 0: I'm back!
Task 0: Running (Cooperative)...
software interruption!
Task 1: Running (Preemptive)...
Task 1: Running (Preemptive)...
timer interruption!
tick: 3
Task 2: Running (Preemptive)...
Task 2: Running (Preemptive)...
Task 2: Running (Preemptive)...
timer interruption!
tick: 4
Task 0: Running (Cooperative)...
software interruption!
QEMU: Terminated
```

### 👤 薛鹏（XuePengis）

**✉️ 提交邮箱**：2082994803@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 协作整合 | 负责审核并合并成员分支，维护 `master` 主线完整性 |
| 移植代码 | 将教师提供的源代码适配整合到当前版本 |
| 优化中断开启的时机 | 将全局中断的开启推迟到第一次 `schedule()` 启动任务之时，保证多方式切换的正确性 |
| 编写多任务测试用例验证混合调度 | 编写多个任务的测试用例，验证混合调度的正确性 |

#### ✅ 提交记录

| 任务模块 | 提交编号 | 完成情况 |
| :--- | :--- | :--- |
| 基础环境搭建 | ee5e79a | 移植 RISC-V 基础环境，确保 baseline 成功运行，并成功输出|
| 优化中断开启的时机 | b769638 | 当前代码在 `timer_init()` 中就开启了全局中断（MSTATUS_MIE），导致无法实现多任务切换，因此我们应该将全局中断的开启推迟到第一次 `schedule()` 启动任务之时。|
| 编写多任务测试用例验证混合调度 | bec7db4 | 编写明确的测试代码，Task 0：纯协同式任务，主动调用 task_yield() 让出 CPU。Task 1 和 Task 2：死循环型计算任务（不调用 yield），用来验证定时器中断是否能强制抢占它们。|

### 👤 杨怡萱（yangyixuan）

**✉️ 提交邮箱**：yangyixuan0829@qq.com@example.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |


### 👤 徐蜚遥（徐蜚遥）

**✉️ 提交邮箱**：1605756286@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

### 👤 姚翎（Yao-Ling）

**✉️ 提交邮箱**：748262229@qq.com

#### 📌 任务分工

| 任务模块 | 任务描述 |
| :--- | :--- |
| 无 | 无 |

**提交编号**  
本次实验的最终提交编号为：bec7db4
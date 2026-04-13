# Custom Macro Definition

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart.c \

DEFS +=

# Common Part for the Makefile

CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS += -nostdlib -fno-builtin -g -Wall
CFLAGS += -march=rv32g -mabi=ilp32

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
# 新增：定义 objcopy 工具
OBJCOPY = ${CROSS_COMPILE}objcopy
MKDIR = mkdir -p
RM = rm -rf

OUTPUT_PATH = out

OBJS_ASM := $(addprefix ${OUTPUT_PATH}/, $(patsubst %.S, %.o, ${SRCS_ASM}))
OBJS_C   := $(addprefix $(OUTPUT_PATH)/, $(patsubst %.c, %.o, ${SRCS_C}))
OBJS = ${OBJS_ASM} ${OBJS_C}

ELF = ${OUTPUT_PATH}/os.elf
# 新增：定义目标 bin 文件路径
BIN = ${OUTPUT_PATH}/os.bin

LDFLAGS = -T os.ld

.DEFAULT_GOAL := all
# 修改：将 ${BIN} 加入默认构建目标
all: ${OUTPUT_PATH} ${ELF} ${BIN}

${OUTPUT_PATH}:
	@${MKDIR} $@

# start.o must be the first in dependency!
${ELF}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o ${ELF} $^

# 新增：通过 objcopy 将 elf 转换为纯二进制 bin 文件
${BIN}: ${ELF}
	${OBJCOPY} -O binary ${ELF} ${BIN}

${OUTPUT_PATH}/%.o : %.c
	${CC} ${DEFS} ${CFLAGS} -c -o $@ $<

${OUTPUT_PATH}/%.o : %.S
	${CC} ${DEFS} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${ELF}

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${ELF} -s -S &
	@${GDB} ${ELF} -q -x gdbinit

.PHONY : clean
clean:
	@${RM} ${OUTPUT_PATH}
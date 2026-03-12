OUTPUT_ARCH( "riscv" )
ENTRY( _start )
MEMORY
{
    ram_text (rx) : ORIGIN = 0x80000000, LENGTH = 0x8000000
    ram_data (rw) : ORIGIN = 0x84000000, LENGTH = 0x4000000
}
SECTIONS
{
    .text : {
        . = ALIGN(4096);
        PROVIDE(_text_start = .);
        *(.text .text.*)
        PROVIDE(_text_end = .);
    } >ram_text
    .rodata : {
        . = ALIGN(4096);
        PROVIDE(_rodata_start = .);
        *(.rodata .rodata.*)
        PROVIDE(_rodata_end = .);
    } >ram_data
    .data : {
        . = ALIGN(4096);
        PROVIDE(_data_start = .);
        *(.sdata .sdata.*)
        *(.data .data.*)
        PROVIDE(_data_end = .);
    } >ram_data
    .bss : {
        . = ALIGN(4096);
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE(_bss_end = .);
    } >ram_data
    PROVIDE(_memory_start = ORIGIN(ram_text));
    PROVIDE(_memory_end = ORIGIN(ram_data) + LENGTH(ram_data));
    PROVIDE(_heap_start = _bss_end);
    PROVIDE(_heap_size = _memory_end - _heap_start);
}

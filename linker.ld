/* linker.ld - Memory layout for DELIX kernel (32-bit x86) */

ENTRY(_start)

SECTIONS
{
    /* Load kernel at 1MB (0x100000) — standard for protected mode */
    . = 0x100000;

    .text : {
        *(.multiboot)   /* Multiboot header MUST be first */
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
}

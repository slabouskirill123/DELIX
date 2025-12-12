; boot.asm - Multiboot-compliant entry point for DELIX kernel

section .multiboot
align 4
dd 0x1BADB002          ; Multiboot magic number
dd 0x0                 ; Flags: no special features
dd -0x1BADB002         ; Checksum (must sum to zero with magic)

section .text
global _start
extern kernel_main

_start:
    cli                    ; Disable interrupts
    mov esp, stack_top     ; Set up stack
    call kernel_main       ; Jump to C code
    hlt                    ; Halt if kernel returns (should not happen)
.Lhang:
    jmp .Lhang             ; Infinite loop

section .bss
align 16
stack_bottom:
    resb 8192              ; 8 KB stack
stack_top:

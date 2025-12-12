/* kernel.c - DELIX kernel entry point */

#include "terminal.h"
#include "terminal_commands.h"

/* Write a byte to an I/O port */
void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* Read a byte from an I/O port */
unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Compare two null-terminated strings */
int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

/* Kernel main entry point */
void kernel_main(void) {
    terminal_initialize();
    print("Hello from DELIX Kernel\n");
    print("> ");

    char input[32];
    while (1) {
        readline(input, sizeof(input));

        /* Skip empty input */
        if (strcmp(input, "") == 0) {
            print("> ");
            continue;
        }

        /* Search for matching command */
        int found = 0;
        int num = get_num_commands();
        for (int i = 0; i < num; i++) {
            const struct command* cmd = get_command(i);
            if (strcmp(input, cmd->name) == 0) {
                cmd->func();
                found = 1;
                break;
            }
        }

        /* Handle unknown command */
        if (!found) {
            print("Unknown command. Type 'help' for list of available commands.\n");
        }

        print("> ");
    }
}

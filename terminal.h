/* terminal.h - Terminal interface for DELIX kernel */

#ifndef TERMINAL_H
#define TERMINAL_H
/* Terminal color schemes */
#define TERM_COLOR_DARK  0x07  // white on black
#define TERM_COLOR_LIGHT 0x70  // black on white

#include <stddef.h>

/* Hardware I/O functions (implemented in kernel.c) */
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

/* Terminal control */
void terminal_set_color(unsigned char color);
void terminal_initialize(void);
void terminal_clear(void);
void terminal_putchar(char c);
void print(const char* str);
void readline(char* buffer, int maxlen);

#endif

/* terminal.c - VGA terminal with color themes (no arrow navigation) */

#include "terminal.h"

#define VGA_BUFFER ((volatile short*)0xB8000)
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static unsigned char terminal_color = TERM_COLOR_DARK;  // white on black

/* Simple I/O delay */
static inline void io_wait(void) {
    for (volatile int i = 0; i < 100; i++);
}

/* Update hardware cursor position */
static void update_cursor(void) {
    unsigned short pos = (unsigned short)(terminal_row * VGA_WIDTH + terminal_column);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
}

/* Clear screen with current color */
void terminal_clear(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i] = (short)(' ' | (terminal_color << 8));
    }
    terminal_row = 0;
    terminal_column = 0;
    update_cursor();
}

/* Initialize terminal */
void terminal_initialize(void) {
    terminal_clear();
}

/* Output a single character */
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            VGA_BUFFER[terminal_row * VGA_WIDTH + terminal_column] = (short)(' ' | (terminal_color << 8));
        }
    } else {
        VGA_BUFFER[terminal_row * VGA_WIDTH + terminal_column] = (short)(c | (terminal_color << 8));
        terminal_column++;
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    /* Scroll if needed */
    if (terminal_row >= VGA_HEIGHT) {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                VGA_BUFFER[(y - 1) * VGA_WIDTH + x] = VGA_BUFFER[y * VGA_WIDTH + x];
            }
        }
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (short)(' ' | (terminal_color << 8));
        }
        terminal_row = VGA_HEIGHT - 1;
    }

    update_cursor();
}

/* Print a null-terminated string */
void print(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}

/* Set terminal color scheme */
void terminal_set_color(unsigned char color) {
    terminal_color = color;
    /* Refresh entire screen with new color */
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        char c = (char)(VGA_BUFFER[i] & 0xFF);
        VGA_BUFFER[i] = (short)(c | (terminal_color << 8));
    }
    update_cursor();
}


void readline(char* buffer, int maxlen) {
    int pos = 0;

    while (1) {
        unsigned char scancode = inb(0x60);

        // Skip key release events
        if (scancode & 0x80) {
            continue;
        }

        // Handle Enter
        if (scancode == 0x1C) {
            buffer[pos] = '\0';
            print("\n");

            // Wait for key release to prevent repeat
            while (1) {
                unsigned char code = inb(0x60);
                if (code == (0x1C | 0x80)) { // release code
                    break;
                }
            }

            return; // Exit readline and return control to kernel_main
        }

        // Handle Backspace
        if (scancode == 0x0E && pos > 0) {
            pos--;
            terminal_putchar('\b');
            // Wait for release
            while ((inb(0x60) & 0x7F) != 0x0E || !(inb(0x60) & 0x80));
            continue;
        }

        // Handle printable chars
        char c = 0;
        switch (scancode) {
            case 0x10: c = 'q'; break; case 0x11: c = 'w'; break;
            case 0x12: c = 'e'; break; case 0x13: c = 'r'; break;
            case 0x14: c = 't'; break; case 0x15: c = 'y'; break;
            case 0x16: c = 'u'; break; case 0x17: c = 'i'; break;
            case 0x18: c = 'o'; break; case 0x19: c = 'p'; break;
            case 0x1E: c = 'a'; break; case 0x1F: c = 's'; break;
            case 0x20: c = 'd'; break; case 0x21: c = 'f'; break;
            case 0x22: c = 'g'; break; case 0x23: c = 'h'; break;
            case 0x24: c = 'j'; break; case 0x25: c = 'k'; break;
            case 0x26: c = 'l'; break;
            case 0x2C: c = 'z'; break; case 0x2D: c = 'x'; break;
            case 0x2E: c = 'c'; break; case 0x2F: c = 'v'; break;
            case 0x30: c = 'b'; break; case 0x31: c = 'n'; break;
            case 0x32: c = 'm'; break;
            case 0x02: c = '1'; break; case 0x03: c = '2'; break;
            case 0x04: c = '3'; break; case 0x05: c = '4'; break;
            case 0x06: c = '5'; break; case 0x07: c = '6'; break;
            case 0x08: c = '7'; break; case 0x09: c = '8'; break;
            case 0x0A: c = '9'; break; case 0x0B: c = '0'; break;
            case 0x39: c = ' '; break;
            default: continue;
        }

        if (pos < maxlen - 1) {
            buffer[pos++] = c;
            terminal_putchar(c);
            // Wait for key release
            while ((inb(0x60) & 0x7F) != scancode || !(inb(0x60) & 0x80));
        }
    }
}

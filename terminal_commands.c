/* terminal_commands.c - Built-in terminal commands for DELIX kernel */

#include "terminal_commands.h"
#include "terminal.h"
#include "utils/rtc/rtc.h"

#define KERNEL_NAME    "DELIX"
#define KERNEL_VERSION "V0.4"
#define KERNEL_ARCHITECTURE "i386 (x86)"

/* Show current time (hours:minutes) */
void cmd_clock(void) {
    int year, month, day, hour, minute, second;
    rtc_get_time(&year, &month, &day, &hour, &minute, &second);

    // Print in format: HH:MM
    void print_int2(int n) {  // Print 2 digits with leading zero if needed
        if (n < 10) terminal_putchar('0');
        if (n == 0) { print("0"); return; }
        char temp[16]; int i = 0;
        while (n > 0) { temp[i++] = '0' + (n % 10); n /= 10; }
        for (int j = i - 1; j >= 0; j--) terminal_putchar(temp[j]);
    }

    print_int2(hour); terminal_putchar(':'); print_int2(minute); print("\n");
}

/* Show current date and time from RTC */
void cmd_date(void) {
    int year, month, day, hour, minute, second;
    rtc_get_time(&year, &month, &day, &hour, &minute, &second);

    // Print in format: YYYY-MM-DD HH:MM:SS
    char buf[32];
    // Simple itoa for each value
    void print_int(int n) {
        if (n == 0) { print("0"); return; }
        char temp[16]; int i = 0;
        while (n > 0) { temp[i++] = '0' + (n % 10); n /= 10; }
        for (int j = i - 1; j >= 0; j--) terminal_putchar(temp[j]);
    }

    print_int(year); terminal_putchar('-');
    print_int(month); terminal_putchar('-');
    print_int(day); terminal_putchar(' ');
    print_int(hour); terminal_putchar(':');
    print_int(minute); terminal_putchar(':');
    print_int(second); print("\n");
}

/* Show kernel architecture */
void cmd_arch(void) {
    print(KERNEL_ARCHITECTURE);
    print("\n");
}

/* Switch to dark theme (white on black) */
void cmd_dark(void) {
    terminal_set_color(TERM_COLOR_DARK);
}

/* Switch to light theme (black on white) */
void cmd_light(void) {
    terminal_set_color(TERM_COLOR_LIGHT);
}

/* Show kernel version */
void cmd_version(void) {
    print(KERNEL_NAME);
    print(" Kernel ");
    print(KERNEL_VERSION);
    print("\n");
}

/* Reboot the system via keyboard controller */
void cmd_reset(void) {
    print("Resetting system...\n");
    unsigned char status;
    do {
        status = inb(0x64);
    } while (status & 0x02);  // wait until input buffer is clear
    outb(0x64, 0xFE);         // send reset command
    while (1) {
        __asm__ volatile("hlt");
    }
}

/* Clear the terminal screen */
void cmd_clear(void) {
    terminal_clear();
}

/* Display help with all available commands */
void cmd_help(void) {
    print("Kernel - Built-in commands:\n");
    int num = get_num_commands();
    for (int i = 0; i < num; i++) {
        const struct command* cmd = get_command(i);
        print("  ");
        print(cmd->usage);
        print("\n");
    }
}

/* Command registry */
static const struct command commands[] = {
    {"version",    cmd_version,    "version          - Show kernel version"},
    {"arch",       cmd_arch,       "arch             - Show kernel architecture"},
    {"help",       cmd_help,       "help             - Show this help message"},
    {"clear",      cmd_clear,      "clear            - Clear the screen"},
    {"reset",      cmd_reset,      "reset            - Reboot the system"},
    {"date",       cmd_date,       "date             - Show current date and time"},
    {"clock",      cmd_clock,      "clock            - Show current time (HH:MM)"},
    {"dark",       cmd_dark,       "dark             - Set dark theme (white on black)"},
    {"light",      cmd_light,      "light            - Set light theme (black on white)"},
};

int get_num_commands(void) {
    return sizeof(commands) / sizeof(commands[0]);
}

const struct command* get_command(int index) {
    return &commands[index];
}

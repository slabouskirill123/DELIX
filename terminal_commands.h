#ifndef TERMINAL_COMMANDS_H
#define TERMINAL_COMMANDS_H


void cmd_help(void);
void cmd_arch(void);

void cmd_version(void);

/* Command descriptor */
struct command {
    const char* name;        // e.g. "version"
    void (*func)(void);      // handler function
    const char* usage;       // e.g. "version - Show kernel version"
};

/* Get total number of registered commands */
int get_num_commands(void);

/* Get command descriptor by index */
const struct command* get_command(int index);

#endif

// structs.h
#ifndef USH_STRUCTS_H
#define USH_STRUCTS_H

typedef struct var_s {
    char *name;
    char *value;
} var_t;

typedef struct token_s {
    int start;
    int length;
    char *value;
    bool is_minimized;
} token_t;

typedef struct process_s {
    int id;
    pid_t pid;
    char *name;
    int status;
} process_t;

typedef struct app_s {
    bool is_running;
    t_list *vars;
    t_list *commands;
    t_list *processes;
} app_t;

typedef struct command_s {
    char *name;
    int (*func)(app_t *app, char **args);
} command_t;

#endif // !USH_STRUCTS_H

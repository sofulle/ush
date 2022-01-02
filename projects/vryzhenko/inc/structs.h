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
} token_t;

typedef struct app_s {
    bool is_running;
    t_list *vars;
} app_t;

typedef struct command_s {
    char *name;
    int (*func)(app_t *app, char *name, char **args);
} command_t;

#endif // !USH_STRUCTS_H

// functions.h
#ifndef USH_FUNCTIONS_H
#define USH_FUNCTIONS_H

#include "ush.h"

// app
app_t *app_init();
void app_exit(app_t *app);

// line
int line_read(char **line);
int line_parse(char *line, char ***commands);

// command
int command_handle(app_t *app, char **command, bool primary);
int command_replace_vars(app_t *app, char **command);
int command_run(app_t *app, char *name, char **args);
int command_add(app_t *app, char *name, int (*func)(app_t *, char *, char **));

// var
void var_set(app_t *app, char *name, char *value);
char *var_get(app_t *app, char *name);
var_t *var_search(app_t *app, char *name);

// exec
int exec_exit(app_t *app, char *name, char **args);

// utils
void prompt_print(app_t *app);

#endif // !USH_FUNCTIONS_H

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
int command_split_args(app_t *app, char *command, char **name, char ***args);
int command_run(app_t *app, char *name, char **args);
int command_launch(app_t *app, char *name, char **args);
void command_add(app_t *app, char *name, int (*func)(app_t *, char **));
int command_wait(app_t *app, process_t *process, uint32_t id);

// var
void var_set(app_t *app, char *name, char *value);
void var_unset(app_t *app, char *name);
char *var_get(app_t *app, char *name);
var_t *var_search(app_t *app, char *name);

// exec
int exec_exit(app_t *app, char **args);
int exec_echo(app_t *app, char **args);
int exec_setvar(app_t *app, char **args);
int exec_export(app_t *app, char **args);
int exec_jobs(app_t *app, char **args);
int exec_fg(app_t *app, char **args);
int exec_unset(app_t *app, char **args);

int exec_vector(app_t *app, char **args);

// utils
void prompt_print(app_t *app);
int get_args_count(char *str);
int get_char_count(char *str, char c);
bool is_valid_str(char *str);

#endif // !USH_FUNCTIONS_H

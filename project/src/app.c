// app.c

#include "ush.h"

app_t *app_init() {
    app_t *app = malloc(sizeof(app_t));
    int shlvl = 0;

    app->is_running = true;
    app->vars = NULL;
    app->processes = NULL;
    app->commands = NULL;
    app->last_status = 0;

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGCONT, SIG_DFL);

    shlvl = atoi(getenv("SHLVL"));
    shlvl++;
    setenv("SHLVL", mx_itoa(shlvl), true);

    command_add(app, "exit", exec_exit);
    command_add(app, "bye", exec_exit);
    command_add(app, "echo", exec_echo);
    command_add(app, "export", exec_export);
    command_add(app, "unset", exec_unset);
    command_add(app, "jobs", exec_jobs);
    command_add(app, "fg", exec_fg);
    command_add(app, "pwd", exec_pwd);
    command_add(app, "cd", exec_cd);
    command_add(app, "which", exec_which);

    return app;
}

void app_exit(app_t *app) {
    free(app);
}

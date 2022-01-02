// main.c

#include "ush.h"

int main(int argc, char **argv) {
    app_t *app = app_init();

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    if(argc > 1) {
        if(strcmp(argv[1], "stp") == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
        }
    }

    var_set(app, "PROMPT", "u$h> ");

    var_set(app, "key", "blue red rabbit");
    var_set(app, "x", "0");
    var_set(app, "y", "1");

    command_add(app, "exit", exec_exit);
    command_add(app, "echo", exec_echo);
    command_add(app, "jobs", exec_jobs);
    command_add(app, "fg", exec_fg);
    
    // ? env: HOME, PWD, OLDPWD

    while(app->is_running) {
        char *line = NULL;
        char **commands = NULL;

        if(isatty(STDOUT_FILENO)) prompt_print(app);
        line_read(&line);
        line_parse(line, &commands);
    
        for(char **command = commands; *command; command++) {
            command_handle(app, command, true);
        }

        free(line);
        mx_del_strarr(&commands);
    }

    app_exit(app);
    return 0;
}
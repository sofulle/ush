// main.c

#include "ush.h"

int main(int argc, char **argv) {
    app_t *app = app_init();

    // ! debug
    if(argc > 1) {
        if(strcmp(argv[1], "1") == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
        }
        var_set(app, "PROMPT", argv[2]);
    }
    else {
        var_set(app, "PROMPT", "u$h> ");
    }

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

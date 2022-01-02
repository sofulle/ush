// main.c

#include "ush.h"

int main() {
    app_t *app = app_init();

    var_set(app, "PROMPT", "u$h> ");

    var_set(app, "key", "blue red rabbit");
    var_set(app, "x", "0");
    var_set(app, "y", "1");
    
    // ? env: HOME, PWD, OLDPWD

    while(app->is_running) {
        char *line = NULL;
        char **commands = NULL;

        prompt_print(app);
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

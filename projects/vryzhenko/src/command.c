// command.c

#include "ush.h"

/*

    command_handle(app, command):
        if command has ~, -, /:
            replace template

        if command has ${}:
            replace variables

        if command has $():
            for subcommand in subcommands:
                command_handle(app, subcommand)

        command_run(app, command)

*/

int command_handle(app_t *app, char **command, bool primary) {
    char *name = NULL;
    char **args = NULL;

    command_replace_vars(app, command);

    args = mx_strsplit(*command, ' ');
    name = args[0];

    command_run(app, name, args);

    mx_del_strarr(&args);

    if(primary) return 0;
    return 0;
}

int command_replace_vars(app_t *app, char **command) {
    char *str = *command;
    int len = strlen(str);
    t_list *tokens = NULL;
    int vars_len = 0;
    int tokens_len = 0;
    int new_len = 0;
    char *new_command = NULL;

    for (int i = 0; i < len; i++) {
        int token_start = -1;
        int token_length = -1;
        char *token_value = NULL;

        if(i + 1 < len)
            if(str[i] == '$' && str[i + 1] == '{') {
                if(i - 1 >= 0)
                    if(str[i - 1] == '\\') continue;

                token_start = i;

                for (int j = i + 2; j < len; j++) {
                    if(str[j] == '}') {
                        token_length = j - token_start - 2;
                        i = j;
                        break;
                    }
                }

                if(token_length == -1) continue;

                token_value = (char *) malloc(token_length * sizeof(char) + 1);
                token_value[token_length] = '\0';

                for (int j = 0; j < token_length; j++) {
                    token_value[j] = str[token_start + j + 2];
                }
            }

        if(token_start != -1) {
            token_t *token = (token_t *) malloc(sizeof(token_t));
            token->start = token_start;
            token->length = token_length;
            token->value = token_value;

            if(tokens == NULL) {
                tokens = mx_create_node(token);
            }
            else {
                mx_push_back(&tokens, token);
            }
        }
    }

    for(t_list *node = tokens; node != NULL; node = node->next) {
        token_t *token = (token_t *)node->data;
        tokens_len = tokens_len + token->length + 3;
        //printf("start: %d\tlength: %d\tname: %s\tvalue: %s\n", token->start, token->length, token->value, var_get(app, token->value));
    }
    for(t_list *node = tokens; node != NULL; node = node->next) {
        token_t *token = (token_t *)node->data;
        vars_len += strlen(var_get(app, token->value));
    }

    new_len = len - tokens_len + vars_len;

    new_command = (char *) malloc((new_len + 1) * sizeof(char));
    memset(new_command, 0, new_len + 1);

    int last_pos = 0, last_pos_new = 0;
    for(t_list *node = tokens; node != NULL; node = node->next) {
        token_t *token = (token_t *)node->data;
        char *value = var_get(app, token->value);
        
        for (int i = last_pos; i < token->start; i++, last_pos_new++) {
            new_command[last_pos_new] = str[i];
        }

        for (unsigned int i = 0; i < strlen(value); i++, last_pos_new++) {
            new_command[last_pos_new] = value[i];
        }

        last_pos = token->start + token->length + 3;
    }
    for (int i = last_pos; i < len; i++, last_pos_new++) {
        new_command[last_pos_new] = str[i];
    }

    free(*command);
    *command = new_command;

    return 0;
}

int command_run(app_t *app, char *name, char **args) {

    app->is_running = app->is_running;

    args = NULL;

    printf("%s\n", name);

    return 0;
}

// int command_add(app_t *app, char *name, int (*func)(app_t *, char *, char **)) {

//     app->is_running = app->is_running;

//     args = NULL;

    

//     return 0;
// }


// line.c

#include "ush.h"

int line_read(char **line) {
    size_t bufsize = 0;

    *line = (char *) malloc(LINE_LENGTH * sizeof(char));
    if(*line == NULL) return 1;

    memset(*line, 0, LINE_LENGTH * sizeof(char));
    getline(line, &bufsize, stdin);

    return 0;
}

int line_parse(char *line, char ***commands) {
    *commands = mx_strsplit(line, COMMANDS_SEPARATOR);

    for(char **command = *commands; *command; command++) {
        char *clear_command = mx_strtrim(*command);
        free(*command);
        *command = clear_command;
    }

    if(*commands) return 0;
    else return 1;
}

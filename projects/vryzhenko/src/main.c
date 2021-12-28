// main.c

#include "ush.h"

// int argc, char **argv

int main(void) {
    app_t *app = app_init();

    while(1) {
        char *line = (char *) malloc(1024 * sizeof(char));
        size_t bufsize = 0;

        printf("u$h> ");
    
        getline(&line, &bufsize, stdin);

        printf("%s", line);

        free(line);
    }

    app_exit(app);
    return 0;
}

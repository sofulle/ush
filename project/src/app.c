// app.c

#include "ush.h"

app_t *app_init() {
    app_t *app = malloc(sizeof(app_t));

    app->is_running = true;
    app->vars = NULL;
    app->processes = NULL;
    app->commands = NULL;

    return app;
}

void app_exit(app_t *app) {
    free(app);
}

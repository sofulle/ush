// app.c

#include "ush.h"

app_t *app_init() {
    app_t *app = malloc(sizeof(app_t));

    

    return app;
}

void app_exit(app_t *app) {
    free(app);
}

#include "ush.h"

int unset(char **split) {
    while (*split) {
        unsetenv(*split);
        split++;
    }
    return 0;
}

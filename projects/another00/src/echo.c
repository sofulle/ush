#include "ush.h"
int echo(char **split) {

    int i;
    int j;
    int a = 1;
    char *d;

    if (split[0]) {
        j = 0;
        while (split[j]) {
            i = 0;
            while (split[j][i]) {
                if (split[j][i] == '-' && split[j][i + 1] == 'e' &&
                    split[j + 1][i] == '"') {
                    d = split[j + 1];
                    a = 0;
                }

                i++;
            }
            j++;
        }
    }
    if (a == 1) {
        printf("%s\n", split[0]);
    }
    if (a == 0) {
        for (unsigned long z = 0; z < strlen(d); z++) {
            if (d[z] != '"') {
                if (d[z] == '\\' && d[z + 1] == 'n' &&
                    d[z + 2] != '"') {
                    printf("%c", '\n');
                    z += 2;
                }
                if (d[z] == '\\' && d[z + 1] == 't' &&
                    d[z + 2] != '"') {
                    printf("%c", '\t');
                    z += 2;
                }
                if (d[z + 2] == '"' && d[z] == '\\' &&
                    d[z + 1] == 'n') {
                    z += 3;
                    printf("%c", '\n');
                    printf("%c", '\n');
                }
                if (d[z + 2] == '"' && d[z] == '\\' &&
                    d[z + 1] == 't') {
                    z += 3;
                    printf("%c", '\t');
                    printf("%c", '\t');
                }
                printf("%c", d[z]);
                if (d[z + 1] == '"') {
                    printf("%c", '\n');
                }
            }
        }
    }

    return 0;
}

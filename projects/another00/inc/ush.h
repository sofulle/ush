#ifndef USH_H
#define USH_H

#define _GNU_SOURCE 1
#define _POSIX_C_SOURCE 200809L

#include "libmx.h"
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SPLIT_BUFSIZE 64


extern char **environ;
extern int errno;
typedef struct s_proc {
    pid_t pid;
    struct s_proc *prev;
}              t_proc;
enum {
  flag_i  = 1 << 0,
  flag_P  = 1 << 1,
  flag_u  = 1 << 2,
  flag_s  = 1 << 3,
  flag_L  = 1 << 4,
  flag_a  = 1 << 5,
  flag_n  = 1 << 6,
  flag_e  = 1 << 7,
  flag_E  = 1 << 8,
  boolean = 1 <<9
};

int main(void);
int init(void);
int loop(void);
int quit(void);
char *line(void);
char *get_line(void);
int fg(t_proc **pids);
int echo(char **split);
void which(char **split);
int unset(char **split);
int export(char **split);
void wheeich(char *temp);
int launch(char **split, t_proc **pids);
char **get_split(char *line, const char *split);
int cd(char **split, char ceedee[2][FILENAME_MAX]);

#endif

// ush.h
#ifndef USH_H
#define USH_H


// === Uncomment to switch to Debug mode
#define _DEBUG_
// ===


// Include headers from standart libraries
#define _GNU_SOURCE 1
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/errno.h>

// Include local headers
#include "vector.h"
#include "libmx.h"
#include "definitions.h"
#include "enums.h"
#include "structs.h"
#include "error.h"
#include "functions.h"
#include "debug.h"

#endif // !USH_H

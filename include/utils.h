#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC(x, s)                                                           \
    x = malloc(s);                                                             \
    if (x == NULL) {                                                           \
        fprintf(stderr, "%s", strerror(errno));                                \
        exit(1);                                                               \
    }

#define REALLOC(x, s)                                                          \
    x = realloc(x, s);                                                         \
    if (x == NULL) {                                                           \
        fprintf(stderr, "%s", strerror(errno));                                \
        exit(1);                                                               \
    }
#endif

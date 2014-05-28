#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ASSERT

#define ASSERT(x) \
    do { \
        if (!(x)) { \
            printf("%s(%d)\n", __FILE__, __LINE__); \
            puts(#x); \
            exit(-1); \
        } \
    } while(0); \

#endif

/* osapi.h */
#pragma once
#include <stdio.h>
#include "os.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define getposixfd(x) fds[(x)]
#define assert_initialized() \
    if (!initialized)        \
    reterr(ErrInit)

#define alloc(x) malloc(x)
#define destroy(x) free(x)

private
bool isopen(fd);

private
void setupfds();

internal void zero(int8 *, int16);

internal int8 *strnum(int8 *, int8);
internal void copy(int8 *, int8 *, int16);
private
int16 stringlen(int8 *);

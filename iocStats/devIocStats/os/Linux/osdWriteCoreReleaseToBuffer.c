#define _GNU_SOURCE 
#include <stdio.h>
#include "epicsStdio.h"
#include "epicsRelease.h"

int writeCoreReleaseToBuffer(char* buffer, unsigned int size)
{
    FILE *oldOut = epicsGetThreadStdin();
    FILE *file = fmemopen(buffer, size, "w");
    if (!file) return -1;
    epicsSetThreadStdout(file);
    coreRelease();
    epicsSetThreadStdout(oldOut);
    fclose(file);
    return 0;
}

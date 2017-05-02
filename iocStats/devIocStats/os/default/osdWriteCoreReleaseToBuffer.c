#define _POSIX_SOURCE
#include <unistd.h>
#include <stdio.h>
#include "epicsStdio.h"
#include "epicsRelease.h"

#ifdef WIN32
#define fdopen _fdopen
#endif

int writeCoreReleaseToBuffer(char* buffer, unsigned int size)
{
    int fd[2];
    int s;

    if (pipe(fd) < 0) return -1;
    FILE *oldOut = epicsGetThreadStdin();
    FILE *out = fdopen(fd[1], "w");
    epicsSetThreadStdout(out);
    coreRelease();
    epicsSetThreadStdout(oldOut);
    fclose(out);
    size--;
    while (size)
    {
        s = read(fd[0], buffer, size);
        if (s <= 0) break;
        buffer += s;
        size -= s;
    }
    *buffer = 0;
    close(fd[0]);
    return 0;
}

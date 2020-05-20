#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#define fdopen _fdopen
#define pipe(FDS) _pipe(FDS,1024,_O_BINARY)
#else
#define _POSIX_SOURCE
#include <unistd.h>
#endif
#include <stdio.h>
#include "epicsStdio.h"
#include "epicsRelease.h"


int writeCoreReleaseToBuffer(char* buffer, unsigned int size)
{
    int fd[2];
    int s;

    if (pipe(fd) < 0) return -1;
    FILE *oldOut = epicsGetThreadStdout();
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

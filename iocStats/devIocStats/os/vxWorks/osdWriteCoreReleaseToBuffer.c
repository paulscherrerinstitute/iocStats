#include <ioLib.h>
#include <stdio.h>
#include <symLib.h>
#include <sysSymTbl.h>
#include <epicsVersion.h>
#ifdef BASE_VERSION
#include "dbDefs.h"
#else
#include "epicsRelease.h"
#endif

int writeCoreReleaseToBuffer(char* buffer, unsigned int size)
{
    SYM_TYPE type;
    int oldOut, file;
    FUNCPTR memDevCreate, memDevDelete, memDrv;
    FUNCPTR pipeDevCreate, pipeDevDelete, pipeDrv;

    oldOut = ioTaskStdGet(0, 1);
    /* Do we have memDrv ? */
    if (symFindByName(sysSymTbl, "memDrv",       (char**)&memDrv, &type) == 0 &&
        symFindByName(sysSymTbl, "memDevCreate", (char**)&memDevCreate, &type) == 0 &&
        symFindByName(sysSymTbl, "memDevDelete", (char**)&memDevDelete, &type) == 0)
    {
        if (memDrv() != 0) goto pipe;
        if (memDevCreate("/epicsVersion", buffer, size) != 0) goto pipe;
        file = open("/epicsVersion", O_RDWR, 0777);
        if (file < 0)
        {
            memDevDelete("/epicsVersion");
            goto pipe;
        }
        ioTaskStdSet(0, 1, file);
        coreRelease();
        ioTaskStdSet(0, 1, oldOut);
        close(file);
        memDevDelete("/epicsVersion");
        return 0;
    }
pipe:
    /* Do we have pipeDrv ? */
    if (symFindByName(sysSymTbl, "pipeDrv",       (char**)&pipeDrv, &type) == 0 &&
        symFindByName(sysSymTbl, "pipeDevCreate", (char**)&pipeDevCreate, &type) == 0 &&
        symFindByName(sysSymTbl, "pipeDevDelete", (char**)&pipeDevDelete, &type) == 0)
    {
        int n, s;

        if (pipeDrv() != 0) goto sockets;
        if (pipeDevCreate("/epicsVersion", 10, 100) != 0) goto sockets;
        file = open("/epicsVersion", O_RDWR, 0777);
        if (file < 0)
        {
            pipeDevDelete("/epicsVersion", 1);
            goto sockets;
        }
        ioTaskStdSet(0, 1, file);
        coreRelease();
        ioTaskStdSet(0, 1, oldOut);
        ioctl(file, FIONMSGS, (int)&n);
        size--;
        while (size && n--)
        {
            s = read(file, buffer, size);
            buffer += s;
            size -= s;
        }
        *buffer = 0;
        close(file);
        pipeDevDelete("/epicsVersion", 1);
        return 0;
    }
sockets:
    /* Let's use sockets ? */
    return -1;
}

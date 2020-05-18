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

    oldOut = ioTaskStdGet(0, 1);
    /* Do we have memDrv ? */
    if (symFindByName(sysSymTbl, "memDrv",       (char**)&memDrv, &type) == 0 &&
        symFindByName(sysSymTbl, "memDevCreate", (char**)&memDevCreate, &type) == 0 &&
        symFindByName(sysSymTbl, "memDevDelete", (char**)&memDevDelete, &type) == 0)
    {
        if (memDrv() != 0) goto memDrvFail;
        if (memDevCreate("/epicsVersion", buffer, size) != 0) goto memDrvFail;
        file = open("/epicsVersion", O_RDWR, 0777);
        if (file < 0)
        {
            memDevDelete("/epicsVersion");
            goto memDrvFail;
        }
        ioTaskStdSet(0, 1, file);
        coreRelease();
        ioTaskStdSet(0, 1, oldOut);
        close(file);
        memDevDelete("/epicsVersion");
        return 0;
    }
memDrvFail:
    printf("writeCoreReleaseToBuffer: no method to write to buffer\n");
    return -1;
}

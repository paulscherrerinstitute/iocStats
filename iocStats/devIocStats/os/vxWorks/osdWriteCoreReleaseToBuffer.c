#include <ioLib.h>
#include <symLib.h>
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
    FUNCPTR memDevCreate, memDevDelete;
    FUNCPTR pipeDevCreate, pipeDevDelete;
    
    oldOut = ioGlobalStdGet(1);
    /* Do we have memDrv ? */
    if (symFindByName(sysSymTbl, "memDevCreate", (char**)&memDevCreate, &type) == 0 &&
        symFindByName(sysSymTbl, "memDevDelete", (char**)&memDevDelete, &type) == 0)
    {
        if (memDevCreate("/epicsVersion", buffer, size) != 0) return -1;
        file = open("/epicsVersion", O_RDWR, 0777);
        if (file < 0) return -1;
        ioGlobalStdSet(1, file);
        coreRelease();
        ioGlobalStdSet(1, oldOut);
        close(file);
        memDevDelete("/epicsVersion");
        return 0;
    }
    /* Do we have pipeDrv ? */
    if (symFindByName(sysSymTbl, "pipeDevCreate", (char**)&pipeDevCreate, &type) == 0 &&
        symFindByName(sysSymTbl, "pipeDevDelete", (char**)&pipeDevDelete, &type) == 0)
    {
        int n, s;

        if (pipeDevCreate("/epicsVersion", 10, 100) != 0) return -1;
        file = open("/epicsVersion", O_RDWR, 0777);
        if (file < 0) return -1;
        ioGlobalStdSet(1, file);
        coreRelease();
        ioGlobalStdSet(1, oldOut);
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
    /* Let's use sockets */
    return -1;
}

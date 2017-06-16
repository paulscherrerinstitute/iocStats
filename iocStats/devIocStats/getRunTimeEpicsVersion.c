#include <string.h>
#include <stdlib.h>
#include "devIocStats.h"

const char* getRunTimeEpicsVersion()
{
    /* We cannot rely on epicsReleaseVersion because that is a macro,
       thus is gives the EPICS version this code has been compiled with,
       not the EPICS version at run time!
       
       The only run-time information is what coreRelease() prints to stdout.
    */

    static char* ver = NULL;
    if (!ver)
    {
        char *buffer;
        char *built;
        char *endv;
        char *endb;
        
        buffer = calloc(1,400);
        if (!buffer) return "<nomem>";
        if (writeCoreReleaseToBuffer(buffer, 400) != 0) return "<fail>";

        ver = strstr(buffer, "EPICS R");
        if (ver) ver += 6;
        else
        {      
            ver = strstr(buffer, "Version R");
            if (!ver) return "<unknown>";
            ver += 8;
        }
        endv = strpbrk(ver, " \t\n$");
        if (!endv) endv = ver + strlen(ver);

        built = strstr(buffer, " built ");
        if (built)
        {
            endb = strpbrk(built, "\n$");
            if (endb) *endb = 0;
            strcpy(endv, built);
        }
        else
            *endv = 0;
    }
    return ver;
}

/*************************************************************************\
* Copyright (c) 2009 Helmholtz-Zentrum Berlin fuer Materialien und Energie.
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/* osdSystemInfo.c - System info strings: posix implementation = use uname() */

/*
 *  Author: Ralph Lange (HZB/BESSY)
 *
 *  Modification History
 *  2009-05-20 Ralph Lange (HZB/BESSY)
 *     Restructured OSD parts
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <devIocStats.h>

static char *notavail = "<not available>";
static struct utsname uts;
static char* kernelversion;
static char* osversion;

int devIocStatsInitSystemInfo (void) {
    char line[160];
    FILE* file;

    uname(&uts);
    kernelversion = calloc(strlen(uts.sysname)+strlen(uts.release)+strlen(uts.machine)+3, 1);
    sprintf(kernelversion, "%s %s %s", uts.sysname, uts.release, uts.machine);

    /* os version is stored in different formats in differently named release files.
       candidates:
        - /etc/redhat-release
        - /etc/system-release
        - /etc//lsb-release
        - /etc//system-release-cpe
    */
    osversion = notavail;
    
    /* Try system-release or redhat-release (one-liner) */
    file = fopen("/etc/system-release", "r");
    if (!file)
    file = fopen("/etc/redhat-release", "r");
    if (file)
    {
        if (fgets(line, sizeof(line), file))
        {
            size_t n = strlen(line);
            if (n > 0 && line[n-1] == '\n')
                line[n-1] = 0;
            osversion = strdup(line);
        }
        fclose(file);
        return 0;
    }
    
    /* Try lsb-release or os-release (lines with keywords) */
    file = fopen("/etc/lsb-release", "r");
    if (!file)
    file = fopen("/etc/os-release", "r");
    if (file)
    {
        /* find longest name string */
        while (fgets(line, sizeof(line), file))
        {
            size_t i;
            if ((strncmp(line, "NAME=", 5) == 0 && (i=5)) ||
                (strncmp(line, "PRETTY_NAME=", 12) == 0 && (i=12)) ||
                (strncmp(line, "DISTRIB_DESCRIPTION=", 20) == 0 && (i=20)))
            {
                size_t n;
                
                n = strlen(line+i);
                if (line[i+n-1] == '\n')
                {
                    n--;
                    line[i+n] = 0;
                }
                if (line[i] == '"' && line[i+n-1] == '"')
                {
                    i++;
                    n -= 2;
                    line[i+n] = 0;
                }
                if (osversion == notavail || n > strlen(osversion))
                {
                    if (osversion != notavail)
                        free(osversion);
                    osversion = strdup(line+i);
                }
            }
        } 
        fclose(file);
        return 0;
    }

    /* Try system-release-cpe (colon separated fields) */
    file = fopen("/etc/system-release-cpe", "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            char* ci = line;
            char* co = line;
            unsigned int field=0;
            /* look for cpe*:/o: */
            if (strncmp(line, "cpe:/o:", 7) == 0)
            {
                /* get 3 fields separated by : and remove escapes */
                ci += 7;
                while (*ci)
                {
                    if (*ci == ':')
                    {
                        if (++field == 3) break;
                        *co++ = ' ';
                        continue;
                    }
                    if (*ci == '\\')
                    {
                        ci++;
                        if (!*ci) break;
                    }
                    *co++ = *ci++;
                }
                *co = 0;
            }
            if (co > line)
            {
                osversion = strdup(line);
            }
        }
        fclose(file);
        return 0;
    }
            
    return 0;
}

int devIocStatsGetBSPVersion (char **pval)
{
    *pval = notavail;
    return -1;
}

int devIocStatsGetKernelVersion (char **pval)
{
    *pval = kernelversion;
    return 0;
}

int devIocStatsGetOSVersion (char **pval)
{
    *pval = osversion;
    return 0;
}

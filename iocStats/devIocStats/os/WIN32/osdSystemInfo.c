/*************************************************************************\
 * Copyright (c) 2012 Paul-Scherrer Institut(PSI)
 * Copyright (c) 2009 Helmholtz-Zentrum Berlin fuer Materialien und Energie.
 * Copyright (c) 2002 The University of Chicago, as Operator of Argonne
 *     National Laboratory.
 * Copyright (c) 2002 The Regents of the University of California, as
 *     Operator of Los Alamos National Laboratory.
 * EPICS BASE Versions 3.13.7
 * and higher are distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
\*************************************************************************/

/* osdSystemInfo.c - System info strings: vxWorks implementation */

/*
 *  Author: Ralph Lange (HZB/BESSY)
 *
 *  Modification History
 *  2012-03-16 Helge Brands (PSI) 
 *  2009-05-20 Ralph Lange (HZB/BESSY)
 *     Restructured OSD parts
 *
 */

#include <windows.h>
#include <stdio.h>
#include <devIocStats.h>

#define MAX_VERSTRING_SIZE 128
static char versionstring[MAX_VERSTRING_SIZE] = "\0";

/* Read a registry value of REG_SZ type
 * Note: Caller should free the returned pointer
 */
LPBYTE readRegStringValue(HKEY hKey, LPCSTR lpName) {
    DWORD cbData;
    LPBYTE lpData = NULL;

    /* Get value size */
    if (RegQueryValueEx(hKey, lpName, NULL, NULL, NULL, &cbData) != ERROR_SUCCESS) {
        return NULL;
    }
    /* Alloc memory and read the value. */
    lpData = calloc(cbData + 1, 1);
    if (RegQueryValueEx(hKey, lpName, NULL, NULL, lpData, &cbData) != ERROR_SUCCESS) {
        free(lpData);
        return NULL;
    }
    /* Ensure it is null terminated */
    lpData[cbData] = 0;
    return lpData;
}

int devIocStatsInitSystemInfo(void) {
    HKEY hKey;
    LPBYTE lpData = NULL;
    int j = 0;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                "Software\\Microsoft\\Windows NT\\CurrentVersion",
                0,
                KEY_READ,
                &hKey) != ERROR_SUCCESS)
        return -1;

    /* Product name, e.g. "Windows 10 Enterprise 2016 LTSB", "Windows Server 2016 Standard" */
    lpData = readRegStringValue(hKey, "ProductName");
    if (lpData) {
        j = sprintf_s(versionstring, MAX_VERSTRING_SIZE, "%s", lpData);
        free(lpData);
    }

    /* More identifiable than build number for Windows 10, e.g. "1607", "2004" */
    lpData = readRegStringValue(hKey, "ReleaseId");
    if (lpData) {
        j = sprintf_s(versionstring + j, MAX_VERSTRING_SIZE - j, " %s", lpData);
        free(lpData);
    }

    RegCloseKey(hKey);
    return 0;
}

int devIocStatsGetBSPVersion(char **pval) {
    *pval = 0; //sysBspRev();
    return 0;
}

int devIocStatsGetKernelVersion(char **pval) {
    *pval = versionstring;

    return 0;
}

int devIocStatsGetOSVersion(char **pval) {
    *pval = versionstring;

    return 0;
}

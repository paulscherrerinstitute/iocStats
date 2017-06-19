/*************************************************************************\
* Copyright (c) 2009-2010 Helmholtz-Zentrum Berlin
*     fuer Materialien und Energie GmbH.
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/* devIocStats.h -  Device Support Include for IOC statistics - based on */
/* devVXStats.c - Device Support Routines for vxWorks statistics */
/*
 *	Author: Jim Kowalkowski
 *	Date:  2/1/96
 *
 */

/* Cluster info pool types */
#define DATA_POOL 0
#define SYS_POOL  1

/* devIocStats value types (different update rates) */
#define MEMORY_TYPE	0
#define LOAD_TYPE	1
#define FD_TYPE		2
#define CA_TYPE		3
#define STATIC_TYPE	4
#define TOTAL_TYPES	5

/* Names of environment variables (may be redefined in OSD include) */
#define STARTUP  "STARTUP"
#define ST_CMD   "ST_CMD"
#define ENGINEER "ENGINEER"
#define LOCATION "LOCATION"

#include "devIocStatsOSD.h"

typedef int clustInfo[CLUSTSIZES][4];

typedef struct {
    double numBytesTotal;
    double numBytesFree;
    double numBytesAlloc;
    double numBlocksFree;
    double numBlocksAlloc;
    double maxBlockSizeFree;
} memInfo;

typedef struct {
    int used;
    int max;
} fdInfo;

typedef struct {
    int ierrors;
    int oerrors;
} ifErrInfo;

typedef struct {
    long noOfCpus;
    double cpuLoad;
    double iocLoad;
} loadInfo;

typedef struct fsInfo {
    const char* path;
    double fsUsage;
    double fsFreeBytes;
} fsInfo;

/* Functions (API) for OSD layer */
/* All funcs return 0 (OK) / -1 (ERROR) */

/* CPU Load */
extern int devIocStatsInitCpuUsage (void);
extern int devIocStatsGetCpuUsage (loadInfo *pval);

/* IOC Load (CPU utilization by this IOC) */
extern int devIocStatsInitCpuUtilization (loadInfo *pval);
extern int devIocStatsGetCpuUtilization (loadInfo *pval);

/* FD Usage */
extern int devIocStatsInitFDUsage (void);
extern int devIocStatsGetFDUsage (fdInfo *pval);

/* File system Usage */
extern int devIocStatsGetFileSystemUsage(fsInfo *pval);

/* Memory Usage */
extern int devIocStatsInitMemUsage (void);
extern int devIocStatsGetMemUsage (memInfo *pval);

/* RAM Workspace Usage */
extern int devIocStatsInitWorkspaceUsage (void);
extern int devIocStatsGetWorkspaceUsage (memInfo *pval);

/* Suspended Tasks */
extern int devIocStatsInitSuspTasks (void);
extern int devIocStatsGetSuspTasks (int *pval);

/* Cluster Info */
extern int devIocStatsInitClusterInfo (void);
extern int devIocStatsGetClusterInfo (int pool, clustInfo *pval);
extern int devIocStatsGetClusterUsage (int pool, int *pval);

/* Network Interface Errors */
extern int devIocStatsInitIFErrors (void);
extern int devIocStatsGetIFErrors (ifErrInfo *pval);

/* Boot Info */
extern int devIocStatsInitBootInfo (void);
extern int devIocStatsGetBootLine (char **pval);
extern int devIocStatsGetStartupScript (char **pval);
extern int devIocStatsGetStartupScriptDefault (char **pval);

/* System Info */
extern int devIocStatsInitSystemInfo (void);
extern int devIocStatsGetBSPVersion (char **pval);
extern int devIocStatsGetKernelVersion (char **pval);
extern int devIocStatsGetOSVersion (char **pval);

/* Host Info */
extern int devIocStatsInitHostInfo (void);
extern int devIocStatsGetPwd (char **pval);
extern int devIocStatsGetHostname (char **pval);
extern int devIocStatsGetPID (double *proc_id);
extern int devIocStatsGetPPID (double *proc_id);

/* CA Links */
extern long ioccar(unsigned int *pcal, unsigned int *pcalnconn, unsigned int *pcaldconn);

/* EPICS Release */
/* (epicsReleaseVersion is a macro. Thus using it we get the EPICS version
 * this code was compiled with but not the run-time EPICS version.
 * Parse the output of coreRelease() instead.)
 */
const char* getRunTimeEpicsVersion();
extern int writeCoreReleaseToBuffer(char* buffer, unsigned int size);

/* ioccar.c */
/*****************************************************************
                          COPYRIGHT NOTIFICATION
*****************************************************************
 
(C)  COPYRIGHT 1991 Regents of the University of California,
and the University of Chicago Board of Governors.
 
This software was developed under a United States Government license
described on the COPYRIGHT_Combined file included as part
of this distribution.
**********************************************************************/
#include "dbStaticLib.h"
/*definitions needed because of old vs new database access*/
#undef DBR_SHORT
#undef DBR_PUT_ACKT
#undef DBR_PUT_ACKS
#undef VALID_DB_REQ
#undef INVALID_DB_REQ
/*end of conflicting definitions*/
#include "epicsVersion.h"
#ifdef BASE_VERSION
/* R3.13.x */
#include "semLib.h"
#endif
#include "cadef.h"
#include "dbCa.h"
#include "dbLock.h"
#include "dbCommon.h"
/*--------------------------------------------------------------*/
#ifndef BASE_VERSION
/* R3.14.x */
#include "epicsMutex.h"
#include "dbCaPvt.h"

#endif
/*--------------------------------------------------------------*/

epicsShareExtern struct dbBase *pdbbase;

long ioccar(unsigned int *pcal, unsigned int *pcalnconn, unsigned int *pcaldconn)
{
    DBENTRY             dbentry;
    long                status;
    dbCommon            *precord;
    dbRecordType        *pdbRecordType;
    dbFldDes            *pdbFldDes;
    DBLINK              *plink;
    unsigned int        ncalinks=0;
    unsigned int        nconnected=0;
    unsigned int        nDisconnect=0;
    caLink              *pca;
    int                 j;
    int                 locked=0;

    dbInitEntry(pdbbase,&dbentry);
    status = dbFirstRecordType(&dbentry);
    while(!status) {
        status = dbFirstRecord(&dbentry);
        while(!status) {
            pdbRecordType = dbentry.precordType;
            precord = dbentry.precnode->precord;
            for(j=0; j<pdbRecordType->no_links; j++) {
                pdbFldDes = pdbRecordType->papFldDes[pdbRecordType->link_ind[j]];
                plink = (DBLINK *)((char *)precord + pdbFldDes->offset);
                if (plink->type == CA_LINK) {
                    if (!locked) {
                        dbScanLock(precord);
                        locked = 1;
                        if (plink->type != CA_LINK) continue; /* may have changed meanwhile */
                    }
                    ncalinks++;
                    pca = (caLink *)plink->value.pv_link.pvt;
                    if (pca && pca->chid && ca_state(pca->chid) == cs_conn) {
                        nconnected++;
                        nDisconnect += pca->nDisconnect;
                    }
                }
            }
            if (locked) {
                dbScanUnlock(precord);
                locked = 0;
            }
            status = dbNextRecord(&dbentry);
        }
        status = dbNextRecordType(&dbentry);
    }
    *pcal    = ncalinks;
    *pcalnconn = (ncalinks - nconnected);
    *pcaldconn = nDisconnect;
    dbFinishEntry(&dbentry);
    return(0);
}

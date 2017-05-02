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
/*--------------------------------------------------------------*/
#ifndef BASE_VERSION
/* R3.14.x */
#include "epicsMutex.h"
#include "dbCaPvt.h"

#endif
/*--------------------------------------------------------------*/

extern struct dbBase *pdbbase;

long ioccar(unsigned int *pcal, unsigned int *pcalnconn, unsigned int *pcaldconn)
{
    DBENTRY		dbentry;
    long		status;
    char		*precord;
    dbRecordType	*pdbRecordType;
    dbFldDes		*pdbFldDes;
    DBLINK		*plink;
    unsigned int	ncalinks=0;
    unsigned int	nconnected=0;
    unsigned int	nDisconnect=0;
    caLink		*pca;
    int			j;

    dbInitEntry(pdbbase,&dbentry);
    status = dbFirstRecordType(&dbentry);
    while(!status) {
	status = dbFirstRecord(&dbentry);
	while(!status) {
	    pdbRecordType = dbentry.precordType;
	    precord = dbentry.precnode->precord;
	    for(j=0; j<pdbRecordType->no_links; j++) {
		pdbFldDes = pdbRecordType->papFldDes[pdbRecordType->link_ind[j]];
		plink = (DBLINK *)(precord + pdbFldDes->offset);
		if (plink->type == CA_LINK) {
	            dbLockSetGblLock(); 
	            if (plink->type == CA_LINK) {
		        ncalinks++;
		        pca = (caLink *)plink->value.pv_link.pvt;
		        if(pca && pca->chid && (ca_field_type(pca->chid) != TYPENOTCONN)) {
		            nconnected++;
		            nDisconnect += pca->nDisconnect;
		        }
	            }
	            dbLockSetGblUnlock();
                }
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

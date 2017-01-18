#include "rsrv.h"
#include "ellLib.h"

extern ELLLIST clientQ;

void casStatsFetch(unsigned *pChanCount, unsigned *pConnCount)
{
    int circuitCount = ellCount ( &clientQ );
    if ( circuitCount < 0 ) {
	    *pConnCount = 0;
    }
    else {
	    *pConnCount = (unsigned) circuitCount;
    }
    *pChanCount = 0 /*rsrvChannelCount*/;
}

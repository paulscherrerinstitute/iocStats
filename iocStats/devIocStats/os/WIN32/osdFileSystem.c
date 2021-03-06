/* File system statistics
 * Autor: Dirk Zimoch (PSI)
 */

#include <windows.h>

#include <devIocStats.h>

int devIocStatsGetFileSystemUsage(fsInfo *pval) {
    ULARGE_INTEGER availBytes, totalBytes, freeBytes;
    
    if (!GetDiskFreeSpaceEx(pval->path,
        &availBytes, &totalBytes, &freeBytes))
        return -1;

    pval->fsUsage = 100*(1-(double)availBytes.QuadPart/(double)totalBytes.QuadPart);

    pval->fsFreeBytes = (double)availBytes.QuadPart;
    return 0;
}

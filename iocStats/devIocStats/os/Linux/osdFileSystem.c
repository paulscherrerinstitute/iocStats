/* File system statistics
 * Autor: Dirk Zimoch (PSI)
 */
 
#define _ISOC99_SOURCE /* for fmin() */
#include <math.h>
#include <sys/statvfs.h>

#include <devIocStats.h>

int devIocStatsGetFileSystemUsage(fsInfo *pval) {
    struct statvfs stat;

    if (statvfs(pval->path, &stat) != 0)
        return -1;

    pval->fsUsage = fmin(
        (double)stat.f_bfree/(double)stat.f_blocks,
        (double)stat.f_ffree/(double)stat.f_files);

    pval->fsFreeBytes = (double)stat.f_bfree*(double)stat.f_bsize;
    return 0;
}

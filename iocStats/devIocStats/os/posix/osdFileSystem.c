/* File system statistics
 * Autor: Dirk Zimoch (PSI)
 */
 
#define _ISOC99_SOURCE /* for fmin() */
#include <math.h>
#include <sys/statvfs.h>
#include <stdio.h>

#include <devIocStats.h>

int devIocStatsGetFileSystemUsage(fsInfo *pval) {
    struct statvfs stat;

    if (statvfs(pval->path, &stat) != 0)
        return -1;
/*        
    printf("%s\n", pval->path);
    printf("block size           %lu\n", stat.f_bsize);
    printf("fragment size        %lu\n", stat.f_frsize);
    printf("total fragments      %lu\n", stat.f_blocks);
    printf("free fragments       %lu\n", stat.f_bfree);
    printf("available fragments  %lu\n", stat.f_bavail);
    printf("total inodes         %lu\n", stat.f_files);
    printf("free inodes          %lu\n", stat.f_ffree);
    printf("available inodes     %lu\n", stat.f_favail);
*/

    pval->fsUsage = 100*(1-fmin(
        (double)stat.f_bavail/(double)stat.f_blocks,
        (double)stat.f_favail/(double)stat.f_files));

    pval->fsFreeBytes = (double)stat.f_bavail*(double)stat.f_frsize;
    return 0;
}

/* File system statistics
 * Autor: Dirk Zimoch (PSI)
 */
 
#include <stat.h>
#include <stdio.h>

#include <devIocStats.h>

int devIocStatsGetFileSystemUsage(fsInfo *pval) {
    struct statfs stat;
    double bfree, ffree;

    if (statfs((char*)pval->path, &stat) != 0)
        return errno;

/*
    printf("\n%s\n", pval->path);
    printf("block size           %ld\n", stat.f_bsize);
    printf("total blocks         %ld\n", stat.f_blocks);
    printf("free blocks          %ld\n", stat.f_bfree);
    printf("available blocks     %ld\n", stat.f_bavail);
    printf("total inodes         %ld\n", stat.f_files);
    printf("free inodes          %ld\n", stat.f_ffree);
*/

    bfree = (double)stat.f_bavail/(double)stat.f_blocks;
    
    if (stat.f_files > 0)
    {
        ffree = (double)stat.f_ffree/(double)stat.f_files;
        if (bfree > ffree) bfree = ffree;
    }   
    pval->fsUsage = 100*(1-bfree);
    pval->fsFreeBytes = (double)stat.f_bavail*(double)stat.f_bsize;
    return 0;
}

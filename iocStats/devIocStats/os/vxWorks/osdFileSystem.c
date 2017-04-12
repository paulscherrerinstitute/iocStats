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
        return -1;

    printf("%s\n", pval->path);
    printf("block size           %lu\n", stat.f_bsize);
    printf("total fragments      %lu\n", stat.f_blocks);
    printf("free fragments       %lu\n", stat.f_bfree);
    printf("available fragments  %lu\n", stat.f_bavail);
    printf("total inodes         %lu\n", stat.f_files);
    printf("free inodes          %lu\n", stat.f_ffree);

    bfree = (double)stat.f_bavail/(double)stat.f_blocks;
    ffree = (double)stat.f_ffree/(double)stat.f_files;
    
    if (bfree > ffree) bfree = ffree;    
    pval->fsUsage = 100*(1-bfree);

    pval->fsFreeBytes = (double)stat.f_bavail*(double)stat.f_bsize;
    return 0;
}

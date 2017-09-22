runScript startup.cmd

dbLoadRecords "iocFsStats.template","IOCNAME=$(IOC),FSNAME=TMP,FSPATH=/tmp"
dbLoadRecords "iocFsStats.template","IOCNAME=$(IOC),FSNAME=VAR,FSPATH=/var/log"

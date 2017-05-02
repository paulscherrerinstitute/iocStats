runScript startup.cmd

dbLoadRecords "iocFsStats.template","IOCNAME=$(IOC),FSNAME=TMP,FSPATH=/tmp"

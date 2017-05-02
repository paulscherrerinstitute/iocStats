#########################
# iocStats
#########################

epicsEnvSet("STARTUP","$(PWD=)")
epicsEnvSet("ST_CMD","startup.script")

dbLoadTemplate("iocStats.subs", "IOC=$(IOC)")
dbLoadTemplate("iocScanMon.subs", "IOC=$(IOC)")
dbLoadTemplate("iocGenTime.subs", "IOC=$(IOC)")

# END of iocStats

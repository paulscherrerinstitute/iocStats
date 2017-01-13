#########################
# iocStats
#########################

epicsEnvSet("STARTUP","$(PWD=)")
epicsEnvSet("ST_CMD","startup.script")

dbLoadTemplate("$(iocStats_DB)/iocAdminSoft.substitutions", "IOC=$(IOC)")
dbLoadTemplate("$(iocStats_DB)/iocAdminScanMon.substitutions", "IOC=$(IOC)")

# END of iocStats

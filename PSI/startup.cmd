#########################
# iocStats
#########################

epicsEnvSet("ENGINEER","$(USER)")
epicsEnvSet("LOCATION","$(INSTBASE)/iocBoot/ioc/$(IOC)")
epicsEnvSet("STARTUP","$(PWD)")
epicsEnvSet("ST_CMD","startup.script")

dbLoadTemplate("$(iocStats_TEMPLATES)/iocAdminSoft.substitutions", "IOC=$(IOC)")
dbLoadTemplate("$(iocStats_TEMPLATES)/iocAdminScanMon.substitutions", "IOC=$(IOC)")

# END of iocStats
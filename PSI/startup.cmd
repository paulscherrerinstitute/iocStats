# IOC MON

epicsEnvSet("ENGINEER","$(USER)")
epicsEnvSet("LOCATION","$(INSTBASE)")
#softIOC only
epicsEnvSet("STARTUP","/ioc/$(IOC)")
epicsEnvSet("ST_CMD","iocsh startup.script")

dbLoadTemplate("$(iocStats_TEMPLATES)/iocAdminSoft.substitutions", "IOC=$(IOC)")
dbLoadTemplate("$(iocStats_TEMPLATES)/iocAdminScanMon.substitutions", "IOC=$(IOC)")
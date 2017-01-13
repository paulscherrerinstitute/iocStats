#########################
# iocStats
#########################

dbLoadTemplate("$(iocStats_DB)/iocAdminVxWorks.substitutions", "IOC=$(IOC)")
dbLoadTemplate("$(iocStats_DB)/iocAdminScanMon.substitutions", "IOC=$(IOC)")

# END of iocStats

include /ioc/tools/driver.makefile

MODULE=iocStats

BUILDCLASSES += Linux
EXCLUDE_VERSIONS = 3.13

############# OSD sources #############
# Base 3.14 does not correctly define POSIX=NO for mingw
ifeq (mingw,$(findstring mingw, $(T_A)))
  POSIX=NO
endif


SOURCES_Linux += iocStats/devIocStats/os/Linux/osdCpuUsage.c
SOURCES_Linux += iocStats/devIocStats/os/Linux/osdCpuUtilization.c
SOURCES_Linux += iocStats/devIocStats/os/Linux/osdFdUsage.c
SOURCES_Linux += iocStats/devIocStats/os/Linux/osdMemUsage.c
SOURCES_Linux += iocStats/devIocStats/os/posix/osdSystemInfo.c
SOURCES_Linux += iocStats/devIocStats/os/posix/osdHostInfo.c
SOURCES_Linux += iocStats/devIocStats/os/posix/osdPIDInfo.c
SOURCES_Linux += iocStats/devIocStats/os/default/osdWorkspaceUsage.c
SOURCES_Linux += iocStats/devIocStats/os/default/osdClustInfo.c
SOURCES_Linux += iocStats/devIocStats/os/default/osdSuspTasks.c
SOURCES_Linux += iocStats/devIocStats/os/default/osdIFErrors.c
SOURCES_Linux += iocStats/devIocStats/os/default/osdBootInfo.c
SOURCES_Linux += iocStats/devIocStats/os/Linux/osdFileSystem.c

SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdCpuUsage.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdCpuUtilization.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdFdUsage.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdMemUsage.c
SOURCES_vxWorks += iocStats/devIocStats/os/default/osdWorkspaceUsage.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdClustInfo.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdSuspTasks.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdIFErrors.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdBootInfo.c
SOURCES_vxWorks += iocStats/devIocStats/os/vxWorks/osdSystemInfo.c
SOURCES_vxWorks += iocStats/devIocStats/os/posix/osdHostInfo.c
SOURCES_vxWorks += iocStats/devIocStats/os/default/osdPIDInfo.c
SOURCES_vxWorks += iocStats/devIocStats/os/default/osdFileSystem.c
SOURCES_3.13    += PSI/rsrv.c

#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdCpuUsage.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdCpuUtilization.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdFdUsage.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdMemUsage.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdWorkspaceUsage.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdClustInfo.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdSuspTasks.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdIFErrors.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdBootInfo.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdSystemInfo.c
#SOURCES_WIN32 += iocStats/devIocStats/os/WIN32/osdHostInfo.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdPIDInfo.c
#SOURCES_WIN32 += iocStats/devIocStats/os/default/osdFileSystem.c

############# OSI sources #############
SOURCES += iocStats/devIocStats/devIocStatsAnalog.c
SOURCES += iocStats/devIocStats/devIocStatsString.c
SOURCES += iocStats/devIocStats/devIocStatsWaveform.c
SOURCES += iocStats/devIocStats/devIocStatsSub.c
SOURCES += iocStats/devIocStats/devIocStatsTest.c

# CA link statistics
USR_INCLUDES+=-I$(EPICS_BASE)/src/db
SOURCES += iocStats/devIocStats/ioccar.c

# For DTYP="IOC stats"
DBDS += iocStats/devIocStats/devIocStats.dbd

############ TEMPLATES #########
TEMPLATES += iocStats/iocAdmin/Db/iocCluster.template
TEMPLATES += iocStats/iocAdmin/Db/iocRTEMSOnly.template
TEMPLATES += iocStats/iocAdmin/Db/iocScanMon.template
TEMPLATES += iocStats/iocAdmin/Db/iocEnvVar.template
TEMPLATES += iocStats/iocAdmin/Db/iocRTOS.template
TEMPLATES += iocStats/iocAdmin/Db/ioc.template
TEMPLATES += iocStats/iocAdmin/Db/iocGeneralTime.template
TEMPLATES += iocStats/iocAdmin/Db/iocScanMonSum.template
TEMPLATES += iocStats/iocAdmin/Db/iocVxWorksOnly.template
TEMPLATES += iocStats/iocAdmin/Db/iocFsStats.template

TEMPLATES += iocStats/iocAdmin/Db/iocAdminScanMon.substitutions
TEMPLATES += iocStats/iocAdmin/Db/iocAdminSoft.substitutions
TEMPLATES += iocStats/iocAdmin/Db/iocAdminRTEMS.substitutions
TEMPLATES += iocStats/iocAdmin/Db/iocAdminVxWorks.substitutions

############ SCRIPTS #########
SCRIPTS += $(wildcard PSI/*.cmd)
SCRIPTS += PSI/cfg/iocStats.archtmp

############ QT screens ##########
QT += $(wildcard PSI/qt/*)

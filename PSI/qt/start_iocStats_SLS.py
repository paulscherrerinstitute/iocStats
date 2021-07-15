#!/usr/bin/env python

import os
import signal
import subprocess
import re

proc_cmd = 'zenity --progress --pulsate --title "IOC status" --text "Collecting SLS IOC information\nfrom database..." <<<1 &'

# The os.setsid() is passed in the argument preexec_fn so
# it's run after the fork() and before  exec() to run the shell.
pro = subprocess.Popen(proc_cmd, stdout=subprocess.PIPE, 
                       shell=True, preexec_fn=os.setsid) 

os.system("ulimit -n 4096")
#stream = os.popen("ioc boots -f SLS|awk '{ print $1 }'")
#alternative with web service
#curl 'http://iocinfo.psi.ch/api/v2/records?pattern=.*%3AUPTIME&facility=sls' | jq -r '.[].ioc' | uniq
stream = os.popen("ioc records --facility sls '.*:UPTIME$' | awk -F '  +' '{print $4}' | uniq")
output = stream.read()
iocs = output.split("\n")

IOCs={
'AL_IOCS': list(),
'TI_IOCS': list(),
'VA_IOCS': list(),
'MA_IOCS': list(),
'RF_IOCS': list(),
'BPM_IOCS': list(),
'DI_IOCS': list(),
'ID_IOCS': list(),
'BEAM_IOCS': list(),
'OT_IOCS': list()
}

for ioc in iocs:
    if re.search('TEST', ioc) or re.search('^[A,X]', ioc)==None:
        continue
    if re.search('^ARIAL', ioc):
        IOCs["AL_IOCS"].append(ioc)
    elif re.search('^ACORF', ioc):
        IOCs["TI_IOCS"].append(ioc.split(':')[0])
    elif re.search('^A.*-VCS', ioc) or re.search('^A.*VA-', ioc):
        IOCs["VA_IOCS"].append(ioc)
    elif re.search('^ALIMA', ioc) or re.search('^ALBMA', ioc) or re.search('^ABOMA', ioc) or re.search('^ABRMA', ioc) or re.search('^ARIMA', ioc):
        IOCs["MA_IOCS"].append(ioc)
    elif re.search('^ALIRF', ioc) or re.search('^ABORF', ioc) or re.search('^ABTRF', ioc) or re.search('^ATSRF', ioc) or re.search('^ARIRF', ioc):
        IOCs["RF_IOCS"].append(ioc)
    elif re.search('^A.*DI.*-BPM', ioc) or re.search('^A.*DI.*-C[0-9]', ioc) or re.search('^A.*DI.*-TU', ioc):
        IOCs["BPM_IOCS"].append(ioc)
    elif re.search('^A.*DI.*', ioc):
        IOCs["DI_IOCS"].append(ioc)
    elif re.search('^X.*-.*ID', ioc):
        IOCs["ID_IOCS"].append(ioc)
    elif re.search('X0[1-9].*-.*', ioc) or re.search('X1[0-2].*-.*', ioc):
        IOCs["BEAM_IOCS"].append(ioc)
    else:
        IOCs["OT_IOCS"].append(ioc)
            
macro_param = ""
for i in IOCs:
    macro_param += i+"="
    for j in IOCs[i]:
            macro_param += "IOC="+j+";"
    macro_param += ","
cmd="caqtdm -noMsg -macro \""+macro_param+"\" iocStats_SLS_overview.ui"

#killing the info window
os.killpg(os.getpgid(pro.pid), signal.SIGTERM)  # Send the signal to all the process groups

#print(cmd)
#start caqtdm panel
os.popen(cmd)

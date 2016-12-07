# iocStats module

This module conssists of vanilla **iocStats** [sources](https://github.com/epics-modules/iocStats) and PSI specific files and adjustments. PSI specifics are located in [PSI](./PSI) folder and vanilla sources in [iocStats](./iocStats) folder.

## Usage of the iocStats module
To use iocStats module on PSI, module must be requested in statup script.
```
require iocStats
```

## Updating vanilla sources from upstream reporsitory
Vanilla sources were included as git subtree (which gives us a copy of sources and not only a link as in case of submodules) in folder [iocStats](./iocStats).

To include vanilla updates into this project following must be done:

 1. This reporsitory must be cloned to your machine.
 ```
 git clone https://git.psi.ch/cosylab/iocStats.git
 ```
  
 2. Remote reporsitory to the vanilla sources must be added:
 ```
 git remote add iocStats_remote https://github.com/epics-modules/iocStats.git
 ```
 
 3. Updates from the vanilla source can then be acquired using pull with subtree strategy (-s):
 ```
 git pull -s subtree iocStats_remote master
 ```

 4. Updates must then be pushed to this project.
 ```
 git push origin master
 ```
 
#!/bin/bash

usage()
{
    echo "Usage: $0 [options]"
  	echo
    echo "Create archiver configuration (S_C_iocStats_all.config) for all existing SwissFEL IOCs."
    echo "existing SwissFEL IOCs."
    echo
    echo "Options:"
    echo "    -h                   This help"
}

while getopts ":h" o; do
    case "${o}" in
        h)
            usage
            exit 0
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done



rm -rf .tmp
mkdir .tmp

findrecord -l --limit 0 -f swissfel -r 'IOC_CPU_LOAD$'| cut -d ":" -f 1 | xargs -I {} echo '!iocStats, "IOC={}"' >> ./.tmp/iocStats_all.archtmp
archiver_configurator -f -v INFO -o ./S_C_iocStats_all.config ./.tmp/iocStats_all.archtmp

rm -rf .tmp

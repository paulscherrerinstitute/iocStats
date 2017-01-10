#!/bin/bash

usage()
{
    echo "Usage: $0 [options]"
  	echo
    echo "Create iocStats archiver configuration for all existing IOCs on defined facility."
    echo "existing SwissFEL IOCs."
    echo
    echo "Options:"
    echo "    -h                                           This help"
    echo "    -f FACILITY (default: SWISSFEL)              Use IOCs from defined facility"
    echo "    -o OUTPUT (default: ./S_C_iocStats.config)   Output file"

}

FACILITY=SWISSFEL
OUTPUT=./S_C_iocStats.config

while getopts ":f:o:h" o; do
    case "${o}" in
        f)
            FACILITY=${OPTARG}
            ;;
	o)
            OUTPUT=${OPTARG}
            ;;
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

findrecord -l --limit 0 -f $FACILITY -r 'IOC_CPU_LOAD$'| cut -d ":" -f 1 | xargs -I {} echo '!iocStats, "IOC={}"' >> ./.tmp/iocStats_all.archtmp
archiver_configurator -f -v INFO -o $OUTPUT ./.tmp/iocStats_all.archtmp

rm -rf .tmp

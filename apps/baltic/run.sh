#!/bin/bash
source ./../modules.sh
source ./../var_glob.sh

export datstamp=$(date +%Y%m%d_%H%M%S)
exec 1>$METROMS_CASEDIR/run/$ROMS_APPLICATION/run_log.${datstamp} 2>&1

cd $METROMS_BASEDIR/apps/$ROMS_APPLICATION
python3 baltic.py

#!/bin/bash
source ./modules.sh
source ./var_glob_cice.sh

##################################################

module list -t >& modules-$(date --iso=sec)
env >& env-$(date --iso=sec)
##################################################

##################CICE############################
set -x

cd ${METROMS_CASEDIR}
tar -zxf ${METROMS_BASEDIR}/static_libs/${CICEVERSION}.tar.gz
cd ${METROMS_CASEDIR}/cice
tar -zxf ${METROMS_BASEDIR}/static_libs/${ICEPACKVERSION}.tar.gz

# Copy modified source files
export CICE_DIR=${METROMS_CASEDIR}/run/$ROMS_APPLICATION/cice
cp -a ${METROMS_BASEDIR}/apps/common/modified_src/cice_stda/* ${METROMS_CASEDIR}/cice

CICE_SCRIPTS=${METROMS_CASEDIR}/cice/configuration/scripts

cd ${CICE_SCRIPTS}/machines
mv Macros.machine_intel Macros.${MYHOST}_intel
mv env.machine_intel env.${MYHOST}_intel
sed -i "s@ICE_MACHINE_MACHNAME@& $MYHOST@" "env.${MYHOST}_intel"
sed -i "s@ICE_MACHINE_WKDIR@& $CICE_DIR/rundir@" "env.${MYHOST}_intel"
sed -i "s@ICE_MACHINE_INPUTDATA@& $METROMS_FORCING@" "env.${MYHOST}_intel"
cd ..
sed -i "s/myhost/${MYHOST}/g" cice.batch.csh
sed -i "s/myhost/${MYHOST}/g" cice.launch.csh
sed -i "s/mycase/${ROMS_APPLICATION}/g" cice_decomp.csh
sed -i "s/mygridnx/${mygridnx}/g" cice_decomp.csh
sed -i "s/mygridny/${mygridny}/g" cice_decomp.csh
sed -i "s/mygridny/${mygridny}/g" cice_decomp.csh
cd ${CICE_SCRIPTS}/options
mv set_nml.mycase set_nml.${ROMS_APPLICATION}
sed -i "s#<cicedir>#$CICE_DIR#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<cicegriddir>#$CICE_grid_dir#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<ciceatm>#$METROMS_FORCING#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<ciceocn>#$OCN_FORCING#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<icecpu>#$((CICEXCPU*CICEYCPU))#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<cicexgrid>#$mygridnx#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<ciceygrid>#$mygridny#g" set_nml.${ROMS_APPLICATION}
sed -i "s#CICEGRD#$CICEGRD#g" set_nml.${ROMS_APPLICATION}
sed -i "s#CICEKMT#$CICEKMT#g" set_nml.${ROMS_APPLICATION}
sed -i "s#CICEBAT#$CICEBAT#g" set_nml.${ROMS_APPLICATION}

((a=mygridnx/CICEXCPU)); ((rem1=mygridnx % CICEXCPU)); ((b=a+1))
if [ $rem1 -eq 0 ]; then ((BLCKX=a)); else ((BLCKX=b)); fi

((a=mygridny/CICEYCPU)); ((rem1=mygridny % CICEYCPU)); ((b=a+1))
if [ $rem1 -eq 0 ]; then ((BLCKY=a)); else ((BLCKY=b)); fi

sed -i "s#<blckx>#$BLCKX#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<blcky>#$BLCKY#g" set_nml.${ROMS_APPLICATION}

cd ${METROMS_CASEDIR}/cice
./cice.setup -c ${CICE_DIR} -m ${MYHOST} -g ${ROMS_APPLICATION} -p $((CICEXCPU*CICEYCPU))x1
cd $CICE_DIR
./cice.build

cd $CICE_DIR
./setup_run_dirs.csh
set +x
##################################################

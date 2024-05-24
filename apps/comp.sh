#!/bin/bash
source ./modules.sh
source ./var_glob.sh

##################################################

module list -t >& modules-$(date --iso=sec)
env >& env-$(date --iso=sec)

#####################MCT##########################
set -x

# Build MCT
cd ${METROMS_CASEDIR}
# Unpack standard source files
tar -zxf ${METROMS_BASEDIR}/static_libs/mct-${MCTVERSION}.tar.gz
export MCT_DIR=${METROMS_CASEDIR}/MCT
cd $MCT_DIR


./configure FC=$FORT CC=$CC MPIFC=$MPIFC --prefix=$MCT_DIR
make install
make clean

export MCT_INCDIR=${METROMS_CASEDIR}/MCT/include
export MCT_LIBDIR=${METROMS_CASEDIR}/MCT/lib

set +x
##################################################

##################CICE############################
set -x

cd ${METROMS_CASEDIR}
tar -zxf ${METROMS_BASEDIR}/static_libs/${CICEVERSION}.tar.gz
cd ${METROMS_CASEDIR}/cice
tar -zxf ${METROMS_BASEDIR}/static_libs/${ICEPACKVERSION}.tar.gz

# Copy modified source files
export CICE_DIR=${METROMS_CASEDIR}/run/$ROMS_APPLICATION/cice
cp -a ${METROMS_BASEDIR}/apps/common/modified_src/$CICEVERSION/* ${METROMS_CASEDIR}/cice

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
sed -i "s#<icecpu>#$((CICEXCPU*CICEYCPU))#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<cicexgrid>#$mygridnx#g" set_nml.${ROMS_APPLICATION}
sed -i "s#<ciceygrid>#$mygridny#g" set_nml.${ROMS_APPLICATION}

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

if [ ! -f $CICE_DIR/rundir/cice ]; then
    echo "$CICE_DIR/rundir/cice not found"
  echo "Error with compilation "
    exit -1
fi

# Build a library (for use in the ROMS build)
cd $CICE_DIR/rundir/compile
ar rcv libcice.a *.o

rm -f $CICE_DIR/rundir/cice

cd $CICE_DIR
./setup_run_dirs.csh
set +x
##################################################

#######################ROMS#######################
set -x
cd ${METROMS_CASEDIR}

tar -zxf ${METROMS_BASEDIR}/static_libs/${roms_ver}.tar.gz
export MY_ROMS_SRC=${METROMS_CASEDIR}/roms_src

export COMPILERS=${MY_ROMS_SRC}/Compilers
export NestedGrids=1
export MY_PROJECT_DIR=${METROMS_BASEDIR}/apps/${ROMS_APPLICATION}
export SCRATCH_DIR=${METROMS_CASEDIR}/build

cd ${MY_ROMS_SRC}
cp -a ${METROMS_BASEDIR}/apps/common/modified_src/$roms_ver/* .

export USE_MCT=on
export MY_CPP_FLAGS="${MY_CPP_FLAGS} -DNO_LBC_ATT -DMODEL_COUPLING -DUSE_MCT -DMCT_COUPLING -DMCT_LIB -DCICE_COUPLING -DCICE_OCEAN"

CICE_INCDIR := ${METROMS_CASEDIR}/run/${ROMS_APPLICATION}/cice/rundir/compile
CICE_LIBDIR := ${METROMS_CASEDIR}/run/${ROMS_APPLICATION}/cice/rundir/compile
FFLAGS += -I${CICE_INCDIR}
LIBS += -L${CICE_LIBDIR} -lcice

#export USE_DAP=on
#export PATH=/usr/bin:$PATH
export MY_HEADER_DIR=${MY_PROJECT_DIR}/include
export MY_ANALYTICAL_DIR=${MY_HEADER_DIR}
export BINDIR=${METROMS_CASEDIR}/run/${ROMS_APPLICATION}

cd ${MY_ROMS_SRC}
make clean
make -j $ROMS_comp_nproc

cp ${MY_ROMS_SRC}/ROMS/External/coupling.dat $BINDIR/
set +x

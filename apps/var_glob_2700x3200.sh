#######################################
export METROMS_BASEDIR=/users/work/mmuzyka/new_metroms_work_backup
export METROMS_CASEDIR=/users/work/mmuzyka/CSDIR/new_metro_2700x3200_01
#######################################

export PYTHONPATH=${PYTHONPATH}:$METROMS_BASEDIR/apps/python/

export MCTVERSION=2.11
export ICEPACKVERSION=icepack1.2.5
export CICEVERSION=cice6.2.0
export roms_ver="roms-1087"

export FORT=ifort
export CC=icc
export CXX=icc

export ROMS_APPLICATION=baltic
export MYHOST=tryton

export METROMS_FORCING=/users/work/mmuzyka/CSDIR/input_2700x3200
export CICE_grid_dir=/users/work/mmuzyka/CSDIR/input_2700x3200
export mygridnx=2700
export mygridny=3200

#ROMS compilation
export USE_MPI=on
export USE_MPIF90=on
export USE_OpenMP=
export USE_LARGE=on
export USE_DEBUG=on
export USE_NETCDF4=on
export USE_CICE=on
export ROMS_comp_nproc=24

#PROCESSORS DECOMPOSITION
export CICEXCPU=8
export CICEYCPU=9
export ROMSXCPU=12
export ROMSYCPU=10

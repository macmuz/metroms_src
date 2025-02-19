#######################################
export METROMS_BASEDIR=/home/plgrid/plgmacmuz/metroms_src
export METROMS_CASEDIR=/scratch/lustre/plgmacmuz/CSDIR/metro_POP_wind
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

export METROMS_FORCING=/scratch/lustre/plgmacmuz/ROMS/UERRA
export CICE_grid_dir=/scratch/lustre/plgmacmuz/ROMS/input_560x600
export mygridnx=560
export mygridny=600

#ROMS compilation
export USE_MPI=on
export USE_MPIF90=on
export USE_OpenMP=
export USE_LARGE=on
export USE_DEBUG=
export USE_NETCDF4=on
export USE_CICE=on
export ROMS_comp_nproc=24

#PROCESSORS DECOMPOSITION
export CICEXCPU=4
export CICEYCPU=6
export ROMSXCPU=8
export ROMSYCPU=12
#export CICEXCPU=8
#export CICEYCPU=6
#export ROMSXCPU=15
#export ROMSYCPU=16

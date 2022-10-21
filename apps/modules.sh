if [ -f /etc/bashrc ]; then
    . /etc/bashrc
fi

ulimit -l unlimited
ulimit -s unlimited
export KMP_STACKSIZE=274877906944

export HOME=/users/kdm/jakacki

module purge
module load tryton/netcdf/4.7.4-intel19-mpi-iopan

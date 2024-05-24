if [ -f /etc/bashrc ]; then
    . /etc/bashrc
fi

ulimit -l unlimited
ulimit -s unlimited
export KMP_STACKSIZE=274877906944

export HOME=/users/kdm/mmuzyka

module purge
module load trytonp/netcdf/4.9.2-intel24-mpi

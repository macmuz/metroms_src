#!/bin/bash

#suffix='2700x3200'
suffix=1370x1460
myCase=gls
#myCase=lmd

ARG=${1:-$myCase}_${2:-$suffix}

ln -sf var_glob_${ARG}.sh var_glob.sh

ln -sf baltic_${ARG}.py baltic/baltic.py

ln -sf roms_keyword_${ARG}.in common/roms_in/roms_keyword.in

ln -sf Params_${ARG}.py python/Params.py

ln -sf baltic_${ARG}.h baltic/include/baltic.h

cp baltic/run_slurm_template.sh baltic/run_slurm_${ARG}.sh
sed -i "s/ARG1/${1:-$myCase}/" baltic/run_slurm_${ARG}.sh
sed -i "s/ARG2/${2:-$suffix}/" baltic/run_slurm_${ARG}.sh

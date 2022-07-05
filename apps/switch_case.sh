#!/bin/bash
#suffix='2700x3200'
suffix='560x600'


rm var_glob.sh
ln -s var_glob_$suffix.sh var_glob.sh

rm baltic/baltic.py
ln -s baltic_$suffix.py baltic/baltic.py

rm baltic/run_slurm.sh
ln -s run_slurm_$suffix.sh baltic/run_slurm.sh

rm common/roms_in/roms_keyword.in
ln -s roms_keyword_$suffix.in common/roms_in/roms_keyword.in

rm python/Params.py
ln -s Params_$suffix.py python/Params.py

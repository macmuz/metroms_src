#!/bin/bash

ARG=${1}_${2}

ln -sf var_glob_${ARG}.sh ../var_glob.sh

ln -sf baltic_${ARG}.py baltic.py

ln -sf roms_keyword_${ARG}.in ../common/roms_in/roms_keyword.in

ln -sf Params_${ARG}.py ../python/Params.py

ln -sf baltic_${ARG}.h include/baltic.h

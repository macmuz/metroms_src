#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh var_glob_uerraGLSlog3slow_560x600.sh 1370x1460
./run.sh

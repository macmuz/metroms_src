#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition test
#SBATCH -N 8
#SBATCH --time=00:15:00

./switch_case_run.sh clean3 560x600
./run.sh

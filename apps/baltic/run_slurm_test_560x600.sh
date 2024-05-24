#!/bin/bash

#SBATCH -J test
#SBATCH --partition test
#SBATCH -N 8
#SBATCH --time=00:15:00

./switch_case_run.sh test 560x600
./run.sh

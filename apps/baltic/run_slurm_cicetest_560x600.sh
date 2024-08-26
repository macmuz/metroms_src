#!/bin/bash

#SBATCH -J cicetest
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=30:00:00

./switch_case_run.sh cicetest 560x600
./run.sh

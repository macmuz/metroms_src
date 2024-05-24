#!/bin/bash

#SBATCH -J grid30
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh oldgrid30 560x600
./run.sh

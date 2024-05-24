#!/bin/bash

#SBATCH -J grid50v3
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh oldgrid50v3 560x600
./run.sh

#!/bin/bash

#SBATCH -J grid50v2
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh oldgrid50v2 560x600
./run.sh

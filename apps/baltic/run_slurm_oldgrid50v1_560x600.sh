#!/bin/bash

#SBATCH -J 50v1
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh oldgrid50v1 560x600
./run.sh

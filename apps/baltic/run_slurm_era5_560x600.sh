#!/bin/bash

#SBATCH -J 560x600
#SBATCH --partition batch
#SBATCH -N 12
#SBATCH --time=72:00:00

./switch_case_run.sh era5 560x600
./run.sh

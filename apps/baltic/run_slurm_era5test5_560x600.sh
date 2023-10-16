#!/bin/bash

#SBATCH -J test5
#SBATCH --partition batch
#SBATCH -N 5
#SBATCH --time=72:00:00

./switch_case_run.sh era5test5 560x600
./run.sh

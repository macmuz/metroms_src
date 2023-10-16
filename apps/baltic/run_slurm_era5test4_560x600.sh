#!/bin/bash

#SBATCH -J test4
#SBATCH --partition batch
#SBATCH -N 12
#SBATCH --time=72:00:00

./switch_case_run.sh era5test4 560x600
./run.sh

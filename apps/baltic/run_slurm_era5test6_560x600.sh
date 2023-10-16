#!/bin/bash

#SBATCH -J test6
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=60:00:00

./switch_case_run.sh era5test6 560x600
./run.sh

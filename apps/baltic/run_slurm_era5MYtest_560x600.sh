#!/bin/bash

#SBATCH -J era5MYtest
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh era5MYtest 560x600
./run.sh

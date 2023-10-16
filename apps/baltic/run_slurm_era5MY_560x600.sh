#!/bin/bash

#SBATCH -J era5MY
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh era5MY 560x600
./run.sh

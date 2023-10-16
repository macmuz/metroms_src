#!/bin/bash

#SBATCH -J 05NM
#SBATCH --partition batch
#SBATCH -N 12
#SBATCH --time=72:00:00

./switch_case_run.sh era5 1370x1460
./run.sh

#!/bin/bash

#SBATCH -J 05test
#SBATCH --partition test
#SBATCH -N 12
#SBATCH --time=00:15:00

./switch_case_run.sh era5test1 1370x1460
./run.sh

#!/bin/bash

#SBATCH -J ICM
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh clean2014ICM 560x600
./run.sh

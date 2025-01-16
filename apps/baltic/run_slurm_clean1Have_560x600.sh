#!/bin/bash

#SBATCH -J clean1Have
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh clean1Have 560x600
./run.sh

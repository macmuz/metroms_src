#!/bin/bash

#SBATCH -J smooth
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh smooth 560x600
./run.sh

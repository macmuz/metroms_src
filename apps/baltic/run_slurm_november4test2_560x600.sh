#!/bin/bash

#SBATCH -J r4test2
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh november4test2 560x600
./run.sh

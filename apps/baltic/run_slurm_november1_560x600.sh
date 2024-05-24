#!/bin/bash

#SBATCH -J november1
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh november1 560x600
./run.sh

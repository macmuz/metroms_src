#!/bin/bash

#SBATCH -J 50lvls
#SBATCH --partition batch
#SBATCH -N 5
#SBATCH --time=72:00:00

./switch_case_run.sh 50lvls 560x600
./run.sh

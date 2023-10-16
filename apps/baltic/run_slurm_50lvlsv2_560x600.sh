#!/bin/bash

#SBATCH -J v2_50lvls
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh 50lvlsv2 560x600
./run.sh

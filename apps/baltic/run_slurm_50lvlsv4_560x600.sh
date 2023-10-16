#!/bin/bash

#SBATCH -J v4_50lvls
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh 50lvlsv4 560x600
./run.sh

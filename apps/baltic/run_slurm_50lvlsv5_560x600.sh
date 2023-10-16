#!/bin/bash

#SBATCH -J v5_25
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh 50lvlsv5 560x600
./run.sh

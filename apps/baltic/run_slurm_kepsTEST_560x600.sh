#!/bin/bash

#SBATCH -J kepsTEST
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh kepsTEST 560x600
./run.sh

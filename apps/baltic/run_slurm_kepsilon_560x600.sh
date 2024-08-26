#!/bin/bash

#SBATCH -J kepsilon
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh kepsilon 560x600
./run.sh

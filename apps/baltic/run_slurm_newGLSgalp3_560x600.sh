#!/bin/bash

#SBATCH -J galp3
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh newGLSgalp3 560x600
./run.sh

#!/bin/bash

#SBATCH -J galp6
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh newGLSgalp6 560x600
./run.sh

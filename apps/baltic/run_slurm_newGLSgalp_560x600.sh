#!/bin/bash

#SBATCH -J galp
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=60:00:00

./switch_case_run.sh newGLSgalp 560x600
./run.sh

#!/bin/bash

#SBATCH -J galp5
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh newGLSgalp5 560x600
./run.sh

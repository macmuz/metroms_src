#!/bin/bash

#SBATCH -J oct2
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLSoct2 560x600
./run.sh

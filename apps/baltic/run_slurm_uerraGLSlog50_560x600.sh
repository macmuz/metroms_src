#!/bin/bash

#SBATCH -J log50
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLSlog50 560x600
./run.sh

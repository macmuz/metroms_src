#!/bin/bash

#SBATCH -J log3slow
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLSlog3slow 560x600
./run.sh

#!/bin/bash

#SBATCH -J LMDwind
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=40:00:00

./switch_case_run.sh newLMDwind 560x600
./run.sh

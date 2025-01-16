#!/bin/bash

#SBATCH -J clean2014zeta
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=60:00:00

./switch_case_run.sh clean2014zeta 560x600
./run.sh

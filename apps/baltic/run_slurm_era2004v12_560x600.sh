#!/bin/bash

#SBATCH -J 2004v12
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=50:00:00

./switch_case_run.sh era2004v12 560x600
./run.sh

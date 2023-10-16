#!/bin/bash

#SBATCH -J 2004v4
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=60:00:00

./switch_case_run.sh era2004v4 560x600
./run.sh

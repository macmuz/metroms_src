#!/bin/bash

#SBATCH -J era2004v13
#SBATCH --partition batch_16h
#SBATCH -N 8
#SBATCH --time=12:00:00

./switch_case_run.sh era2004v13 560x600
./run.sh

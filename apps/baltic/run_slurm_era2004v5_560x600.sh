#!/bin/bash

#SBATCH -J 2004v5
#SBATCH --partition test 
#SBATCH -N 8
#SBATCH --time=00:15:00

./switch_case_run.sh era2004v5 560x600
./run.sh

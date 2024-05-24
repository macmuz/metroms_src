#!/bin/bash

#SBATCH -J MYlog2
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraMYlog2 560x600
./run.sh

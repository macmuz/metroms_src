#!/bin/bash

#SBATCH -J uerraGLS3
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLS3 560x600
./run.sh

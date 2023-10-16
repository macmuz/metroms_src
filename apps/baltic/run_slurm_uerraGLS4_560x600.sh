#!/bin/bash

#SBATCH -J uerraGLS4
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLS4 560x600
./run.sh

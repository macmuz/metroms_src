#!/bin/bash

#SBATCH -J mber4MY
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh november4MY 560x600
./run.sh

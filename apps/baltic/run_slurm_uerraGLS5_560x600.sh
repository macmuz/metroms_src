#!/bin/bash

#SBATCH -J uerraGLS5
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraGLS5 560x600
./run.sh

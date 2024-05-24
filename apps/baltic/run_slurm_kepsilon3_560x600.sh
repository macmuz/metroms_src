#!/bin/bash

#SBATCH -J keps3
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh kepsilon3 560x600
./run.sh

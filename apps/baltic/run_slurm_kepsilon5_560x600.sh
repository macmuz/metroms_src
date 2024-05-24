#!/bin/bash

#SBATCH -J keps5
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=30:00:00

./switch_case_run.sh kepsilon5 560x600
./run.sh

#!/bin/bash

#SBATCH -J keps5clima
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh keps5clima 560x600
./run.sh

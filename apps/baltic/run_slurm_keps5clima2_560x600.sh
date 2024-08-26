#!/bin/bash

#SBATCH -J keps5clima2
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=60:00:00

./switch_case_run.sh keps5clima2 560x600
./run.sh

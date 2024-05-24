#!/bin/bash

#SBATCH -J newera5MY
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh newera5MYtest 560x600
./run.sh

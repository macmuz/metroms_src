#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh era5MYtest5 560x600
./run.sh

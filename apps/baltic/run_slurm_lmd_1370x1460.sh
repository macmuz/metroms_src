#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition batch_16h
#SBATCH -N 12
#SBATCH --time=16:00:00

./switch_case_run.sh lmd 1370x1460
./run.sh

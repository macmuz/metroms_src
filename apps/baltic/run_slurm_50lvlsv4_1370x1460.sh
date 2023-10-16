#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition batch
#SBATCH -N 12
#SBATCH --time=72:00:00

./switch_case_run.sh 50lvlsv4 1370x1460
./run.sh

#!/bin/bash

#SBATCH -J metroms12N
#SBATCH --partition batch_16h
#SBATCH -N 12
#SBATCH --time=16:00:00

./run.sh

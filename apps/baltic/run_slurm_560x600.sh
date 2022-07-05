#!/bin/bash

#SBATCH -J metroms5N
#SBATCH --partition batch
#SBATCH -N 5 
#SBATCH --time=12:00:00

./run.sh

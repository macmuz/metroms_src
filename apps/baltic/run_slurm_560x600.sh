#!/bin/bash

#SBATCH -J metroms5N
#SBATCH --partition plgrid
#SBATCH -N 5 
#SBATCH --time=72:00:00

./run.sh

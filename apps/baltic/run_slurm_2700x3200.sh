#!/bin/bash

#SBATCH -J metroms8N
#SBATCH --partition batch
#SBATCH -N 8 
#SBATCH --time=24:00:00

./run.sh

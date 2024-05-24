#!/bin/bash

#SBATCH -J MYlog
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraMYlog 560x600
./run.sh

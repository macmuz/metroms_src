#!/bin/bash

#SBATCH -J LMDlog
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh uerraLMDlog 560x600
./run.sh

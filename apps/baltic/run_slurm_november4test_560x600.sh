#!/bin/bash

#SBATCH -J ber4test
#SBATCH --partition batch
#SBATCH -N 8
#SBATCH --time=72:00:00

./switch_case_run.sh november4test 560x600
./run.sh

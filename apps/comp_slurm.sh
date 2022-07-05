#!/bin/bash
#SBATCH -J metro_comp
#SBATCH --partition test
#SBATCH -N 1
#SBATCH --time=00:15:00

env -i comp.sh

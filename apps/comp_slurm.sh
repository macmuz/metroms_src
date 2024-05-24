#!/bin/bash
#SBATCH -J metro_comp
#SBATCH --partition test
#SBATCH -N 1
#SBATCH --time=00:15:00

sed -i "s@HOME=.*@HOME=${HOME}@" modules.sh
env -i ./comp.sh

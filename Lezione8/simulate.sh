#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/ex1 ]]
then
	rm -rf -v data/ex1
fi
mkdir -p -v data/ex1

echo -e "\n----------------------------------------------"
echo -e "ENERGY ESTIMATION!  MONTE CARLO ALGORITHM\n"
echo -e "Variational Monte Carlo code (via metropoli alghoritm) to compute the expectation value for the Hamiltonian for a single quantum particle in 1D.\n"
echo -e "----------------------------------------------\n"
echo -e "\nExercise 1"
echo -e "----------------------------------------------\n"

./main.exe

mv output_energy.dat data/ex1/output_energy.dat
mv output_position.dat data/ex1/output_position.dat

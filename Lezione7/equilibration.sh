#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/equilibration ]]
then
	rm -rf -v data/equilibration
fi

mkdir -p -v data/equilibration

echo -e "\n----------------------------------------------"
echo -e "EQUILIBRATION VALUTATION!  MONTE CARLO ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.fcc config.in
cp  input.solid input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/50000/g" input.in
sed -i "s/nstep_dummy/1/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
sed -i "s/delta_dummy/0.11/g" input.in
./NVE_NVT.exe
mv output_epot.dat data/equilibration/output_pot_solid.dat

echo -e "----------------------------------------------\n"
echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"

cp config.out config.in
cp  input.liquid input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/50000/g" input.in
sed -i "s/nstep_dummy/1/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
sed -i "s/delta_dummy/0.205/g" input.in
./NVE_NVT.exe
mv output_epot.dat data/equilibration/output_pot_liquid.dat

echo -e "----------------------------------------------\n"
echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"

cp config.out config.in
cp  input.gas input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/50000/g" input.in
sed -i "s/nstep_dummy/1/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
sed -i "s/delta_dummy/4/g" input.in
./NVE_NVT.exe
mv output_epot.dat data/equilibration/output_pot_gas.dat

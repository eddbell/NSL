#!/bin/bash
make

echo -e "<INFO> Checking if data directory exists; if yes, delete it..."
if [[ -d data ]]
then
	rm -rf ./data/outputeq*
else
	mkdir -p data
	echo -e "<INFO> data directory created"
fi

echo -e "\n----------------------------------------------"
echo -e "EQUILIBRATION VALUTATION!  MONTE CARLO ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"
echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.fcc config.in
cp  input.solid input.in
sed -i "s/nblk_dummy/10000/g" input.in
sed -i "s/nstep_dummy/1/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
./NVE_NVT.exe
mv output_* data/outputeq_temp_solid.dat

echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.liquid input.in
sed -i "s/nblk_dummy/20000/g" input.in
sed -i "s/nstep_dummy/1/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
./NVE_NVT.exe
mv output_* data/outputeq_temp_liquid.dat

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.gas input.in
sed -i "s/nblk_dummy/100/g" input.in
sed -i "s/nstep_dummy/2500/g" input.in
sed -i "s/eqtemp_dummy/0/g" input.in
./NVE_NVT.exe

mv output_* data/outputeq_temp_gas.dat

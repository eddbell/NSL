#!/bin/bash

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/NVE ]]
then
	rm -rf -v data/NVE
	echo -e "Cleaning ./data/NVE directory"

fi

mkdir -p data/NVE

echo -e "\nExercise 7.4 - Molecolar dinamics for NVE thermodinamic system\n"

echo -e "ESTIMATION OF RADIAL DISTRIBUTION FUNCTION g(r) - Metropolis ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "______________________________________________________________________\n"

rm -rf output_*

iNEVT=0
nblk=20
restart=0
delta=0.0005

echo -e "\nStime of  Potential energy and Pressure via MOLECULAR DYNAMIC ALGORITHM\n"

echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
nstep=1000
eqtemp=2000
temp=1.5414
cp ./Config/config.fcc ./Config/config.in
cp  ./Config/input.solid ./Config/input.in
sed -i "s/iNEVT_dummy/$iNEVT/g" ./Config/input.in
sed -i "s/nblk_dummy/$nblk/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nstep_dummy/$nstep/g" ./Config/input.in
sed -i "s/delta_dummy/$delta/g" ./Config/input.in
sed -i "s/eqtemp_dummy/$eqtemp/g" ./Config/input.in
sed -i "s/temp_dummy/$temp/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        ${iNEVT}        "
echo -e "Blocks =       $nblk           "
echo -e "Steps =        $nstep          "
echo -e "eqtemp =       $eqtemp         "
echo -e "delta =        $delta          "

./NVE_NVT.exe

mv -v output_g.dat data/NVE/output_g_solid_${iNEVT}.dat
mv -v output_g_err.dat data/NVE/output_g_err_solid_${iNEVT}.dat
mv -v output_epot.dat data/NVE/output_epot_solid_${iNEVT}.dat
mv -v output_pres.dat data/NVE/output_pres_solid_${iNEVT}.dat

echo -e "______________________________________________________________________\n"

echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
nstep=1000
eqtemp=5000
temp=1.888815
cp ./Config/config.out ./Config/config.in
cp  ./Config/input.liquid ./Config/input.in
sed -i "s/iNEVT_dummy/${iNEVT}/g" ./Config/input.in
sed -i "s/nblk_dummy/$nblk/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nstep_dummy/$nstep/g" ./Config/input.in
sed -i "s/delta_dummy/$delta/g" ./Config/input.in
sed -i "s/eqtemp_dummy/$eqtemp/g" ./Config/input.in
sed -i "s/temp_dummy/$temp/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        ${iNEVT}        "
echo -e "Blocks =       $nblk           "
echo -e "Steps =        $nstep          "
echo -e "eqtemp =       $eqtemp         "
echo -e "delta =        $delta          "

./NVE_NVT.exe

mv -v output_g.dat data/NVE/output_g_liquid_${iNEVT}.dat
mv -v output_g_err.dat data/NVE/output_g_err_liquid_${iNEVT}.dat
mv -v output_epot.dat data/NVE/output_epot_liquid_${iNEVT}.dat
mv -v output_pres.dat data/NVE/output_pres_liquid_${iNEVT}.dat

echo -e "______________________________________________________________________\n"

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
nstep=20000
eqtemp=40000
temp=0.955
cp ./Config/config.out ./Config/config.in
cp  ./Config/input.gas ./Config/input.in

sed -i "s/iNEVT_dummy/${iNEVT}/g" ./Config/input.in
sed -i "s/nblk_dummy/$nblk/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nstep_dummy/$nstep/g" ./Config/input.in
sed -i "s/delta_dummy/$delta/g" ./Config/input.in
sed -i "s/eqtemp_dummy/$eqtemp/g" ./Config/input.in
sed -i "s/temp_dummy/$temp/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        ${iNEVT}        "
echo -e "Blocks =       $nblk           "
echo -e "Steps =        $nstep          "
echo -e "eqtemp =       $eqtemp         "
echo -e "delta =        $delta          "

./NVE_NVT.exe

mv -v output_g.dat data/NVE/output_g_gas_${iNEVT}.dat
mv -v output_g_err.dat data/NVE/output_g_err_gas_${iNEVT}.dat
mv -v output_epot.dat data/NVE/output_epot_gas_${iNEVT}.dat
mv -v output_pres.dat data/NVE/output_pres_gas_${iNEVT}.dat
echo -e "______________________________________________________________________\n"

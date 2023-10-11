#!/bin/bash

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/NVT ]]
then
	rm -rf -v data/NVT
	echo -e "Cleaning ./data/NVT directory"

fi

mkdir -p data/NVT

echo -e "\nExercise 7.4 - Molecolar dinamics for NVT thermodinamic system\n"

echo -e "ESTIMATION OF RADIAL DISTRIBUTION FUNCTION g(r) - Metropolis ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "______________________________________________________________________\n"

rm -rf output_*

deltas=(0.0005 0.11)
deltal=(0.0005 0.205)
deltag=(0.0005 4)
a_eqtemp=(20000,0,0)

iNEVT=1
nblk=20
restart=0
nstep=2000
delta=0.0005
eqtemp=0


echo -e "\nStime of  Potential energy and Pressure via MOLECULAR DYNAMIC ALGORITHM\n"

echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
delta=0.11
temp=0.8
cp ./Config/config.fcc ./Config/config.in
cp  ./Config/input.solid ./Config/input.in
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

mv -v output_g.dat data/NVT/output_g_solid_${iNEVT}.dat
mv -v output_g_err.dat data/NVT/output_g_err_solid_${iNEVT}.dat
mv -v output_epot.dat data/NVT/output_epot_solid_${iNEVT}.dat
mv -v output_pres.dat data/NVT/output_pres_solid_${iNEVT}.dat

echo -e "______________________________________________________________________\n"

echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
delta=0.205
temp=1.1
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

mv -v output_g.dat data/NVT/output_g_liquid_${iNEVT}.dat
mv -v output_g_err.dat data/NVT/output_g_err_liquid_${iNEVT}.dat
mv -v output_epot.dat data/NVT/output_epot_liquid_${iNEVT}.dat
mv -v output_pres.dat data/NVT/output_pres_liquid_${iNEVT}.dat

echo -e "______________________________________________________________________\n"

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
delta=4
temp=1.2
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

mv -v output_g.dat data/NVT/output_g_gas_${iNEVT}.dat
mv -v output_g_err.dat data/NVT/output_g_err_gas_${iNEVT}.dat
mv -v output_epot.dat data/NVT/output_epot_gas_${iNEVT}.dat
mv -v output_pres.dat data/NVT/output_pres_gas_${iNEVT}.dat
echo -e "______________________________________________________________________\n"

#!/bin/bash

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/equilibration ]]
then
	rm -rf data/equilibration
	echo -e "Cleaning ./data/equilibration directory"
fi

mkdir -p data/equilibration

echo -e "\nExercise 7.1/7.2 - Molecolar dinamics with Metropolis Algorithm\n"

echo -e "EQUILIBRATION - Metropolis ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "______________________________________________________________________\n"

echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
NBLK=50000
NSTEP=1
iNEVT=1
restart=0
cp ./Config/config.fcc ./Config/config.in
cp ./Config/input.solid ./Config/input.in
sed -i "s/iNEVT_dummy/$iNEVT/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nblk_dummy/$NBLK/g" ./Config/input.in
sed -i "s/nstep_dummy/$NSTEP/g" ./Config/input.in
sed -i "s/eqtemp_dummy/0/g" ./Config/input.in
sed -i "s/delta_dummy/0.11/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        $iNEVT            (FIXED)"
echo -e "Restart =       $restart         (FIXED)"
echo -e "Blocks =       $NBLK         (FIXED)"
echo -e "Steps =        $NSTEP            (FIXED)"
echo -e "eqtemp =       0            (FIXED)"
echo -e "delta =        0.11 "


./NVE_NVT.exe
mv ./output_epot.dat data/equilibration/output_pot_solid.dat

echo "<INFO> Saving position output file in: ./data/equilibration/output_pot_solid.dat"
echo -e "______________________________________________________________________\n"


echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
restart=0
cp ./Config/config.out ./Config/config.in
cp  ./Config/input.liquid ./Config/input.in
sed -i "s/iNEVT_dummy/$iNEVT/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nblk_dummy/$NBLK/g" ./Config/input.in
sed -i "s/nstep_dummy/$NSTEP/g" ./Config/input.in
sed -i "s/eqtemp_dummy/0/g" ./Config/input.in
sed -i "s/delta_dummy/0.205/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        $iNEVT            (FIXED)"
echo -e "Restart =       $restart         (FIXED)"
echo -e "Blocks =       $NBLK         (FIXED)"
echo -e "Steps =        $NSTEP            (FIXED)"
echo -e "eqtemp =       0            (FIXED)"
echo -e "delta =        0.205 "

./NVE_NVT.exe
mv ./output_epot.dat data/equilibration/output_pot_liquid.dat

echo "<INFO> Saving position output file in: ./data/equilibration/output_pot_liquid.dat"
echo -e "______________________________________________________________________\n"

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"

cp ./Config/config.out ./Config/config.in
cp ./Config/input.gas ./Config/input.in
sed -i "s/iNEVT_dummy/$iNEVT/g" ./Config/input.in
sed -i "s/restart_dummy/$restart/g" ./Config/input.in
sed -i "s/nblk_dummy/$NBLK/g" ./Config/input.in
sed -i "s/nstep_dummy/$NSTEP/g" ./Config/input.in
sed -i "s/eqtemp_dummy/0/g" ./Config/input.in
sed -i "s/delta_dummy/4/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        $iNEVT            (FIXED)"
echo -e "Restart =       $restart         (FIXED)"
echo -e "Blocks =       $NBLK         (FIXED)"
echo -e "Steps =        $NSTEP            (FIXED)"
echo -e "eqtemp =       0            (FIXED)"
echo -e "delta =        4 "

./NVE_NVT.exe
mv ./output_epot.dat data/equilibration/output_pot_gas.dat

echo "<INFO> Saving position output file in: ./data/equilibration/output_pot_gas.dat"
echo -e "______________________________________________________________________\n"

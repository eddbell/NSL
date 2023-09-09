#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/ex3 ]]
then
	rm -rf -v data/ex3
	echo -e "Cleaning ./data/ex3 directory"
fi

mkdir -p data/ex3

echo -e "\nExercise 7.3 - Molecolar dinamics with Metropolis Algorithm\n"

echo -e "ESTIMATION OF THERMODINAMIC QUANTITIES - Metropolis ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "______________________________________________________________________\n"

echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
cp ./Config/config.fcc ./Config/config.in
cp  ./Config/input.solid ./Config/input.in
sed -i "s/iNEVT_dummy/1/g" ./Config/input.in
sed -i "s/nblk_dummy/20/g" ./Config/input.in
sed -i "s/nstep_dummy/2000/g" ./Config/input.in
sed -i "s/eqtemp_dummy/1000/g" ./Config/input.in
sed -i "s/delta_dummy/0.11/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        1               (FIXED)"
echo -e "Blocks =       20              (FIXED)"
echo -e "Steps =        2000            (FIXED)"
echo -e "eqtemp =       1000            (FIXED)"
echo -e "delta =        0.11            "

./NVE_NVT.exe


mv output_g.dat data/ex3/output_g_solid_1.dat
mv output_g_err.dat data/ex3/output_g_err_solid_1.dat
echo "<INFO> Saving position output file in: ./data/ex3/output_g_solid_1.dat.dat"
echo "<INFO> Saving position output file in: ./data/equilibration/output_g_err_solid_1.dat"
echo -e "______________________________________________________________________\n"

echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
cp ./Config/config.out ./Config/config.in
cp  ./Config/input.liquid ./Config/input.in
sed -i "s/iNEVT_dummy/1/g" ./Config/input.in
sed -i "s/nblk_dummy/20/g" ./Config/input.in
sed -i "s/nstep_dummy/2000/g" ./Config/input.in
sed -i "s/eqtemp_dummy/1000/g" ./Config/input.in
sed -i "s/delta_dummy/0.205/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        1               (FIXED)"
echo -e "Blocks =       20              (FIXED)"
echo -e "Steps =        2000            (FIXED)"
echo -e "eqtemp =       1000            (FIXED)"
echo -e "delta =        0.205            "
./NVE_NVT.exe

mv output_g.dat data/ex3/output_g_liquid_1.dat
mv output_g_err.dat data/ex3/output_g_err_liquid_1.dat

echo "<INFO> Saving position output file in: ./data/ex3/output_g_liquid_1.dat.dat"
echo "<INFO> Saving position output file in: ./data/equilibration/output_g_err_liquid_1.dat"
echo -e "______________________________________________________________________\n"

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
cp ./Config/config.out ./Config/config.in
cp  ./Config/input.gas ./Config/input.in
sed -i "s/iNEVT_dummy/1/g" ./Config/input.in
sed -i "s/nblk_dummy/20/g" ./Config/input.in
sed -i "s/nstep_dummy/2000/g" ./Config/input.in
sed -i "s/eqtemp_dummy/1000/g" ./Config/input.in
sed -i "s/delta_dummy/4/g" ./Config/input.in

echo -e "<PARAMETERS INFO>"
echo -e "iNEVT =        1               (FIXED)"
echo -e "Blocks =       20              (FIXED)"
echo -e "Steps =        2000            (FIXED)"
echo -e "eqtemp =       1000            (FIXED)"
echo -e "delta =        4            "
./NVE_NVT.exe

mv output_g.dat data/ex3/output_g_gas_1.dat
mv output_g_err.dat data/ex3/output_g_err_gas_1.dat
echo "<INFO> Saving position output file in: ./data/ex3/output_g_gas_1.dat.dat"
echo "<INFO> Saving position output file in: ./data/equilibration/output_g_err_gas_1.dat"
echo -e "______________________________________________________________________\n"
rm -rf output_*

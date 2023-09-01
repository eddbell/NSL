#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/ex3 ]]
then
	rm -rf -v data/ex3
fi

mkdir -p data/ex3

echo -e "\n----------------------------------------------"
echo -e "ESTIMATION OF THERMODINAMIC QUANTITIES!  MONTE CARLO ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"
echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.fcc config.in
cp  input.solid input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/20/g" input.in
sed -i "s/nstep_dummy/2000/g" input.in
sed -i "s/eqtemp_dummy/1000/g" input.in
sed -i "s/delta_dummy/0.11/g" input.in
./NVE_NVT.exe

mv output_g.dat data/ex3/output_g_solid_1.dat
mv output_g_err.dat data/ex3/output_g_err_solid_1.dat


echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.liquid input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/20/g" input.in
sed -i "s/nstep_dummy/2000/g" input.in
sed -i "s/eqtemp_dummy/1000/g" input.in
sed -i "s/delta_dummy/0.205/g" input.in
./NVE_NVT.exe

mv output_g.dat data/ex3/output_g_liquid_1.dat
mv output_g_err.dat data/ex3/output_g_err_liquid_1.dat

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.gas input.in
sed -i "s/iNEVT_dummy/1/g" input.in
sed -i "s/nblk_dummy/20/g" input.in
sed -i "s/nstep_dummy/2000/g" input.in
sed -i "s/eqtemp_dummy/1000/g" input.in
sed -i "s/delta_dummy/4/g" input.in

./NVE_NVT.exe

mv output_g.dat data/ex3/output_g_gas_1.dat
mv output_g_err.dat data/ex3/output_g_err_gas_1.dat

rm -rf output_*

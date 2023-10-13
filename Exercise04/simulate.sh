#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
	"Please equilibrate before : use the code ./equilibrate.sh"
else
	rm -rf ./data/output_*
fi

echo -e "\n----------------------------------------------"
echo -e "ESTIMATION OF THERMODINAMIC QUANTITIES!  MONTE CARLO ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"
echo -e "----------------------------------------------\n"
echo -e "\nSOLID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.fcc config.in
cp  input.solid input.in
sed -i "s/nblk_dummy/40/g" input.in
sed -i "s/nstep_dummy/2000/g" input.in
sed -i "s/eqtemp_dummy/2000/g" input.in
./NVE_NVT.exe

for Q in "etot" "epot" "temp" "ekin" "pres"
do
	mv output_${Q}.dat data/output_${Q}_solid.dat
done

echo -e "\nLIQUID SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.liquid input.in
sed -i "s/nblk_dummy/40/g" input.in
sed -i "s/nstep_dummy/2000/g" input.in
sed -i "s/eqtemp_dummy/5000/g" input.in
./NVE_NVT.exe

for Q in "etot" "epot" "temp" "ekin" "pres"
do
	mv output_${Q}.dat data/output_${Q}_liquid.dat
done

echo -e "\nGAS SIMULATION"
echo -e "----------------------------------------------\n"
cp config.out config.in
cp  input.gas input.in
sed -i "s/nblk_dummy/40/g" input.in
sed -i "s/nstep_dummy/20000/g" input.in
sed -i "s/eqtemp_dummy/40000/g" input.in
./NVE_NVT.exe

for Q in "etot" "epot" "temp" "ekin" "pres"
do
	mv output_${Q}.dat data/output_${Q}_gas.dat
done

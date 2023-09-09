#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/ex4 ]]
then
	rm -rf -v data/ex4
	echo -e "Cleaning ./data/ex4 directory"

fi

mkdir -p data/ex4

./Config/
echo -e "\nExercise 7.4 - Molecolar dinamics with Metropolis Algorithm\n"

echo -e "ESTIMATION OF RADIAL DISTRIBUTION FUNCTION g(r) - Metropolis ALGORITHM\n"
echo -e "Classic Lennard-Jones fluid MC simulation using Lennard-Jones units.\n"

echo -e "______________________________________________________________________\n"

rm -rf output_*
deltas=(0.0005 0.11)
deltal=(0.0005 0.205)
deltag=(0.0005 4)
for iNEVT in 0 1
do
  if ((${iNEVT} == 0))
    then
      echo -e "\nStime of  Potential energy and Pressure via MOLECULAR DYNAMIC ALGORITHM\n"
    else
      echo -e "\nStime of  Potential energy and Pressure via MONTE CARLO ALGORITHM\n"
  fi
  echo -e "----------------------------------------------\n"
  echo -e "\nSOLID SIMULATION"
  echo -e "----------------------------------------------\n"

  cp ./Config/config.fcc ./Config/config.in
  cp  ./Config/input.solid ./Config/input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" ./Config/input.in
  sed -i "s/nblk_dummy/20/g" ./Config/input.in
  sed -i "s/nstep_dummy/2000/g" ./Config/input.in
  sed -i "s/eqtemp_dummy/2000/g" ./Config/input.in
  sed -i "s/delta_dummy/${deltas[${iNEVT}]}/g" ./Config/input.in

	echo -e "<PARAMETERS INFO>"
	echo -e "iNEVT =        ${iNEVT}        "
	echo -e "Blocks =       20              (FIXED)"
	echo -e "Steps =        2000            (FIXED)"
	echo -e "eqtemp =       2000            (FIXED)"
	echo -e "delta =        ${deltas[${iNEVT}]}    "
  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_solid_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_solid_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_solid_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_solid_${iNEVT}.dat

	echo "<INFO> Saving position output file in: ./data/ex4/output_g_solid_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_g_err_solid_${iNEVT}.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_epot_solid_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_pres_solid_${iNEVT}.dat"
	echo -e "______________________________________________________________________\n"

  echo -e "\nLIQUID SIMULATION"
  echo -e "----------------------------------------------\n"
  cp ./Config/config.out ./Config/config.in
  cp  ./Config/input.liquid ./Config/input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" ./Config/input.in
  sed -i "s/nblk_dummy/20/g" ./Config/input.in
  sed -i "s/nstep_dummy/2000/g" ./Config/input.in
  sed -i "s/eqtemp_dummy/5000/g" ./Config/input.in
  sed -i "s/delta_dummy/${deltal[${iNEVT}]}/g" ./Config/input.in

	echo -e "<PARAMETERS INFO>"
	echo -e "iNEVT =        ${iNEVT}        "
	echo -e "Blocks =       20              (FIXED)"
	echo -e "Steps =        2000            (FIXED)"
	echo -e "eqtemp =       5000            (FIXED)"
	echo -e "delta =        ${deltal[${iNEVT}]}    "

  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_liquid_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_liquid_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_liquid_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_liquid_${iNEVT}.dat

	echo "<INFO> Saving position output file in: ./data/ex4/output_g_liquid_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_g_err_liquid_${iNEVT}.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_epot_liquid_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_pres_liquid_${iNEVT}.dat"
	echo -e "______________________________________________________________________\n"

  echo -e "\nGAS SIMULATION"
  echo -e "----------------------------------------------\n"
  cp ./Config/config.out ./Config/config.in
  cp  ./Config/input.gas ./Config/input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" ./Config/input.in
  sed -i "s/nblk_dummy/20/g" ./Config/input.in
  sed -i "s/nstep_dummy/10000/g" ./Config/input.in
  sed -i "s/eqtemp_dummy/1000/g" ./Config/input.in
  sed -i "s/delta_dummy/${deltag[${iNEVT}]}/g" ./Config/input.in

	echo -e "<PARAMETERS INFO>"
	echo -e "iNEVT =        ${iNEVT}        "
	echo -e "Blocks =       20              (FIXED)"
	echo -e "Steps =        10000            (FIXED)"
	echo -e "eqtemp =       1000            (FIXED)"
	echo -e "delta =        ${deltag[${iNEVT}]}    "

  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_gas_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_gas_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_gas_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_gas_${iNEVT}.dat

	echo "<INFO> Saving position output file in: ./data/ex4/output_g_gas_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_g_err_gas_${iNEVT}.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_epot_gas_${iNEVT}.dat.dat"
	echo "<INFO> Saving position output file in: ./data/ex4/output_pres_gas_${iNEVT}.dat"
	echo -e "______________________________________________________________________\n"
done

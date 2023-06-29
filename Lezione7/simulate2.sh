#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
fi

if [[ -d data/ex4 ]]
then
	rm -rf -v data/ex4
fi

mkdir -p data/ex4

echo -e "\n----------------------------------------------"
echo -e "Exercise 07.4:"
echo -e "Classic Lennard-Jones fluid MC/MD simulation using Lennard-Jones units.\n"

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

  cp config.fcc config.in
  cp  input.solid input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" input.in
  sed -i "s/nblk_dummy/20/g" input.in
  sed -i "s/nstep_dummy/2000/g" input.in
  sed -i "s/eqtemp_dummy/2000/g" input.in
  sed -i "s/delta_dummy/${deltas[${iNEVT}]}/g" input.in
  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_solid_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_solid_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_solid_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_solid_${iNEVT}.dat


  echo -e "\nLIQUID SIMULATION"
  echo -e "----------------------------------------------\n"
  cp config.out config.in
  cp  input.liquid input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" input.in
  sed -i "s/nblk_dummy/20/g" input.in
  sed -i "s/nstep_dummy/2000/g" input.in
  sed -i "s/eqtemp_dummy/5000/g" input.in
  sed -i "s/delta_dummy/${deltal[${iNEVT}]}/g" input.in
  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_liquid_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_liquid_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_liquid_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_liquid_${iNEVT}.dat

  echo -e "\nGAS SIMULATION"
  echo -e "----------------------------------------------\n"
  cp config.out config.in
  cp  input.gas input.in
  sed -i "s/iNEVT_dummy/${iNEVT}/g" input.in
  sed -i "s/nblk_dummy/20/g" input.in
  sed -i "s/nstep_dummy/10000/g" input.in
  sed -i "s/eqtemp_dummy/1000/g" input.in
  sed -i "s/delta_dummy/${deltag[${iNEVT}]}/g" input.in

  ./NVE_NVT.exe

  mv output_g.dat data/ex4/output_g_gas_${iNEVT}.dat
  mv output_g_err.dat data/ex4/output_g_err_gas_${iNEVT}.dat
  mv output_epot.dat data/ex4/output_epot_gas_${iNEVT}.dat
  mv output_pres.dat data/ex4/output_pres_gas_${iNEVT}.dat
done

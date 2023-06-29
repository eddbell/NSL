#!/bin/bash
rm -rf ../dati/dati5/*.dat
for iD in 0 1
do
	for iGS in 0 1 2
	do
		cp input.dummy input.in
		sed -i "s/iD_dummy/${iD}/g" input.in
		sed -i "s/iGS_dummy/${iGS}/g" input.in
		./metropolis.exe

		mv output_rad_* ../dati/dati5/output_rad_${iD}_${iGS}.dat
		mv output_pos_* ../dati/dati5/output_pos_${iD}_${iGS}.dat
	done
done
rm -rf output_*

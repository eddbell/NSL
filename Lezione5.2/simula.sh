#!/bin/bash
make

if [[ -d data ]]
then
	rm -rf -v data
fi
mkdir -p -v data

delta=(6 22 2 5)
count=0
for iD in 0 1
do
	for iGS in 0 1
	do
		cp input.dummy input.in
		sed -i "s/iD_dummy/${iD}/g" input.in
		sed -i "s/iGS_dummy/${iGS}/g" input.in
		sed -i "s/delta_dummy/${delta[${count}]}/g" input.in
		./main.exe

		mv output_rad.dat data/output_rad_${iD}_${iGS}.dat
		mv output_pos.dat data/output_pos_${iD}_${iGS}.dat
		((count++))
	done
done
rm -rf output_*

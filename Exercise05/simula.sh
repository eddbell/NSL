#!/bin/bash
make

if [[ -d data ]]
then
	rm -rf data
fi
mkdir -p data
echo -e "Cleaning ./data directory"

echo -e "\nExercise 5 - Metropolis algorithm\n"
echo -e "_________Equilibration analysis_________\n\n"


for pos in 1 5 20 50
do
	cp input.dummy input.in
	sed -i "s/iD_dummy/1/g" input.in
	sed -i "s/iGS_dummy/0/g" input.in
	sed -i "s/delta_dummy/0.8/g" input.in
	sed -i "s/x_dummy/${pos}/g" input.in
	sed -i "s/steps_dummy/1000/g" input.in
	sed -i "s/nblk_dummy/100/g" input.in

	echo -e "<PARAMETERS INFO>"
	echo -e "Steps =        100000          (FIXED)"
	echo -e "Blocks =       100             (FIXED)"
	echo -e "iD =           1               (FIXED)"
	echo -e "iGS =          0               (FIXED)"
	echo -e "delta =        0.8             (FIXED)"
	echo -e "position (x) = ${pos}"

	./main.exe

	mv output_rad.dat ./data/output_rad_varx_${pos}.dat
	mv output_pos.dat ./data/output_pos_varx_${pos}.dat
	echo "<INFO> Saving position output file in: ./data/output_rad_varx_${pos}.dat"
  echo "<INFO> Saving radius output file in:   ./data/output_pos_varx_${pos}.dat"
  echo -e "______________________________________________________________________\n\n"
done
rm -rf output_*

echo -e "_________Block statistics analysis_________\n\n"
for steps in 1000 10000 100000
do
	cp input.dummy input.in
	sed -i "s/iD_dummy/1/g" input.in
	sed -i "s/iGS_dummy/0/g" input.in
	sed -i "s/delta_dummy/0.8/g" input.in
	sed -i "s/x_dummy/1/g" input.in
	sed -i "s/steps_dummy/${steps}/g" input.in
	sed -i "s/nblk_dummy/100/g" input.in

	echo -e "<PARAMETERS INFO>"
	echo -e "Steps =        ${steps}"
	echo -e "Blocks =       100             (FIXED)"
	echo -e "iD =           1               (FIXED)"
	echo -e "iGS =          0               (FIXED)"
	echo -e "delta =        0.8             (FIXED)"
	echo -e "position (x) = 1               (FIXED)"

	./main.exe

	mv output_rad.dat ./data/output_rad_stat_${steps}.dat
	mv output_pos.dat ./data/output_pos_stat_${steps}.dat
	echo "<INFO> Saving position output file in: ./data/output_rad_stat_${steps}.dat"
  echo "<INFO> Saving radius output file in:   ./data/output_pos_stat_${steps}.dat"
  echo -e "______________________________________________________________________\n\n"
done
rm -rf output_*

delta=(2.5 6 0.8 1.8)
count=0
for iD in 0 1
do
	for iGS in 0 1
	do
		cp input.dummy input.in
		sed -i "s/iD_dummy/${iD}/g" input.in
		sed -i "s/iGS_dummy/${iGS}/g" input.in
		sed -i "s/x_dummy/1/g" input.in
		sed -i "s/delta_dummy/${delta[count]}/g" input.in
		sed -i "s/steps_dummy/100000/g" input.in
		sed -i "s/nblk_dummy/100000/g" input.in

		echo -e "<PARAMETERS INFO>"
		echo -e "Steps =        100000                  (FIXED)"
		echo -e "Blocks =       100000             (FIXED)"
		echo -e "iD =           ${iD}"
		echo -e "iGS =          ${iGS}"
		echo -e "delta =        ${delta[count]}"
		echo -e "position (x) = 1                  (FIXED)"

		./main.exe

		mv output_rad.dat data/output_rad_eq_${iD}_${iGS}.dat
		mv output_pos.dat data/output_pos_eq_${iD}_${iGS}.dat

		echo "<INFO> Saving position output file in: ./data/output_rad_eq_${iD}_${iGS}.dat"
		echo "<INFO> Saving radius output file in:   ./data/output_pos_eq_${iD}_${iGS}.dat"
		echo -e "______________________________________________________________________\n\n"
		((count++))
	done
done
rm -rf output_*

echo   "_________Radius and 3D distribution estimation_________"


delta=(2.5 6 0.8 1.8)
count=0
for iD in 0 1
do
	for iGS in 0 1
	do
		cp input.dummy input.in
		sed -i "s/iD_dummy/${iD}/g" input.in
		sed -i "s/iGS_dummy/${iGS}/g" input.in
		sed -i "s/x_dummy/1/g" input.in
		sed -i "s/delta_dummy/${delta[count]}/g" input.in
		sed -i "s/steps_dummy/100000/g" input.in
		sed -i "s/nblk_dummy/100/g" input.in

		echo -e "<PARAMETERS INFO>"
		echo -e "Steps =        100000          (FIXED)"
		echo -e "Blocks =       100             (FIXED)"
		echo -e "iD =           ${iD}"
		echo -e "iGS =          ${iGS}"
		echo -e "delta =        ${delta[count]}"
		echo -e "position (x) = 1               (FIXED)"

		./main.exe

		mv output_rad.dat data/output_rad_${iD}_${iGS}.dat
		mv output_pos.dat data/output_pos_${iD}_${iGS}.dat

		echo "<INFO> Saving position output file in: ./data/output_rad_${iD}_${iGS}.dat"
		echo "<INFO> Saving radius output file in:   ./data/output_pos_${iD}_${iGS}.dat"
		echo -e "______________________________________________________________________\n\n"
		((count++))
	done
done
rm -rf output_*

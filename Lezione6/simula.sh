#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
else
	rm -rf data/output_*
fi

name_algorithm=("METROPOLI" "GIBBS")
for m in 0 1
do
echo -e "\n----------------------------------------------"
echo -e "EXECUTION WITH ${name_algorithm[${m}]} ALGORITHM"
echo -e "----------------------------------------------\n"
	for h in 0 0.02
	do
		echo -e "\nWith external magnetic field parameter equal to ${h}.\n"
		for t in 0.50 0.65 0.80 0.95 1.10 1.25 1.40 1.55 1.70 1.85 2.00
		do
			cp input.dummy input.dat
			sed -i "s/temp_dummy/${t}/g" input.dat
			sed -i "s/h_dummy/${h}/g" input.dat
			sed -i "s/algorithm_dummy/${m}/g" input.dat
			sed -i "s/eq_dummy/0/g" input.dat
			sed -i "s/nblk_dummy/100/g" input.dat
			sed -i "s/nstep_dummy/10000/g" input.dat
			./Monte_Carlo_ISING_1D.exe
		done
		for name in "ene" "heat" "mag" "chi"
		do
			mv output_${name}_h_m.dat output_${name}_${h}_${m}.dat
		done
	done
done

mv output_* data/

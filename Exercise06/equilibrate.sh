#!/bin/bash
make

if [[ ! -d data ]]
then
	mkdir -p -v data
else
	rm -rf data/outputEq*
fi


name_algorithm=("METROPOLI" "GIBBS")
for m in 0 1
do
echo -e "\n----------------------------------------------"
echo -e "EQUILIBRATION VALUTATION!  ${name_algorithm[${m}]} ALGORITHM"
echo -e "----------------------------------------------\n"
	for h in 0 0.02
	do
		echo -e "\nWith external magnetic field parameter equal to ${h}.\n"
		for t in 1.00 1.50 2.00
		do
			cp input.dummy input.dat
			sed -i "s/temp_dummy/${t}/g" input.dat
			sed -i "s/h_dummy/${h}/g" input.dat
			sed -i "s/algorithm_dummy/${m}/g" input.dat
			sed -i "s/nblk_dummy/300/g" input.dat
			sed -i "s/nstep_dummy/1/g" input.dat
			sed -i "s/eq_dummy/1/g" input.dat
			./Monte_Carlo_ISING_1D.exe
			mv output_eq_ene_m_t.dat outputEq_ene_${m}_${t}.dat
		done
	done
done

mv outputEq_* data/

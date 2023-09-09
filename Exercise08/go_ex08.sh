#!/bin/bash
make

if [[ -d data ]]
then
	rm -rf data
fi
mkdir -p data
echo -e "Cleaning ./data directory"

echo -e "\nExercise 8.1 - Variational Monte Carlo method\n"
echo -e "_________________Equilibration_________________\n\n"

SA=1
nstp=100000
nblk=100
mu=1
sig=0.2
delta=5
delta_opt=1
temp=1
delta_temp=0.0001
x=0

cp input.dummy input.in
sed -i "s/SA_dummy/$SA/g" input.in
sed -i "s/steps_dummy/$nstp/g" input.in
sed -i "s/nblk_dummy/$nblk/g" input.in
sed -i "s/mu_dummy/$mu/g" input.in
sed -i "s/sig_dummy/$sig/g" input.in
sed -i "s/delta_dummy/$delta/g" input.in
sed -i "s/delta_opt_dummy/$delta_opt/g" input.in
sed -i "s/temp_dummy/$temp/g" input.in
sed -i "s/delta_t_dummy/$delta_temp/g" input.in
sed -i "s/x_dummy/$x/g" input.in


echo -e "<PARAMETERS INFO>"
echo -e "SA =           $SA"
echo -e "Steps =        $nstp"
echo -e "Blocks =       $nblk\n"
echo -e "Mu =           $mu"
echo -e "sig =          $sig"
echo -e "delta =        $delta"
echo -e "delta otp =    $delta_opt"
echo -e "temp =         $temp"
echo -e "delta temp =   $delta_temp"
echo -e "position (x) = $x"

./main.exe

mv -v output_H.dat ./data/output_H.dat
mv -v output_x_psi2.dat ./data/output_x_psi2.dat
mv -v out_opt.dat ./data/out_opt.dat

echo -e "___________________________________________________________\n\n"

rm -rf output_*

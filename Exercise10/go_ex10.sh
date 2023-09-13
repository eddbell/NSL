#!/bin/bash
make -f makefile || exit

if [[ -d data ]]
then
	rm -rf data
fi
mkdir -p data

echo -e "Cleaning ./data directory"

echo -e "\nExercise 10 - The Travel Salesman Problem\n"

NUM_CORES=6
NUM_CITIES=50
POPULATION_SIZE=300
MAX_GENERATIONS=300
M_RATE=0.1
C_RATE=0.8
CONVENIENT_EXPONENT=10
GENERATION_MIGRATION=30

cp input.dummy input.in

sed -i "s/NUM_CITIES_dummy/$NUM_CITIES/g" input.in
sed -i "s/POPULATION_SIZE_dummy/$POPULATION_SIZE/g" input.in
sed -i "s/MAX_GENERATIONS_dummy/$MAX_GENERATIONS/g" input.in
sed -i "s/M_RATE_dummy/$M_RATE/g" input.in
sed -i "s/C_RATE_dummy/$C_RATE/g" input.in
sed -i "s/CONVENIENT_EXPONENT_dummy/$CONVENIENT_EXPONENT/g" input.in
sed -i "s/GENERATION_MIGRATION_dummy/$GENERATION_MIGRATION/g" input.in


echo -e "<PARAMETERS INFO>"
echo -e "NUM_CITIES = $NUM_CITIES"
echo -e "NUM_CORES = $NUM_CORES"
echo -e "POPULATION_SIZE = $POPULATION_SIZE"
echo -e "MAX_GENERATIONS = $MAX_GENERATIONS"
echo -e "GENERATION_MIGRATION = $GENERATION_MIGRATION"
echo -e "M_RATE = $M_RATE"
echo -e "C_RATE = $C_RATE"
echo -e "CONVENIENT_EXPONENT = $CONVENIENT_EXPONENT"


/usr/bin/mpiexec -np $NUM_CORES ./go.exe

for((node=0; node<$NUM_CORES; node++)); do
mv -v out_order_N$node.dat ./data/out_order_N$node.dat
mv -v out_L_N$node.dat ./data/out_L_N$node.dat
mv -v out_L_Ave_N$node.dat ./data/out_L_Ave_N$node.dat
done

echo -e "___________________________________________________________\n\n"

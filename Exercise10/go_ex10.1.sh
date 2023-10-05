#!/bin/bash
make -f makefile || exit

if [[ -d data ]]
then
	rm -rf data
	echo -e "Cleaning ./data directory"
fi
mkdir -p data

echo -e "\nExercise 10 - The Travel Salesman Problem\n"
echo -e "___________________________________________________________\n\n"


BOOL_MIGRATION=1
NUM_CORES=5
NUM_CITIES=50
POPULATION_SIZE=300
MAX_GENERATIONS=300
M_RATE=0.2
C_RATE=0.5
R_RATE=0.001
CONVENIENT_EXPONENT=2
GENERATION_MIGRATION=4
echo -e "\nExercise 10.1 - using $NUM_CORES comunicating cores \n"
cp input.dummy input.in

sed -i "s/BOOL_MIGRATION_dummy/$BOOL_MIGRATION/g" input.in
sed -i "s/NUM_CITIES_dummy/$NUM_CITIES/g" input.in
sed -i "s/POPULATION_SIZE_dummy/$POPULATION_SIZE/g" input.in
sed -i "s/MAX_GENERATIONS_dummy/$MAX_GENERATIONS/g" input.in
sed -i "s/M_RATE_dummy/$M_RATE/g" input.in
sed -i "s/C_RATE_dummy/$C_RATE/g" input.in
sed -i "s/R_RATE_dummy/$R_RATE/g" input.in
sed -i "s/CONVENIENT_EXPONENT_dummy/$CONVENIENT_EXPONENT/g" input.in
sed -i "s/GENERATION_MIGRATION_dummy/$GENERATION_MIGRATION/g" input.in


echo -e "<PARAMETERS INFO>"

echo -e "BOOL_MIGRATION = $BOOL_MIGRATION"
echo -e "NUM_CITIES = $NUM_CITIES"
echo -e "NUM_CORES = $NUM_CORES"
echo -e "POPULATION_SIZE = $POPULATION_SIZE"
echo -e "MAX_GENERATIONS = $MAX_GENERATIONS"
echo -e "GENERATION_MIGRATION = $GENERATION_MIGRATION"
echo -e "M_RATE = $M_RATE"
echo -e "C_RATE = $C_RATE"
echo -e "R_RATE = $R_RATE"
echo -e "CONVENIENT_EXPONENT = $CONVENIENT_EXPONENT"


/usr/bin/mpiexec -np $NUM_CORES ./go.exe

for((node=0; node<$NUM_CORES; node++)); do
mv -v out_order_N$node.dat ./data/out_order_N$node.$BOOL_MIGRATION.dat
mv -v out_L_N$node.dat ./data/out_L_N$node.$BOOL_MIGRATION.dat
#mv -v out_L_Ave_N$node.dat ./data/out_L_Ave_N$node.$BOOL_MIGRATION.dat
done

echo -e "___________________________________________________________\n\n"

echo -e "\nExercise 10.2 - using $NUM_CORES NON comunicating cores \n"
BOOL_MIGRATION=0

cp input.dummy input.in

sed -i "s/BOOL_MIGRATION_dummy/$BOOL_MIGRATION/g" input.in
sed -i "s/NUM_CITIES_dummy/$NUM_CITIES/g" input.in
sed -i "s/POPULATION_SIZE_dummy/$POPULATION_SIZE/g" input.in
sed -i "s/MAX_GENERATIONS_dummy/$MAX_GENERATIONS/g" input.in
sed -i "s/M_RATE_dummy/$M_RATE/g" input.in
sed -i "s/C_RATE_dummy/$C_RATE/g" input.in
sed -i "s/R_RATE_dummy/$R_RATE/g" input.in
sed -i "s/CONVENIENT_EXPONENT_dummy/$CONVENIENT_EXPONENT/g" input.in
sed -i "s/GENERATION_MIGRATION_dummy/$GENERATION_MIGRATION/g" input.in


echo -e "<PARAMETERS INFO>"

echo -e "BOOL_MIGRATION = $BOOL_MIGRATION"
echo -e "NUM_CITIES = $NUM_CITIES"
echo -e "NUM_CORES = $NUM_CORES"
echo -e "POPULATION_SIZE = $POPULATION_SIZE"
echo -e "MAX_GENERATIONS = $MAX_GENERATIONS"
echo -e "GENERATION_MIGRATION = $GENERATION_MIGRATION"
echo -e "M_RATE = $M_RATE"
echo -e "C_RATE = $C_RATE"
echo -e "R_RATE = $R_RATE"
echo -e "CONVENIENT_EXPONENT = $CONVENIENT_EXPONENT"


/usr/bin/mpiexec -np $NUM_CORES ./go.exe

for((node=0; node<$NUM_CORES; node++)); do
mv -v out_order_N$node.dat ./data/out_order_N$node.$BOOL_MIGRATION.dat
mv -v out_L_N$node.dat ./data/out_L_N$node.$BOOL_MIGRATION.dat
#mv -v out_L_Ave_N$node.dat ./data/out_L_Ave_N$node.$BOOL_MIGRATION.dat
done

echo -e "___________________________________________________________\n\n"

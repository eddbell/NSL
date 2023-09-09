#!/bin/bash
make

if [[ -d data ]]
then
	rm -rf data
fi
mkdir -p data

echo -e "Cleaning ./data directory"

echo -e "\nExercise 9 - The Travel Salesman Problem\n"
echo -e "_________________Square map_________________\n\n"

MAP_GEOMETRY=0
NUM_CITIES=34
POPULATION_SIZE=1000
MAX_GENERATIONS=200
M_RATE=0.03
C_RATE=0.5
CONVENIENT_EXPONENT=4

cp input.dummy input.in

sed -i "s/MAP_GEOMETRY_dummy/$MAP_GEOMETRY/g" input.in
sed -i "s/NUM_CITIES_dummy/$NUM_CITIES/g" input.in
sed -i "s/POPULATION_SIZE_dummy/$POPULATION_SIZE/g" input.in
sed -i "s/MAX_GENERATIONS_dummy/$MAX_GENERATIONS/g" input.in
sed -i "s/M_RATE_dummy/$M_RATE/g" input.in
sed -i "s/C_RATE_dummy/$C_RATE/g" input.in
sed -i "s/CONVENIENT_EXPONENT_dummy/$CONVENIENT_EXPONENT/g" input.in


echo -e "<PARAMETERS INFO>"
echo -e "MAP_GEOMETRY = $MAP_GEOMETRY"
echo -e "NUM_CITIES = $NUM_CITIES"
echo -e "POPULATION_SIZE = $POPULATION_SIZE"
echo -e "MAX_GENERATIONS = $MAX_GENERATIONS"
echo -e "M_RATE = $M_RATE"
echo -e "C_RATE = $C_RATE"
echo -e "CONVENIENT_EXPONENT = $CONVENIENT_EXPONENT"

./go.exe

mv -v out_cities.dat ./data/out_cities_square.dat
mv -v out_order.dat ./data/out_order_square.dat
mv -v out_L.dat ./data/out_L_square.dat
mv -v out_L_Ave.dat ./data/out_L_Ave_square.dat

echo -e "___________________________________________________________\n\n"

echo -e "_________________Circle map_________________\n\n"

MAP_GEOMETRY=1
NUM_CITIES=34
POPULATION_SIZE=1000
MAX_GENERATIONS=300
M_RATE=0.03
C_RATE=0.5
CONVENIENT_EXPONENT=3

cp input.dummy input.in

sed -i "s/MAP_GEOMETRY_dummy/$MAP_GEOMETRY/g" input.in
sed -i "s/NUM_CITIES_dummy/$NUM_CITIES/g" input.in
sed -i "s/POPULATION_SIZE_dummy/$POPULATION_SIZE/g" input.in
sed -i "s/MAX_GENERATIONS_dummy/$MAX_GENERATIONS/g" input.in
sed -i "s/M_RATE_dummy/$M_RATE/g" input.in
sed -i "s/C_RATE_dummy/$C_RATE/g" input.in
sed -i "s/CONVENIENT_EXPONENT_dummy/$CONVENIENT_EXPONENT/g" input.in


echo -e "<PARAMETERS INFO>"
echo -e "MAP_GEOMETRY = $MAP_GEOMETRY"
echo -e "NUM_CITIES = $NUM_CITIES"
echo -e "POPULATION_SIZE = $POPULATION_SIZE"
echo -e "MAX_GENERATIONS = $MAX_GENERATIONS"
echo -e "M_RATE = $M_RATE"
echo -e "C_RATE = $C_RATE"
echo -e "CONVENIENT_EXPONENT = $CONVENIENT_EXPONENT"

./go.exe

mv -v out_cities.dat ./data/out_cities_circle.dat
mv -v out_order.dat ./data/out_order_circle.dat
mv -v out_L.dat ./data/out_L_circle.dat
mv -v out_L_Ave.dat ./data/out_L_Ave_circle.dat

echo -e "___________________________________________________________\n\n"

rm -rf output_*

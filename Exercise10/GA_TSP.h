#ifndef __GA_TSP__
#define __GA_TSP__
#include "../Libs/mylibs.h"
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"

struct City {
    double x;
    double y;
};

struct Tour {
    vector<int> order;
    double distance;
};

// Function to calculate the Euclidean distance between two cities
double distance(const City&, const City&);
void shuffle(vector<int>&);

// Funzione per calcolare la lunghezza del percorso (tour)
double tourLength(const Tour&, const vector<City>&);
double L2function(const Tour&, const vector<City>&);
// Funzione per generare una popolazione iniziale di tour casuali
vector<Tour> generateInitialPopulation(const vector<City>&);
bool CheckPopulation(vector<Tour>);
//inverte due interi
void swap(int&, int&);
void SwapTours(vector<Tour>&,int,int);
//genetics mutation operators
void swapTwoCities(vector<int>&);


//shift n contiguos cities of m steps from the second city
void shiftContiguosCities(vector<int>&);

void permuteContiguousCities(vector<int>&);
void inversionContiguousCities(vector<int>&);
bool isInVector(vector<int>, double);
void Crossover(vector<int>&,vector<int>&);
// Funzione per selezionare il migliore tra due tour
Tour selectBestTour(const Tour, const Tour);
//return the cities ordered following the best tour
vector<City> sortCities(vector<City>,vector<int>);

void sortPopulation(vector<Tour>&);
void PrintPop(vector<Tour>);
void PrintTour(Tour);

#endif

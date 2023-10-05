#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "main.h"
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"
using namespace std;

// Define global variables
bool MAP_GEOMETRY=0;
unsigned int NUM_CITIES = 34;
unsigned int POPULATION_SIZE = 50;
unsigned int MAX_GENERATIONS = 1000;
double M_RATE = 0.02;
double C_RATE = 0.5;
double CONVENIENT_EXPONENT = 3;
Random rnd;

struct City {
    int x;
    int y;
};

struct Tour {
    vector<int> order;
    double distance;
};

// Function to calculate the Euclidean distance between two cities
double distance(const City& city1, const City& city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

void shuffle(vector<int>& tour){
  vector<int> v_old = tour;
  for (long unsigned int i{1}; i<NUM_CITIES; i++){
      int id1 = i;
      int id2 = rnd.Rannyu(1,NUM_CITIES);

      int save_el = tour[id1];
      tour[id1] = tour[id2];
      tour[id2] = save_el;
  }
  return;
}

// Funzione per calcolare la lunghezza del percorso (tour)
double tourLength(const Tour& tour, const vector<City>& cities) {
    double totalDistance = 0.0;
    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
        totalDistance += distance(cities[tour.order[i]], cities[tour.order[i + 1]]);
    }
    totalDistance += distance(cities[tour.order[NUM_CITIES]], cities[tour.order[0]]);
    return totalDistance;
}

double L2function(const Tour& tour, const vector<City>& cities) {
    double totalDistance = 0.0;
    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
        totalDistance += pow(distance(cities[tour.order[i]], cities[tour.order[i + 1]]),2);
    }
    totalDistance += pow(distance(cities[tour.order[NUM_CITIES]], cities[tour.order[0]]),2);
    return totalDistance;
}

// Funzione per generare una popolazione iniziale di tour casuali
vector<Tour> generateInitialPopulation(const vector<City>& cities) {
    vector<Tour> population(POPULATION_SIZE);

    for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].order = vector<int>(NUM_CITIES);
        for (int j = 0; j < NUM_CITIES; ++j) {
            population[i].order[j] = j;
        }
        shuffle(population[i].order);
        population[i].distance = tourLength(population[i], cities);

        //for (long unsigned int l{};l<NUM_CITIES;l++)  cout<< population[i].order[l]<<" ";
        //cout<< "distance = "<<population[i].distance<<endl;

    }
    return population;
}

bool CheckPopulation(vector<Tour> pop){
  // Ciclo per confrontare ogni elemento con gli altri

  for (int k = 0;k<POPULATION_SIZE;++k){
    for (unsigned int i = 0; i < NUM_CITIES - 1; ++i) {
        for (int j = i + 1; j < NUM_CITIES; ++j) {
          if (pop[k].order[i] == pop[k].order[j]){
            cout<<"Problem: Trovato un elemento duplicato in un tour della popolazione"<<endl;
            for (int l{};l<NUM_CITIES;l++)
              cout<< pop[k].order[l]<<" ";
            cout<<endl;
            return 1;
          }
        }
    }
    /*
    if(pop[k].order.front() != pop[k].order.back()){
      cout<<"Problem: Salesman non torna alla città da cui è partito"<<endl;
      for (long unsigned int l{};l<pop[k].order.size();l++)  cout<< pop[k].order[l]<<" ";
      cout<<endl;
      return 1;
    }*/
  }
  return 0; // Nessun elemento duplicato trovato
}

//inverte due interi
void swap(int& id1, int& id2){

  int save_element = id1;

  id1 = id2;
  id2 = save_element;

  return;
}

void SwapTours(vector<Tour>& population, int in, int fi){

  Tour save_element = population[in];

  population[in] = population[fi];
  population[fi] = save_element;

  return;
}
//genetics mutation operators
void swapTwoCities(vector<int>& tour){

  int id1 = (int)rnd.Rannyu(1,NUM_CITIES);
  int id2 = (int)rnd.Rannyu(1,NUM_CITIES);
  int save_element = tour[id1];

  tour[id1] = tour[id2];
  tour[id2] = save_element;

  return;
}


//shift n contiguos cities of m steps from the second city
void shiftContiguosCities(vector<int>& tour)

{
    int n = rnd.Rannyu(0., NUM_CITIES - 1);
    int m = rnd.Rannyu(0., NUM_CITIES - 2);
    int a = rnd.Rannyu(0., NUM_CITIES - 2 - m);

    vector<int> vcopy = tour;

    vector<int> lcopy(NUM_CITIES);

    for (size_t i = 0; i < NUM_CITIES; ++i)
        lcopy[i] = i;

    lcopy.erase(lcopy.begin()); // remove the first one because it has to stay fixed

    vector<int> index(m); // create a vector of indexes that are to be relocated
    for (size_t i{}; i < index.size(); ++i)
        index[i] = (a + i + n) % (NUM_CITIES - 1);

    vector<int> copiator(m); // Vector on which temporarily copy the data
    for (size_t i{}; i < copiator.size(); ++i)
        copiator[i] = lcopy[a + i];

    for (unsigned int i{}; i < m; ++i) // eliminate the elements ranging from a to a+m
        lcopy.erase(lcopy.begin() + a);

    while (lcopy.size() < NUM_CITIES - 1)
    { // insert the deleted elements in the correct position
        for (unsigned int i{}; i < m; ++i)
        {
            if ((size_t)index[i] <= lcopy.size() && lcopy.size() < NUM_CITIES - 1)
                lcopy.emplace(lcopy.begin() + index[i], copiator[i]);
        }
    }

    // cout<<"checkpoint"<<endl;

    for (unsigned int i{1}; i < NUM_CITIES; ++i)
    {
        tour[i] = vcopy[lcopy[i - 1]];
        // cout<<i<<endl;
    }

}

void permuteContiguousCities(vector<int>& tour){

  // cout<<"Mutation 3"<<endl;
  double med = (double)NUM_CITIES / 2.;

  int m = rnd.Rannyu(1., med - 1);
  vector<int> vcopy = tour;

  int a = rnd.Rannyu(1., med - m);      //  generate the starting position of the m elements
  int b = rnd.Rannyu(med, 2 * med - m); // generate the starting position of the other m elements

  for (unsigned int i = a, j = b; i < a + m; ++i, ++j)
  {
      tour[j] = vcopy[i];
      tour[i] = vcopy[j];
  }

  return;
}

void inversionContiguousCities(vector<int>& tour)
{
    int m = rnd.Rannyu(1., NUM_CITIES - 2);
    vector<int> vcopy = tour;

    int a = rnd.Rannyu(0., NUM_CITIES - m);

    for (int i = 1;i<m;i++)
    {
        tour[a+i] = vcopy[a+m-i];
    }
}
bool isInVector(vector<int> v, double element){

  for (long unsigned int i = 0; i < v.size(); i++){
    if (v[i]==element){

      return 1;
    }
  }
  //cout<<"elemento NON trovato"<<endl;
  return 0;

}
void Crossover(vector<int>& parent1,vector<int>& parent2)
{
    int cutter = rnd.Rannyu(NUM_CITIES/2, NUM_CITIES-1);
    //cout<<cutter<<" "<<NUM_CITIES<<endl;
    vector<int> copy_p2 = parent2;
    vector<int> copy_p1 = parent1;
    vector<int> copy_p1_cut = copy_p1;
    vector<int> copy_p2_cut = copy_p2;

    copy_p1_cut.resize(cutter);
    copy_p2_cut.resize(cutter);

    unsigned int count{};
    for (unsigned int i{1}; i < NUM_CITIES; i++) {
      if(isInVector(copy_p2_cut,copy_p1[i])==0){
        parent2[cutter+count] = copy_p1[i];
        count++;
      }
    }

    count = 0;
    for (unsigned int i{1}; i < NUM_CITIES; i++) {
      if(isInVector(copy_p1_cut,copy_p2[i]) ==0){
        parent1[cutter+count] = copy_p2[i];
        count++;
      }
    }

    return;
}
// Funzione per selezionare il migliore tra due tour
Tour selectBestTour(const Tour& tour1, const Tour& tour2) {
    //cout<<endl<<tour1.distance<<" "<< tour2.distance<<endl;
    return (tour1.distance < tour2.distance) ? tour1 : tour2;
}

//return the cities ordered following the best tour
vector<City> sortCities(vector<City> unsortedCities,vector<int> sortingVec){
   vector<City> newCities(NUM_CITIES);
   for( unsigned int i{}; i< NUM_CITIES;i++){
       newCities[i].x = unsortedCities[sortingVec[i]].x;
       newCities[i].y = unsortedCities[sortingVec[i]].y;
     }
   return newCities;
}

void sortPopulation(vector<Tour>& population){

  unsigned int id{};
  for( int j{}; j< POPULATION_SIZE;j++){
    Tour bestTour = population[j];
    for( unsigned int i{j}; i< POPULATION_SIZE;i++){
      if (bestTour.distance > population[i].distance){
        bestTour = population[i];
        id = i;
      }
    }
    SwapTours(population, j, id);
  }

  return;
}

void PrintPop(vector<Tour> population){
    cout<<"Distances:"<<endl;
    for( int j{}; j< POPULATION_SIZE;j++) cout<<population[j].distance<<" ";
    cout<<endl<<endl;
}

void PrintTour(Tour tour){
    cout<<"Tour:"<<endl;
    for( int j{}; j< NUM_CITIES;j++) cout<<tour.order[j]<<" ";
    cout<<endl<<endl;
}

void Input(void)
{
  ifstream ReadInput;

  rnd.init_random();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> MAP_GEOMETRY;

  ReadInput >> NUM_CITIES;

  ReadInput >> POPULATION_SIZE;

  ReadInput >> MAX_GENERATIONS;

  ReadInput >> M_RATE;

  ReadInput >> C_RATE;

  ReadInput >> CONVENIENT_EXPONENT;

  ReadInput.close();
}
int main() {
    Input();
    //cout<<"PROVA "<<pow(rnd.Rannyu(),CONVENIENT_EXPONENT)<<endl;
    double bestL_ave{};
    vector<City> cities(NUM_CITIES);
    // Inizializza le città con coordinate casuali
    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
      if (!MAP_GEOMETRY){
        cities[i].x = (int)(rnd.Rannyu()*100);
        cities[i].y = (int)(rnd.Rannyu()*100);
      }else{
        double theta = rnd.RanAngle();
        cities[i].x = (int)(cos(theta)*100);
        cities[i].y = (int)(sin(theta)*100);
      }
    }
    vector<Tour> population = generateInitialPopulation(cities);
    //CheckPopulation(population);

    ofstream out_L;
    out_L.open("out_L.dat",ios::app);

    Tour bestTour = population[0];
    for (int generation = 1; generation <= MAX_GENERATIONS; ++generation) {

        if(generation % (int)(MAX_GENERATIONS/10) == 0) cout<<generation/(int)(MAX_GENERATIONS/10)*10<<"%"<<endl;

        bestL_ave = 0;
        vector<Tour> newPopulation;

        // Crossover e mutazione per generare la nuova popolazione

        for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
            sortPopulation(population);
            int p1 = ((int)(POPULATION_SIZE*pow(rnd.Rannyu(),CONVENIENT_EXPONENT)));
            int p2 = ((int)(POPULATION_SIZE*pow(rnd.Rannyu(),CONVENIENT_EXPONENT)));

            //fill the vector of the half lower L values and then I apply the average block for blocks of POPULATION_SIZE/2 size
            if (i < POPULATION_SIZE/2)  bestL_ave += population[i].distance/(POPULATION_SIZE/2);

            //cout<<" fraction of generation: "<<i<< "of" << POPULATION_SIZE<<endl;
            if (rnd.Rannyu()<M_RATE){
              //cout<<"-----------start mutation 1-----------"<<endl;
              //PrintTour(population[p1]);
              swapTwoCities(population[p1].order);
              //CheckPopulation(population);

              //PrintTour(population[p1]);
              //cout<<"-----------end mutation-----------"<<endl;
              population[p1].distance = tourLength(population[p1], cities);
            }

            if (rnd.Rannyu()<M_RATE){
              //cout<<"-----------start mutation 2-----------"<<endl;
              //PrintTour(population[p1]);
              shiftContiguosCities(population[p1].order);
              //PrintTour(population[p1]);
              //CheckPopulation(population);
              population[p1].distance = tourLength(population[p1], cities);

              //cout<<"-----------end mutation-----------"<<endl<<endl;
            }


            if (rnd.Rannyu()<M_RATE){
              //cout<<"-----------start mutation 3-----------"<<endl;
              //PrintTour(population[p1]);
              permuteContiguousCities(population[p1].order);
              //PrintTour(population[p1]);
              //CheckPopulation(population);
              population[p1].distance = tourLength(population[p1], cities);

              //cout<<"-----------end mutation-----------"<<endl<<endl;
            }
            if (rnd.Rannyu()<M_RATE){
              //cout<<"-----------start mutation 4-----------"<<endl;
              //PrintTour(population[p1]);
              inversionContiguousCities(population[p1].order);
              //PrintTour(population[p1]);
              //CheckPopulation(population);
              population[p1].distance = tourLength(population[p1], cities);

              //cout<<"-----------end mutation-----------"<<endl<<endl;
            }
            //PrintTour(population[p1]);

            if (rnd.Rannyu()<C_RATE){
              //cout<<"-----------start Crossover-----------"<<endl;
              //PrintTour(population[p1]);
              //PrintTour(population[p2]);
              Crossover(population[p1].order, population[p2].order);
              //CheckPopulation(population);
              //PrintTour(population[p1]);
              //PrintTour(population[p2]);
              population[p1].distance = tourLength(population[p1], cities);
              population[p2].distance = tourLength(population[p2], cities);
              //cout<<"-----------end crossover-----------"<<endl<<endl;
            }

            //CheckPopulation(population);
            for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
                bestTour = selectBestTour(bestTour, population[i]);
            }

            newPopulation.push_back(bestTour);
        }
        population = newPopulation;

        for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
            bestTour = selectBestTour(bestTour, population[i]);
        }

        out_L<<generation<<setw(15)<<bestTour.distance<<setw(15)<<bestL_ave<<endl;
    }
    out_L.close();


    vector<City> sortedCities(NUM_CITIES);

    sortedCities = sortCities(cities,bestTour.order);
    //sortYCities = sortCities(cities.y,bestTour.order);
    // Output del miglior tour e della sua lunghezza
    cout << "Best Tour Order: ";
    ofstream out_order, out_cities;
    out_cities.open("out_cities.dat",ios::app);
    out_order.open("out_order.dat",ios::app);

    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
        cout << bestTour.order[i] << " ";
        out_order << i <<setw(15)<< bestTour.order[i]<<endl;
        out_cities<<i<<setw(15)<<sortedCities[i].x<<setw(15)<<sortedCities[i].y<<endl;
        //cout<<i<<setw(15)<<sortXCities<<setw(15)<<sortYCities<<endl;
    }
    out_order.close();
    out_cities.close();
    cout << endl;

    cout << "Best Tour Length: " << bestTour.distance << endl;

    return 0;
}
